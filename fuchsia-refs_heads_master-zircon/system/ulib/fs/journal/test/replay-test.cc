// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <lib/zx/vmo.h>

#include <map>

#include <fs/journal/replay.h>
#include <zxtest/zxtest.h>

#include "entry-view.h"

namespace fs {
namespace {

const vmoid_t kInfoVmoid = 1;
const vmoid_t kJournalVmoid = 2;
const vmoid_t kOtherVmoid = 3;
const size_t kJournalLength = 10;
const uint32_t kBlockSize = 8192;
const uint64_t kGoldenSequenceNumber = 1337;

class MockVmoidRegistry : public fs::VmoidRegistry {
 public:
  ~MockVmoidRegistry() { ZX_ASSERT(vmos_.empty()); }

  const zx::vmo& GetVmo(vmoid_t vmoid) const { return *(vmos_.at(vmoid)); }

  void SetNextVmoid(vmoid_t vmoid) { next_vmoid_ = vmoid; }

 private:
  zx_status_t AttachVmo(const zx::vmo& vmo, vmoid_t* out) override {
    vmos_.emplace(std::make_pair(next_vmoid_, zx::unowned_vmo(vmo.get())));
    *out = next_vmoid_++;
    return ZX_OK;
  }

  zx_status_t DetachVmo(vmoid_t vmoid) final {
    vmos_.erase(vmoid);
    return ZX_OK;
  }

  vmoid_t next_vmoid_ = BLOCK_VMOID_INVALID;
  std::map<vmoid_t, zx::unowned_vmo> vmos_;
};

class ParseJournalTestFixture : public zxtest::Test {
 public:
  virtual ~ParseJournalTestFixture() = default;

  void SetUp() override {
    auto info_block_buffer = std::make_unique<fs::VmoBuffer>();
    registry_.SetNextVmoid(kInfoVmoid);
    ASSERT_OK(info_block_buffer->Initialize(&registry_, 1, kBlockSize, "info-block"));
    info_block_ = JournalSuperblock(std::move(info_block_buffer));

    registry_.SetNextVmoid(kJournalVmoid);
    ASSERT_OK(journal_buffer_.Initialize(&registry_, kJournalLength, kBlockSize, "journal"));

    registry_.SetNextVmoid(kOtherVmoid);
  }

  JournalSuperblock* info_block() { return &info_block_; }
  fs::VmoBuffer* journal_buffer() { return &journal_buffer_; }
  MockVmoidRegistry* registry() { return &registry_; }

 private:
  MockVmoidRegistry registry_;
  JournalSuperblock info_block_;
  fs::VmoBuffer journal_buffer_;
};

using ParseJournalTest = ParseJournalTestFixture;

TEST_F(ParseJournalTest, ParseBadJournalExpectError) {
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  EXPECT_EQ(ZX_ERR_IO,
            ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
}

TEST_F(ParseJournalTest, ParseEmptyJournalNoOperations) {
  info_block()->Update(0, 0);

  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  EXPECT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  EXPECT_EQ(0, operations.size());
  EXPECT_EQ(0, sequence_number);
}

TEST_F(ParseJournalTest, ParseEmptyJournalNonzeroSequenceNumber) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  EXPECT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  EXPECT_EQ(0, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber, sequence_number);
}

void AddOperation(uint64_t dev_offset, uint64_t length,
                  fbl::Vector<fs::BufferedOperation>* operations) {
  fs::BufferedOperation operation;
  operation.op.type = fs::OperationType::kWrite;
  operation.op.dev_offset = dev_offset;
  operation.op.length = length;
  operations->push_back(std::move(operation));
}

void CheckWriteOperation(const fs::BufferedOperation& operation, uint64_t vmo_offset,
                         uint64_t dev_offset, uint64_t length) {
  EXPECT_EQ(kJournalVmoid, operation.vmoid);
  EXPECT_EQ(fs::OperationType::kWrite, operation.op.type);
  EXPECT_EQ(vmo_offset, operation.op.vmo_offset);
  EXPECT_EQ(dev_offset, operation.op.dev_offset);
  EXPECT_EQ(length, operation.op.length);
}

TEST_F(ParseJournalTest, ParseOneEntryOneOperation) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);

  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(1, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 1, sequence_number);
  uint64_t vmo_offset = kJournalEntryHeaderBlocks;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, 10, 1));
}

