// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "util.h"

#include "debug.h"

void fail(const char* message) {
  uart_puts(message);
  while (1) {
  }
}
