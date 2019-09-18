// WARNING: This file is machine generated by fidlgen.

#pragma once

#include <lib/fidl/internal.h>
#include <lib/fidl/llcpp/array.h>
#include <lib/fidl/llcpp/coding.h>
#include <lib/fidl/llcpp/string_view.h>
#include <lib/fidl/llcpp/sync_call.h>
#include <lib/fidl/llcpp/traits.h>
#include <lib/fidl/llcpp/transaction.h>
#include <lib/fidl/llcpp/vector_view.h>
#include <lib/fit/function.h>
#include <lib/zx/channel.h>
#include <lib/zx/vmo.h>
#include <zircon/fidl.h>

namespace llcpp {

namespace fuchsia {
namespace hardware {
namespace skipblock {

struct WriteBytesOperation;
struct ReadWriteOperation;
struct PartitionInfo;
class SkipBlock;

extern "C" const fidl_type_t fuchsia_hardware_skipblock_WriteBytesOperationTable;

struct WriteBytesOperation {
  static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_WriteBytesOperationTable;
  static constexpr uint32_t MaxNumHandles = 1;
  static constexpr uint32_t PrimarySize = 32;
  [[maybe_unused]]
  static constexpr uint32_t MaxOutOfLine = 0;

  // Memory object describing buffer to write from.
  ::zx::vmo vmo = {};

  // VMO offset in bytes.
  uint64_t vmo_offset = {};

  // Device offset in bytes to begin operation from.
  // Must be flash page aligned.
  uint64_t offset = {};

  // Number of bytes to write.
  // Must be flash page aligned.
  uint64_t size = {};
};

extern "C" const fidl_type_t fuchsia_hardware_skipblock_ReadWriteOperationTable;

struct ReadWriteOperation {
  static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_ReadWriteOperationTable;
  static constexpr uint32_t MaxNumHandles = 1;
  static constexpr uint32_t PrimarySize = 24;
  [[maybe_unused]]
  static constexpr uint32_t MaxOutOfLine = 0;

  // Memory object describing buffer to read into or write from.
  ::zx::vmo vmo = {};

  // VMO offset in bytes.
  uint64_t vmo_offset = {};

  // Block # to begin operation from.
  uint32_t block = {};

  // Number of blocks to read or write.
  uint32_t block_count = {};
};

extern "C" const fidl_type_t fuchsia_hardware_skipblock_PartitionInfoTable;

struct PartitionInfo {
  static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_PartitionInfoTable;
  static constexpr uint32_t MaxNumHandles = 0;
  static constexpr uint32_t PrimarySize = 32;
  [[maybe_unused]]
  static constexpr uint32_t MaxOutOfLine = 0;

  // Partition type GUID.
  ::fidl::Array<uint8_t, 16> partition_guid = {};

  // Describes the read/write size.
  uint64_t block_size_bytes = {};

  // Describes size of partition in terms of blocks.
  uint32_t partition_block_count = {};
};

extern "C" const fidl_type_t fuchsia_hardware_skipblock_SkipBlockGetPartitionInfoResponseTable;
extern "C" const fidl_type_t fuchsia_hardware_skipblock_SkipBlockReadRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_skipblock_SkipBlockReadResponseTable;
extern "C" const fidl_type_t fuchsia_hardware_skipblock_SkipBlockWriteRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_skipblock_SkipBlockWriteResponseTable;
extern "C" const fidl_type_t fuchsia_hardware_skipblock_SkipBlockWriteBytesRequestTable;
extern "C" const fidl_type_t fuchsia_hardware_skipblock_SkipBlockWriteBytesResponseTable;

class SkipBlock final {
  SkipBlock() = delete;
 public:

  struct GetPartitionInfoResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t status;
    PartitionInfo partition_info;

    static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_SkipBlockGetPartitionInfoResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 56;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  using GetPartitionInfoRequest = ::fidl::AnyZeroArgMessage;

  struct ReadResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t status;

    static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_SkipBlockReadResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  struct ReadRequest final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    ReadWriteOperation op;

    static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_SkipBlockReadRequestTable;
    static constexpr uint32_t MaxNumHandles = 1;
    static constexpr uint32_t PrimarySize = 40;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kRequest;
    using ResponseType = ReadResponse;
  };

  struct WriteResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t status;
    bool bad_block_grown;