TEST_F(ParseJournalTest, ParseOneEntryOneOperationFullJournal) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  const uint64_t kDevOffset = 10;  // Arbitrary
  const uint64_t kLength = kJournalLength - kEntryMetadataBlocks;
  AddOperation(kDevOffset, kLength, &ops);

  const uint64_t kEntryLength = kLength + kEntryMetadataBlocks;
  static_assert(kEntryLength == kJournalLength, "Attempting to test full journal");
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(1, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 1, sequence_number);
  uint64_t vmo_offset = kJournalEntryHeaderBlocks;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, kDevOffset, kLength));
}

TEST_F(ParseJournalTest, ParseOneEntryOneOperationWrapsAroundJournal) {
  uint64_t vmo_offset = kJournalLength - 2;
  info_block()->Update(vmo_offset, kGoldenSequenceNumber);

  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 4, &ops);

  const uint64_t kEntryLength = 4 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), vmo_offset, kEntryLength), ops,
                              kGoldenSequenceNumber);

  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(2, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 1, sequence_number);
  vmo_offset += kJournalEntryHeaderBlocks;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, 10, 1));
  vmo_offset = 0;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[1], vmo_offset, 11, 3));
}

TEST_F(ParseJournalTest, ParseOneEntryManyOperations) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 3, &ops);
  AddOperation(/* dev_offset= */ 20, /* length= */ 2, &ops);
  AddOperation(/* dev_offset= */ 30, /* length= */ 1, &ops);

  const uint64_t kEntryLength = 6 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  EXPECT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  EXPECT_EQ(3, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 1, sequence_number);
  uint64_t vmo_offset = kJournalEntryHeaderBlocks;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, 10, 3));
  vmo_offset += 3;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[1], vmo_offset, 20, 2));
  vmo_offset += 2;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[2], vmo_offset, 30, 1));
}

TEST_F(ParseJournalTest, ParseMultipleEntries) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);
  const uint64_t kEntryLengthA = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view_a(fs::BlockBufferView(journal_buffer(), 0, kEntryLengthA), ops,
                                kGoldenSequenceNumber);

  ops.reset();
  AddOperation(/* dev_offset= */ 20, /* length= */ 3, &ops);
  const uint64_t kEntryLengthB = 3 + kEntryMetadataBlocks;
  JournalEntryView entry_view_b(fs::BlockBufferView(journal_buffer(), kEntryLengthA, kEntryLengthB),
                                ops, kGoldenSequenceNumber + 1);

  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(2, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 2, sequence_number);
  uint64_t vmo_offset = kJournalEntryHeaderBlocks;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, 10, 1));
  vmo_offset += kEntryLengthA;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[1], vmo_offset, 20, 3));
}

// Tests that contiguous entries with a non-increasing sequence number will
// be discarded. In a functioning journal, each subsequent entry will have exclusively
// incrementing sequence numbers, and deviation from that behavior will imply "invalid
// journal metadata" that should be discarded. This tests one of those deviations (sequence number
// is not incremented), and validates that the bad entry is ignored.
TEST_F(ParseJournalTest, ParseMultipleEntriesWithSameSequenceNumberOnlyKeepsFirst) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);
  const uint64_t kEntryLengthA = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view_a(fs::BlockBufferView(journal_buffer(), 0, kEntryLengthA), ops,
                                kGoldenSequenceNumber);

  ops.reset();
  AddOperation(/* dev_offset= */ 20, /* length= */ 3, &ops);
  const uint64_t kEntryLengthB = 3 + kEntryMetadataBlocks;
  JournalEntryView entry_view_b(fs::BlockBufferView(journal_buffer(), kEntryLengthA, kEntryLengthB),
                                ops, kGoldenSequenceNumber);

  // Writing entries with the same sequence number only parses the first.
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(1, operations.size());
  ASSERT_EQ(kGoldenSequenceNumber + 1, sequence_number);
  uint64_t vmo_offset = kJournalEntryHeaderBlocks;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, 10, 1));
}

