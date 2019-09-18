// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ZIRCON_SYSTEM_UAPP_IHDA_INTEL_HDA_CONTROLLER_H_
#define ZIRCON_SYSTEM_UAPP_IHDA_INTEL_HDA_CONTROLLER_H_

#include <fbl/intrusive_wavl_tree.h>
#include <fbl/unique_ptr.h>

#include "intel_hda_codec.h"
#include "intel_hda_device.h"

namespace audio {
namespace intel_hda {

class IntelHDAController : public IntelHDADevice,
                           public fbl::WAVLTreeContainable<fbl::unique_ptr<IntelHDAController>> {
 public:
  using ControllerTree = fbl::WAVLTree<uint32_t, fbl::unique_ptr<IntelHDAController>>;

  zx_status_t DumpRegs(int argc, const char** argv);

  uint32_t id() const { return id_; }
  uint32_t GetKey() const { return id(); }

  static zx_status_t Enumerate();
  static ControllerTree& controllers() { return controllers_; }

 private:
  friend class std::default_delete<IntelHDAController>;

  IntelHDAController(uint32_t id, const char* const dev_name)
      : IntelHDADevice(dev_name, Type::Controller), id_(id) {}

  ~IntelHDAController() {}

  const uint32_t id_;
  static ControllerTree controllers_;
};

}  // namespace intel_hda
}  // namespace audio

#endif  // ZIRCON_SYSTEM_UAPP_IHDA_INTEL_HDA_CONTROLLER_H_