    static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_SkipBlockWriteResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  struct WriteRequest final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    ReadWriteOperation op;

    static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_SkipBlockWriteRequestTable;
    static constexpr uint32_t MaxNumHandles = 1;
    static constexpr uint32_t PrimarySize = 40;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kRequest;
    using ResponseType = WriteResponse;
  };

  struct WriteBytesResponse final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    int32_t status;
    bool bad_block_grown;

    static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_SkipBlockWriteBytesResponseTable;
    static constexpr uint32_t MaxNumHandles = 0;
    static constexpr uint32_t PrimarySize = 24;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kResponse;
  };
  struct WriteBytesRequest final {
    FIDL_ALIGNDECL
    fidl_message_header_t _hdr;
    WriteBytesOperation op;

    static constexpr const fidl_type_t* Type = &fuchsia_hardware_skipblock_SkipBlockWriteBytesRequestTable;
    static constexpr uint32_t MaxNumHandles = 1;
    static constexpr uint32_t PrimarySize = 48;
    static constexpr uint32_t MaxOutOfLine = 0;
    static constexpr bool HasFlexibleEnvelope = false;
    static constexpr ::fidl::internal::TransactionalMessageKind MessageKind =
        ::fidl::internal::TransactionalMessageKind::kRequest;
    using ResponseType = WriteBytesResponse;
  };