TEST_F(ParseJournalTest, ParseEscapedEntry) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);
  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;

  // Create an "escaped" entry.
  fs::BlockBufferView view(journal_buffer(), 1, 1);
  auto ptr = static_cast<uint64_t*>(view.Data(0));
  ptr[0] = kJournalEntryMagic;
  ptr[1] = 0xDEADBEEF;

  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  // Verify that it was escaped.
  const auto& const_entry_view = entry_view;
  EXPECT_EQ(kJournalBlockDescriptorFlagEscapedBlock, const_entry_view.header()->target_flags[0]);
  EXPECT_EQ(0, ptr[0]);
  EXPECT_EQ(0xDEADBEEF, ptr[1]);

  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(1, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 1, sequence_number);
  uint64_t vmo_offset = kJournalEntryHeaderBlocks;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, 10, 1));

  // Verify that the entry is un-escaped after parsing.
  EXPECT_EQ(kJournalEntryMagic, ptr[0]);
  EXPECT_EQ(0xDEADBEEF, ptr[1]);
}

TEST_F(ParseJournalTest, ParseTooOldDropped) {
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);

  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  // Move the info block past this counter, but in the same location.
  info_block()->Update(0, kGoldenSequenceNumber + 1);

  // Observe that the new sequence_number is parsed, but the entry is dropped.
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(0, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 1, sequence_number);
}

TEST_F(ParseJournalTest, ParseNewerThanExpectedDropped) {
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);

  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  // Move the info block backwards in time.
  const uint64_t kUpdatedSequenceNumber = kGoldenSequenceNumber - 1;
  info_block()->Update(0, kUpdatedSequenceNumber);

  // Observe that the entry's sequence_number is parsed as too new, and dropped.
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  EXPECT_EQ(0, operations.size());
  EXPECT_EQ(kUpdatedSequenceNumber, sequence_number);
}

TEST_F(ParseJournalTest, ParseEntryMultipleTimes) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);

  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  // Observe that we can replay journal entries with this setup.
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(1, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 1, sequence_number);
  uint64_t vmo_offset = kJournalEntryHeaderBlocks;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, 10, 1));
  operations.reset();

  // We can replay the same entries multiple times.
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(1, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 1, sequence_number);
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, 10, 1));
}

TEST_F(ParseJournalTest, ParseEntryModifiedHeaderDropped) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);

  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  // Before we replay, flip some bits in the header.
  fs::BlockBufferView buffer_view(journal_buffer(), 0, kEntryLength);
  auto raw_block = static_cast<JournalHeaderBlock*>(buffer_view.Data(0));
  raw_block->target_blocks[16] = ~(raw_block->target_blocks[16]);

  // As a result, there are no entries identified as replayable.
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(0, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber, sequence_number);
}

TEST_F(ParseJournalTest, ParseEntryModifiedEntryDropped) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);

  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  // Before we replay, flip some bits in the entry.
  fs::BlockBufferView buffer_view(journal_buffer(), 0, kEntryLength);
  auto raw_bytes = static_cast<uint8_t*>(buffer_view.Data(1));
  raw_bytes[0] = static_cast<uint8_t>(~raw_bytes[0]);

  // As a result, there are no entries identified as replayable.
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(0, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber, sequence_number);
}

TEST_F(ParseJournalTest, ParseEntryModifiedCommitDropped) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);

  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  // Before we replay, flip some bits in the commit.
  fs::BlockBufferView buffer_view(journal_buffer(), 0, kEntryLength);
  auto raw_commit = static_cast<JournalCommitBlock*>(buffer_view.Data(2));
  raw_commit->prefix.sequence_number++;

  // As a result, there are no entries identified as replayable.
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(0, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber, sequence_number);
}

TEST_F(ParseJournalTest, ParseEntryModifiedAfterCommitStillKept) {
  info_block()->Update(0, kGoldenSequenceNumber);
  fbl::Vector<fs::BufferedOperation> ops;
  AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);

  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                              kGoldenSequenceNumber);

  // Before we replay, flip some bits in the commit block.
  fs::BlockBufferView buffer_view(journal_buffer(), 0, kEntryLength);
  auto raw_bytes = static_cast<uint8_t*>(buffer_view.Data(2));
  // Intentionally flip bits AFTER the commit structure itself, but still in the
  // same block.
  size_t index = sizeof(JournalCommitBlock) + 1;
  raw_bytes[index] = static_cast<uint8_t>(~raw_bytes[index]);

  // The current implementation of journaling is not checksumming the commit block.
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  ASSERT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(1, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber + 1, sequence_number);
  uint64_t vmo_offset = kJournalEntryHeaderBlocks;
  ASSERT_NO_FAILURES(CheckWriteOperation(operations[0], vmo_offset, 10, 1));
}

