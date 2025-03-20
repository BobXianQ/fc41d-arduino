#ifndef __QL_HAL_UART_H__
#define __QL_HAL_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_hal.h"

    ql_hal_errcode_e ql_hal_uart_begin(uint8_t port, uint32_t baudrate, uint8_t config);
    ql_hal_errcode_e ql_hal_uart_end(uint8_t port);
    ql_hal_errcode_e ql_hal_uart_available(uint8_t port, int* len);
    ql_hal_errcode_e ql_hal_uart_peek(uint8_t port);
    ql_hal_errcode_e ql_hal_uart_read(uint8_t port, uint8_t* data, uint32_t data_len);
    ql_hal_errcode_e ql_hal_uart_availableForWrite(uint8_t port, uint32_t* len);
    ql_hal_errcode_e ql_hal_uart_flush(uint8_t port);
    ql_hal_errcode_e ql_hal_uart_write(uint8_t port, const uint8_t* data, uint32_t data_len, uint32_t* write_len);

#ifdef __cplusplus
}
#endif

#endif
