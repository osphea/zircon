// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RUNTESTS_UTILS_SERVICE_PROXY_DIR_H_
#define RUNTESTS_UTILS_SERVICE_PROXY_DIR_H_

#include <fbl/ref_ptr.h>
#include <fbl/string_piece.h>
#include <fs/service.h>

#include <mutex>
#include <string>
#include <unordered_map>

namespace runtests {

// A directory-like object that proxies connection to the underlying
// directory but allows replacing some entries.
class ServiceProxyDir : public fs::Vnode {
 public:
  explicit ServiceProxyDir(zx::channel proxy_dir);

  void AddEntry(std::string name, fbl::RefPtr<fs::Vnode> node);

  // Overridden from |fs::Vnode|:

  zx_status_t Lookup(fbl::RefPtr<fs::Vnode>* out, fbl::StringPiece name) final;
  zx_status_t Getattr(vnattr_t* a) final;
  zx_status_t GetNodeInfo(uint32_t flags, fuchsia_io_NodeInfo* info) final;
  bool IsDirectory() const final;

 private:
  zx::channel proxy_dir_;
  std::unordered_map<std::string, fbl::RefPtr<fs::Vnode>> entries_;
  std::mutex lock_;
};

}  // namespace runtests

#endif  // RUNTESTS_UTILS_SERVICE_PROXY_DIR_H_