TEST_F(ParseJournalTest, ParseDetectsCorruptJournalIfOldEntryHasBadChecksumButGoodLength) {
  info_block()->Update(0, kGoldenSequenceNumber);
  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  // Place two entries into the journal.
  {
    fbl::Vector<fs::BufferedOperation> ops;
    AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);
    JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                                kGoldenSequenceNumber);
  }
  {
    fbl::Vector<fs::BufferedOperation> ops;
    AddOperation(/* dev_offset= */ 20, /* length= */ 1, &ops);
    JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), kEntryLength, kEntryLength),
                                ops, kGoldenSequenceNumber + 1);
  }

  // Before we replay, flip some bits in the old entry's header.
  fs::BlockBufferView buffer_view(journal_buffer(), 0, kEntryLength);
  auto raw_block = static_cast<JournalHeaderBlock*>(buffer_view.Data(0));
  raw_block->target_blocks[16] = ~(raw_block->target_blocks[16]);

  // As a result, there are no entries identified as replayable, and
  // (because the second entry was valid, but the first entry wasn't) the journal
  // is identified as corrupt.
  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  EXPECT_EQ(ZX_ERR_IO_DATA_INTEGRITY,
            ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
}

TEST_F(ParseJournalTest, ParseDoesntDetectCorruptJournalIfOldEntryHasBadChecksumAndBadLength) {
  info_block()->Update(0, kGoldenSequenceNumber);
  const uint64_t kEntryLength = 1 + kEntryMetadataBlocks;
  // Place two entries into the journal.
  {
    fbl::Vector<fs::BufferedOperation> ops;
    AddOperation(/* dev_offset= */ 10, /* length= */ 1, &ops);
    JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), 0, kEntryLength), ops,
                                kGoldenSequenceNumber);
  }
  {
    fbl::Vector<fs::BufferedOperation> ops;
    AddOperation(/* dev_offset= */ 20, /* length= */ 1, &ops);
    JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), kEntryLength, kEntryLength),
                                ops, kGoldenSequenceNumber + 1);
  }

  // Before we replay, flip some bits in the old entry's header.
  //
  // This time, flip the number of blocks to be replayed, so the subsequent entry
  // cannot be located.
  fs::BlockBufferView buffer_view(journal_buffer(), 0, kEntryLength);
  auto raw_block = static_cast<JournalHeaderBlock*>(buffer_view.Data(0));
  raw_block->payload_blocks = ~(raw_block->payload_blocks);

  fbl::Vector<fs::BufferedOperation> operations;
  uint64_t sequence_number = 0;
  EXPECT_OK(ParseJournalEntries(info_block(), journal_buffer(), &operations, &sequence_number));
  ASSERT_EQ(0, operations.size());
  EXPECT_EQ(kGoldenSequenceNumber, sequence_number);
}

class MockTransactionHandler final : public fs::TransactionHandler {
 public:
  using TransactionCallback =
      fit::function<zx_status_t(const block_fifo_request_t* requests, size_t count)>;

  MockTransactionHandler() = default;
  MockTransactionHandler(TransactionCallback* callbacks, size_t transactions_expected)
      : callbacks_(callbacks), transactions_expected_(transactions_expected) {}

  ~MockTransactionHandler() { EXPECT_EQ(transactions_expected_, transactions_seen_); }

  uint32_t FsBlockSize() const final { return kBlockSize; }

  groupid_t BlockGroupID() final { return 1; }

  uint32_t DeviceBlockSize() const final { return kBlockSize; }

  uint64_t BlockNumberToDevice(uint64_t block_num) const final {
    return block_num;
  }

  block_client::BlockDevice* GetDevice() final { return nullptr; }

  zx_status_t RunOperation(const fs::Operation& operation, fs::BlockBuffer* buffer) final {
    return ZX_ERR_NOT_SUPPORTED;
  }

  zx_status_t Transaction(block_fifo_request_t* requests, size_t count) override {
    EXPECT_LT(transactions_seen_, transactions_expected_);
    if (transactions_seen_ == transactions_expected_) {
      return ZX_ERR_BAD_STATE;
    }
    return callbacks_[transactions_seen_++](requests, count);
  }

 private:
  TransactionCallback* callbacks_ = nullptr;
  size_t transactions_expected_ = 0;
  size_t transactions_seen_ = 0;
};

class ReplayJournalTest : public ParseJournalTestFixture {
 public:
  static constexpr uint64_t kJournalAreaStart = 5;
  static constexpr uint64_t kJournalAreaLength = kJournalLength + kJournalMetadataBlocks;
  static constexpr uint64_t kJournalEntryStart = kJournalAreaStart + kJournalMetadataBlocks;
  static constexpr uint64_t kJournalEntryLength = kJournalLength;

