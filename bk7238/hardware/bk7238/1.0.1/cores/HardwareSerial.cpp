/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#include "HardwareSerial.h"

#include "ql_hal.h"

HardwareSerial Serial0(0);
HardwareSerial Serial1(1);

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(unsigned long baud, byte config) {
    log_i("xbtest!!! %s:%d uart_port=%d\r\n", __func__, __LINE__, uart_port);

    HAL_ASSERT(QL_HAL_SUCCESS == ql_hal_uart_begin(uart_port, (uint32_t)baud, config));
    uart_start = 1;

_exit:
    return;
}

void HardwareSerial::end() {
    HAL_ASSERT(QL_HAL_SUCCESS == ql_hal_uart_end(uart_port));
    uart_start = 0;

_exit:
    return;
}

int HardwareSerial::available(void) {
    if (!uart_start)
        return 0;

    int len = 0;
    ql_hal_uart_available(uart_port, &len);

    return len;
}

int HardwareSerial::peek(void) {
    uint8_t data;
    if (!uart_start)
        return 0;

    if (!available())
        return 0;

    HAL_ASSERT(QL_HAL_SUCCESS == ql_hal_uart_peek(uart_port));
    return 1;

_exit:
    return 0;
}

int HardwareSerial::read(void) {
    uint8_t inbuf;

    if (!uart_start)
        return -1;

    HAL_ASSERT(QL_HAL_SUCCESS == ql_hal_uart_read(uart_port, &inbuf, 1));
    return inbuf;

_exit:
    return -1;
}

int HardwareSerial::availableForWrite(void) {
    uint32_t len = 0;
    HAL_ASSERT(QL_HAL_SUCCESS == ql_hal_uart_availableForWrite(uart_port, &len));
    return len;

_exit:
    return 0;
}

void HardwareSerial::flush() {
    if (!uart_start)
        return;
    ql_hal_uart_flush(uart_port);
}

size_t HardwareSerial::write(uint8_t c) {
    if (!uart_start)
        return 0;

    uint32_t write_len = 0;
    HAL_ASSERT(QL_HAL_SUCCESS == ql_hal_uart_write(uart_port, (uint8*)&c, 1, &write_len));
    return write_len;

_exit:
    return 0;
}

size_t HardwareSerial::write(const uint8_t* buffer, size_t size) {
    if (!uart_start)
        return 0;

    uint32_t write_len = 0;
    HAL_ASSERT(QL_HAL_SUCCESS == ql_hal_uart_write(uart_port, buffer, size, &write_len));
    return write_len;

_exit:
    return 0;
}