  // Collection of return types of FIDL calls in this interface.
  class ResultOf final {
    ResultOf() = delete;
   private:
    template <typename ResponseType>
    class GetPartitionInfo_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      GetPartitionInfo_Impl(zx::unowned_channel _client_end);
      ~GetPartitionInfo_Impl() = default;
      GetPartitionInfo_Impl(GetPartitionInfo_Impl&& other) = default;
      GetPartitionInfo_Impl& operator=(GetPartitionInfo_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Read_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      Read_Impl(zx::unowned_channel _client_end, ReadWriteOperation op);
      ~Read_Impl() = default;
      Read_Impl(Read_Impl&& other) = default;
      Read_Impl& operator=(Read_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Write_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      Write_Impl(zx::unowned_channel _client_end, ReadWriteOperation op);
      ~Write_Impl() = default;
      Write_Impl(Write_Impl&& other) = default;
      Write_Impl& operator=(Write_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class WriteBytes_Impl final : private ::fidl::internal::OwnedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::OwnedSyncCallBase<ResponseType>;
     public:
      WriteBytes_Impl(zx::unowned_channel _client_end, WriteBytesOperation op);
      ~WriteBytes_Impl() = default;
      WriteBytes_Impl(WriteBytes_Impl&& other) = default;
      WriteBytes_Impl& operator=(WriteBytes_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };

   public:
    using GetPartitionInfo = GetPartitionInfo_Impl<GetPartitionInfoResponse>;
    using Read = Read_Impl<ReadResponse>;
    using Write = Write_Impl<WriteResponse>;
    using WriteBytes = WriteBytes_Impl<WriteBytesResponse>;
  };

  // Collection of return types of FIDL calls in this interface,
  // when the caller-allocate flavor or in-place call is used.
  class UnownedResultOf final {
    UnownedResultOf() = delete;
   private:
    template <typename ResponseType>
    class GetPartitionInfo_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      GetPartitionInfo_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);
      ~GetPartitionInfo_Impl() = default;
      GetPartitionInfo_Impl(GetPartitionInfo_Impl&& other) = default;
      GetPartitionInfo_Impl& operator=(GetPartitionInfo_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Read_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      Read_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ReadWriteOperation op, ::fidl::BytePart _response_buffer);
      ~Read_Impl() = default;
      Read_Impl(Read_Impl&& other) = default;
      Read_Impl& operator=(Read_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class Write_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      Write_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ReadWriteOperation op, ::fidl::BytePart _response_buffer);
      ~Write_Impl() = default;
      Write_Impl(Write_Impl&& other) = default;
      Write_Impl& operator=(Write_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };
    template <typename ResponseType>
    class WriteBytes_Impl final : private ::fidl::internal::UnownedSyncCallBase<ResponseType> {
      using Super = ::fidl::internal::UnownedSyncCallBase<ResponseType>;
     public:
      WriteBytes_Impl(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, WriteBytesOperation op, ::fidl::BytePart _response_buffer);
      ~WriteBytes_Impl() = default;
      WriteBytes_Impl(WriteBytes_Impl&& other) = default;
      WriteBytes_Impl& operator=(WriteBytes_Impl&& other) = default;
      using Super::status;
      using Super::error;
      using Super::ok;
      using Super::Unwrap;
      using Super::value;
      using Super::operator->;
      using Super::operator*;
    };

   public:
    using GetPartitionInfo = GetPartitionInfo_Impl<GetPartitionInfoResponse>;
    using Read = Read_Impl<ReadResponse>;
    using Write = Write_Impl<WriteResponse>;
    using WriteBytes = WriteBytes_Impl<WriteBytesResponse>;
  };

  class SyncClient final {
   public:
    explicit SyncClient(::zx::channel channel) : channel_(std::move(channel)) {}
    ~SyncClient() = default;
    SyncClient(SyncClient&&) = default;
    SyncClient& operator=(SyncClient&&) = default;

    const ::zx::channel& channel() const { return channel_; }

    ::zx::channel* mutable_channel() { return &channel_; }

    // Returns information about the skip-block partition.
    //
    // The block count can shrink in the event that a bad block is grown. It is
    // recommended to call this again after a bad block is grown.
    // Allocates 72 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::GetPartitionInfo GetPartitionInfo();

    // Returns information about the skip-block partition.
    //
    // The block count can shrink in the event that a bad block is grown. It is
    // recommended to call this again after a bad block is grown.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::GetPartitionInfo GetPartitionInfo(::fidl::BytePart _response_buffer);

    // Reads the specified blocks into the provided vmo.
    // Allocates 64 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::Read Read(ReadWriteOperation op);

    // Reads the specified blocks into the provided vmo.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::Read Read(::fidl::BytePart _request_buffer, ReadWriteOperation op, ::fidl::BytePart _response_buffer);

    // Erases and writes the specified blocks from the provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    // Allocates 64 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::Write Write(ReadWriteOperation op);

    // Erases and writes the specified blocks from the provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::Write Write(::fidl::BytePart _request_buffer, ReadWriteOperation op, ::fidl::BytePart _response_buffer);

    // Erases and writes the specified bytes from the provided vmo. If offset
    // and size in |op| are not aligned to |block_size_bytes| then the driver will
    // first read the partitially written blocks and combine them with the
    // provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    // Allocates 72 bytes of message buffer on the stack. No heap allocation necessary.
    ResultOf::WriteBytes WriteBytes(WriteBytesOperation op);

    // Erases and writes the specified bytes from the provided vmo. If offset
    // and size in |op| are not aligned to |block_size_bytes| then the driver will
    // first read the partitially written blocks and combine them with the
    // provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    UnownedResultOf::WriteBytes WriteBytes(::fidl::BytePart _request_buffer, WriteBytesOperation op, ::fidl::BytePart _response_buffer);

   private:
    ::zx::channel channel_;
  };

  // Methods to make a sync FIDL call directly on an unowned channel, avoiding setting up a client.
  class Call final {
    Call() = delete;
   public:

    // Returns information about the skip-block partition.
    //
    // The block count can shrink in the event that a bad block is grown. It is
    // recommended to call this again after a bad block is grown.
    // Allocates 72 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::GetPartitionInfo GetPartitionInfo(zx::unowned_channel _client_end);

    // Returns information about the skip-block partition.
    //
    // The block count can shrink in the event that a bad block is grown. It is
    // recommended to call this again after a bad block is grown.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::GetPartitionInfo GetPartitionInfo(zx::unowned_channel _client_end, ::fidl::BytePart _response_buffer);

    // Reads the specified blocks into the provided vmo.
    // Allocates 64 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::Read Read(zx::unowned_channel _client_end, ReadWriteOperation op);

    // Reads the specified blocks into the provided vmo.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::Read Read(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ReadWriteOperation op, ::fidl::BytePart _response_buffer);

    // Erases and writes the specified blocks from the provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    // Allocates 64 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::Write Write(zx::unowned_channel _client_end, ReadWriteOperation op);

    // Erases and writes the specified blocks from the provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::Write Write(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ReadWriteOperation op, ::fidl::BytePart _response_buffer);

    // Erases and writes the specified bytes from the provided vmo. If offset
    // and size in |op| are not aligned to |block_size_bytes| then the driver will
    // first read the partitially written blocks and combine them with the
    // provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    // Allocates 72 bytes of message buffer on the stack. No heap allocation necessary.
    static ResultOf::WriteBytes WriteBytes(zx::unowned_channel _client_end, WriteBytesOperation op);

    // Erases and writes the specified bytes from the provided vmo. If offset
    // and size in |op| are not aligned to |block_size_bytes| then the driver will
    // first read the partitially written blocks and combine them with the
    // provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    // Caller provides the backing storage for FIDL message via request and response buffers.
    static UnownedResultOf::WriteBytes WriteBytes(zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, WriteBytesOperation op, ::fidl::BytePart _response_buffer);

  };

  // Messages are encoded and decoded in-place when these methods are used.
  // Additionally, requests must be already laid-out according to the FIDL wire-format.
  class InPlace final {
    InPlace() = delete;
   public:

    // Returns information about the skip-block partition.
    //
    // The block count can shrink in the event that a bad block is grown. It is
    // recommended to call this again after a bad block is grown.
    static ::fidl::DecodeResult<GetPartitionInfoResponse> GetPartitionInfo(zx::unowned_channel _client_end, ::fidl::BytePart response_buffer);

    // Reads the specified blocks into the provided vmo.
    static ::fidl::DecodeResult<ReadResponse> Read(zx::unowned_channel _client_end, ::fidl::DecodedMessage<ReadRequest> params, ::fidl::BytePart response_buffer);

    // Erases and writes the specified blocks from the provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    static ::fidl::DecodeResult<WriteResponse> Write(zx::unowned_channel _client_end, ::fidl::DecodedMessage<WriteRequest> params, ::fidl::BytePart response_buffer);

    // Erases and writes the specified bytes from the provided vmo. If offset
    // and size in |op| are not aligned to |block_size_bytes| then the driver will
    // first read the partitially written blocks and combine them with the
    // provided vmo.
    //
    // In the event that bad block is grown, the partition will shrink and
    // `bad_block_grown` will be set to true. Since this causes the logical to
    // physical block map to change, all previously written blocks at logical
    // addresses after the section being written should be considered corrupted,
    // and rewritten if applicable.
    static ::fidl::DecodeResult<WriteBytesResponse> WriteBytes(zx::unowned_channel _client_end, ::fidl::DecodedMessage<WriteBytesRequest> params, ::fidl::BytePart response_buffer);

  };

  // Pure-virtual interface to be implemented by a server.
  class Interface {
   public:
    Interface() = default;
    virtual ~Interface() = default;
    using _Outer = SkipBlock;
    using _Base = ::fidl::CompleterBase;

    class GetPartitionInfoCompleterBase : public _Base {
     public:
      void Reply(int32_t status, PartitionInfo partition_info);
      void Reply(::fidl::BytePart _buffer, int32_t status, PartitionInfo partition_info);
      void Reply(::fidl::DecodedMessage<GetPartitionInfoResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using GetPartitionInfoCompleter = ::fidl::Completer<GetPartitionInfoCompleterBase>;

    virtual void GetPartitionInfo(GetPartitionInfoCompleter::Sync _completer) = 0;

    class ReadCompleterBase : public _Base {
     public:
      void Reply(int32_t status);
      void Reply(::fidl::BytePart _buffer, int32_t status);
      void Reply(::fidl::DecodedMessage<ReadResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using ReadCompleter = ::fidl::Completer<ReadCompleterBase>;

    virtual void Read(ReadWriteOperation op, ReadCompleter::Sync _completer) = 0;

    class WriteCompleterBase : public _Base {
     public:
      void Reply(int32_t status, bool bad_block_grown);
      void Reply(::fidl::BytePart _buffer, int32_t status, bool bad_block_grown);
      void Reply(::fidl::DecodedMessage<WriteResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using WriteCompleter = ::fidl::Completer<WriteCompleterBase>;

    virtual void Write(ReadWriteOperation op, WriteCompleter::Sync _completer) = 0;

    class WriteBytesCompleterBase : public _Base {
     public:
      void Reply(int32_t status, bool bad_block_grown);
      void Reply(::fidl::BytePart _buffer, int32_t status, bool bad_block_grown);
      void Reply(::fidl::DecodedMessage<WriteBytesResponse> params);

     protected:
      using ::fidl::CompleterBase::CompleterBase;
    };

    using WriteBytesCompleter = ::fidl::Completer<WriteBytesCompleterBase>;

    virtual void WriteBytes(WriteBytesOperation op, WriteBytesCompleter::Sync _completer) = 0;

  };

  // Attempts to dispatch the incoming message to a handler function in the server implementation.
  // If there is no matching handler, it returns false, leaving the message and transaction intact.
  // In all other cases, it consumes the message and returns true.
  // It is possible to chain multiple TryDispatch functions in this manner.
  static bool TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn);

  // Dispatches the incoming message to one of the handlers functions in the interface.
  // If there is no matching handler, it closes all the handles in |msg| and closes the channel with
  // a |ZX_ERR_NOT_SUPPORTED| epitaph, before returning false. The message should then be discarded.
  static bool Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn);

  // Same as |Dispatch|, but takes a |void*| instead of |Interface*|. Only used with |fidl::Bind|
  // to reduce template expansion.
  // Do not call this method manually. Use |Dispatch| instead.
  static bool TypeErasedDispatch(void* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
    return Dispatch(static_cast<Interface*>(impl), msg, txn);
  }

};

constexpr uint32_t GUID_LEN = 16u;

}  // namespace skipblock
}  // namespace hardware
}  // namespace fuchsia
}  // namespace llcpp

namespace fidl {

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::WriteBytesOperation> : public std::true_type {};
static_assert(std::is_standard_layout_v<::llcpp::fuchsia::hardware::skipblock::WriteBytesOperation>);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::WriteBytesOperation, vmo) == 0);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::WriteBytesOperation, vmo_offset) == 8);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::WriteBytesOperation, offset) == 16);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::WriteBytesOperation, size) == 24);
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::WriteBytesOperation) == ::llcpp::fuchsia::hardware::skipblock::WriteBytesOperation::PrimarySize);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::ReadWriteOperation> : public std::true_type {};
static_assert(std::is_standard_layout_v<::llcpp::fuchsia::hardware::skipblock::ReadWriteOperation>);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::ReadWriteOperation, vmo) == 0);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::ReadWriteOperation, vmo_offset) == 8);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::ReadWriteOperation, block) == 16);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::ReadWriteOperation, block_count) == 20);
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::ReadWriteOperation) == ::llcpp::fuchsia::hardware::skipblock::ReadWriteOperation::PrimarySize);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::PartitionInfo> : public std::true_type {};
static_assert(std::is_standard_layout_v<::llcpp::fuchsia::hardware::skipblock::PartitionInfo>);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::PartitionInfo, partition_guid) == 0);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::PartitionInfo, block_size_bytes) == 16);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::PartitionInfo, partition_block_count) == 24);
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::PartitionInfo) == ::llcpp::fuchsia::hardware::skipblock::PartitionInfo::PrimarySize);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::SkipBlock::GetPartitionInfoResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::skipblock::SkipBlock::GetPartitionInfoResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::GetPartitionInfoResponse)
    == ::llcpp::fuchsia::hardware::skipblock::SkipBlock::GetPartitionInfoResponse::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::GetPartitionInfoResponse, status) == 16);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::GetPartitionInfoResponse, partition_info) == 24);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadRequest> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadRequest> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadRequest)
    == ::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadRequest::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadRequest, op) == 16);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadResponse)
    == ::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadResponse::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::ReadResponse, status) == 16);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteRequest> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteRequest> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteRequest)
    == ::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteRequest::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteRequest, op) == 16);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteResponse)
    == ::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteResponse::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteResponse, status) == 16);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteResponse, bad_block_grown) == 20);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesRequest> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesRequest> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesRequest)
    == ::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesRequest::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesRequest, op) == 16);

template <>
struct IsFidlType<::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesResponse> : public std::true_type {};
template <>
struct IsFidlMessage<::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesResponse> : public std::true_type {};
static_assert(sizeof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesResponse)
    == ::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesResponse::PrimarySize);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesResponse, status) == 16);
static_assert(offsetof(::llcpp::fuchsia::hardware::skipblock::SkipBlock::WriteBytesResponse, bad_block_grown) == 20);

}  // namespace fidl