  void ValidInfoReadRequest(const block_fifo_request_t& request) const {
    EXPECT_EQ(BLOCKIO_READ, request.opcode);
    EXPECT_EQ(0, request.vmo_offset);
    EXPECT_EQ(kJournalAreaStart, request.dev_offset);
    EXPECT_EQ(kJournalMetadataBlocks, request.length);
  }

  void ValidEntriesReadRequest(const block_fifo_request_t& request) const {
    EXPECT_EQ(BLOCKIO_READ, request.opcode);
    EXPECT_EQ(0, request.vmo_offset);
    EXPECT_EQ(kJournalEntryStart, request.dev_offset);
    EXPECT_EQ(kJournalEntryLength, request.length);
  }

  // Take the contents of the pre-registered journal superblock and transfer
  // it into the requested vmoid.
  void TransferInfoTo(vmoid_t vmoid) {
    char buf[kBlockSize * kJournalMetadataBlocks];
    EXPECT_OK(registry()->GetVmo(kInfoVmoid).read(buf, 0, sizeof(buf)));
    EXPECT_OK(registry()->GetVmo(vmoid).write(buf, 0, sizeof(buf)));
  }

  // Take the contents of the pre-registered journal buffer and transfer
  // it into the requested vmoid.
  void TransferEntryTo(vmoid_t vmoid, size_t offset, uint64_t length) {
    char entry_buf[kBlockSize * length];
    EXPECT_OK(
        registry()->GetVmo(kJournalVmoid).read(entry_buf, offset * kBlockSize, sizeof(entry_buf)));
    EXPECT_OK(registry()->GetVmo(vmoid).write(entry_buf, offset * kBlockSize, sizeof(entry_buf)));
  }
};

TEST_F(ReplayJournalTest, ReplayBadJournalSuperblockFails) {
  MockTransactionHandler::TransactionCallback callbacks[] = {
      [&](const block_fifo_request_t* requests, size_t count) {
        // Return OK, but don't provide any values. This should fail during replay.
        EXPECT_LE(1, count);
        EXPECT_EQ(BLOCKIO_READ, requests[0].opcode);
        return ZX_OK;
      },
  };
  MockTransactionHandler transaction_handler(callbacks, std::size(callbacks));
  JournalSuperblock superblock;
  ASSERT_EQ(ZX_ERR_IO, ReplayJournal(&transaction_handler, registry(), kJournalAreaStart,
                                     kJournalAreaLength, &superblock));
}

TEST_F(ReplayJournalTest, ReplayCannotReadJournalFails) {
  MockTransactionHandler::TransactionCallback callbacks[] = {
      [&](const block_fifo_request_t* requests, size_t count) {
        EXPECT_LE(1, count);
        EXPECT_EQ(BLOCKIO_READ, requests[0].opcode);
        return ZX_ERR_IO;
      },
  };
  MockTransactionHandler transaction_handler(callbacks, std::size(callbacks));
  JournalSuperblock superblock;
  ASSERT_EQ(ZX_ERR_IO, ReplayJournal(&transaction_handler, registry(), kJournalAreaStart,
                                     kJournalAreaLength, &superblock));
}

TEST_F(ReplayJournalTest, ReplayEmptyJournalDoesNothing) {
  // Fill the preregistered info block with valid data.
  constexpr uint64_t kStart = 1;
  constexpr uint64_t kSequenceNumber = 3;
  info_block()->Update(kStart, kSequenceNumber);

  MockTransactionHandler::TransactionCallback callbacks[] = {
      [&](const block_fifo_request_t* requests, size_t count) {
        // First request: Reading from the journal.
        EXPECT_EQ(2, count);
        ValidInfoReadRequest(requests[0]);
        ValidEntriesReadRequest(requests[1]);

        // Transfer pre-seeded info block, but nothing else.
        TransferInfoTo(requests[0].vmoid);
        return ZX_OK;
      },
  };
  MockTransactionHandler transaction_handler(callbacks, std::size(callbacks));
  JournalSuperblock superblock;
  ASSERT_OK(ReplayJournal(&transaction_handler, registry(), kJournalAreaStart, kJournalAreaLength,
                          &superblock));
  EXPECT_EQ(kStart, superblock.start());
  EXPECT_EQ(kSequenceNumber, superblock.sequence_number());
}

