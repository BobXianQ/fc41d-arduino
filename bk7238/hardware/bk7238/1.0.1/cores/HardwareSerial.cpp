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

#include "ql_uart.h"

HardwareSerial Serial(0);
HardwareSerial Serial1(1);

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(unsigned long baud, byte config) {
    ql_uart_config_s uart_cfg;

    uart_cfg.baudrate = QL_UART_BAUD_115200;
    uart_cfg.data_bit = QL_UART_DATABIT_8;
    uart_cfg.parity_bit = QL_UART_PARITY_NONE;
    uart_cfg.stop_bit = QL_UART_STOP_1;
    uart_cfg.flow_ctrl = QL_FC_NONE;

    HAL_ASSERT(QL_UART_SUCCESS == ql_uart_set_dcbconfig(uart_port, &uart_cfg));
    HAL_ASSERT(QL_UART_SUCCESS == ql_uart_open(uart_port));

    uart_start = 1;

_exit:
    uart_start = 0;
}

void HardwareSerial::end() {
    uart_start = 0;
}

int HardwareSerial::available(void) {
    if (!uart_start)
        return 0;

    int len = 0;
    len = ql_uart_get_rx_count(uart_port);
    return len;
}

int HardwareSerial::peek(void) {
    uint8_t data;
    if (!uart_start)
        return 0;

    if (!available())
        return 0;

    HAL_ASSERT(QL_UART_SUCCESS == ql_uart_read_prefetch(uart_port, &data, 1));
    return 1;

_exit:
    return 0;
}

int HardwareSerial::read(void) {
    uint8_t inbuf;

    if (!uart_start)
        return -1;

        ql_uart_read(ql_uart_port_number_e port,uint8 *data, uint32 data_len)
    return -1;
}

int HardwareSerial::availableForWrite(void) {
    return MAX_UART_WRITE_SZIE;
}

void HardwareSerial::flush() {
    if (!uart_start)
        return;

    // uart_fifo_flush((bk_uart_t)uart_port);
}


size_t HardwareSerial::write(uint8_t c) {
    if (!uart_start)
        return 0;

    // bk_send_byte(uart_port,c);
    return 1;
}


size_t HardwareSerial::write(const   uint8_t* buffer, size_t size) {
    if (!uart_start)
        return 0;

    // bk_send_string(uart_port, (char *)buffer);
    return size;
}
