// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdint.h>

#include "debug.h"

#define S905_UART_WFIFO (0x0)
#define S905_UART_STATUS (0xc)
#define S905_UART_STATUS_TXFULL (1 << 21)

#define UARTREG(reg) (*(volatile uint32_t*)(0xff803000 + (reg)))

void uart_pputc(char c) {
  while (UARTREG(S905_UART_STATUS) & S905_UART_STATUS_TXFULL)
    ;
  UARTREG(S905_UART_WFIFO) = c;
}