TEST_F(ReplayJournalTest, ReplayOneEntry) {
  // Fill the pre-registered info block with valid data.
  constexpr uint64_t kStart = 0;
  constexpr uint64_t kSequenceNumber = 3;
  info_block()->Update(kStart, kSequenceNumber);

  // Fill the pre-registered journal buffer with one entry.
  fbl::Vector<fs::BufferedOperation> operations = {
      {
          .vmoid = 0,
          {
              .type = fs::OperationType::kWrite,
              .vmo_offset = 0,
              .dev_offset = 1234,
              .length = 1,
          },
      },
  };
  uint64_t entry_size = operations[0].op.length + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), kStart, entry_size), operations,
                              kSequenceNumber);

  MockTransactionHandler::TransactionCallback callbacks[] = {
      [&](const block_fifo_request_t* requests, size_t count) {
        // First request: Reading from the journal.
        // Transfer the pre-seeded info block.
        EXPECT_EQ(2, count);
        ValidInfoReadRequest(requests[0]);
        ValidEntriesReadRequest(requests[1]);

        // Transfer the pre-seeded journal entry.
        TransferInfoTo(requests[0].vmoid);
        TransferEntryTo(requests[1].vmoid, kStart, entry_size);
        return ZX_OK;
      },
      [&](const block_fifo_request_t* requests, size_t count) {
        // Observe that the replay code replays the provided operation.
        EXPECT_EQ(1, count);
        EXPECT_EQ(BLOCKIO_WRITE, requests[0].opcode);
        EXPECT_EQ(kJournalEntryHeaderBlocks, requests[0].vmo_offset);
        EXPECT_EQ(operations[0].op.dev_offset, requests[0].dev_offset);
        EXPECT_EQ(operations[0].op.length, requests[0].length);
        return ZX_OK;
      }};
  MockTransactionHandler transaction_handler(callbacks, std::size(callbacks));
  JournalSuperblock superblock;
  ASSERT_OK(ReplayJournal(&transaction_handler, registry(), kJournalAreaStart, kJournalAreaLength,
                          &superblock));
  EXPECT_EQ(kStart, superblock.start());
  // The sequence_number should have advanced to avoid replaying the old entry.
  EXPECT_EQ(kSequenceNumber + 1, superblock.sequence_number());
}

TEST_F(ReplayJournalTest, ReplayCannotWriteParsedEntriesFails) {
  // Fill the pre-registered info block with valid data.
  constexpr uint64_t kStart = 0;
  constexpr uint64_t kSequenceNumber = 3;
  info_block()->Update(kStart, kSequenceNumber);

  // Fill the pre-registered journal buffer with one entry.
  fbl::Vector<fs::BufferedOperation> operations = {
      {
          .vmoid = 0,
          {
              .type = fs::OperationType::kWrite,
              .vmo_offset = 0,
              .dev_offset = 1234,
              .length = 1,
          },
      },
  };
  uint64_t entry_size = operations[0].op.length + kEntryMetadataBlocks;
  JournalEntryView entry_view(fs::BlockBufferView(journal_buffer(), kStart, entry_size), operations,
                              kSequenceNumber);

  MockTransactionHandler::TransactionCallback callbacks[] = {
      [&](const block_fifo_request_t* requests, size_t count) {
        // First request: Reading from the journal.
        // Transfer the pre-seeded info block.
        EXPECT_EQ(2, count);
        ValidInfoReadRequest(requests[0]);
        ValidEntriesReadRequest(requests[1]);

        // Transfer the pre-seeded journal entry.
        TransferInfoTo(requests[0].vmoid);
        TransferEntryTo(requests[1].vmoid, kStart, entry_size);
        return ZX_OK;
      },
      [&](const block_fifo_request_t* requests, size_t count) {
        // Observe that the replay code replays the provided operation, but return
        // an error instead.
        EXPECT_EQ(1, count);
        EXPECT_EQ(BLOCKIO_WRITE, requests[0].opcode);
        return ZX_ERR_IO;
      }};
  MockTransactionHandler transaction_handler(callbacks, std::size(callbacks));
  JournalSuperblock superblock;
  ASSERT_EQ(ZX_ERR_IO, ReplayJournal(&transaction_handler, registry(), kJournalAreaStart,
                                     kJournalAreaLength, &superblock));
}

}  // namespace
}  // namespace fs
