#include "ql_hal.h"
#include "ql_uart.h"

#define MAX_UART_WRITE_SZIE     512

ql_hal_errcode_e ql_hal_uart_begin(uint8_t port, uint32_t baudrate, uint8_t config) {
    ql_uart_config_s uart_cfg;

    uart_cfg.baudrate = QL_UART_BAUD_115200;
    uart_cfg.data_bit = QL_UART_DATABIT_8;
    uart_cfg.parity_bit = QL_UART_PARITY_NONE;
    uart_cfg.stop_bit = QL_UART_STOP_1;
    uart_cfg.flow_ctrl = QL_FC_NONE;
    HAL_ASSERT(QL_UART_SUCCESS == ql_uart_set_dcbconfig((ql_uart_port_number_e)port, &uart_cfg));
    HAL_ASSERT(QL_UART_SUCCESS == ql_uart_open((ql_uart_port_number_e)port));
    return QL_HAL_SUCCESS;

_exit:
    return QL_HAL_FAILED;
}

ql_hal_errcode_e ql_hal_uart_end(uint8_t port) {
    return QL_HAL_SUCCESS;
}

ql_hal_errcode_e ql_hal_uart_available(uint8_t port, int* len) {
    HAL_ASSERT(NULL != len);
    *len = 0;
    *len = ql_uart_get_rx_count((ql_uart_port_number_e)port);
    return QL_HAL_SUCCESS;

_exit:
    return QL_HAL_FAILED;
}

ql_hal_errcode_e ql_hal_uart_peek(uint8_t port) {
    uint8_t data;
    HAL_ASSERT(QL_UART_SUCCESS == ql_uart_read_prefetch((ql_uart_port_number_e)port, &data, 1));
    return QL_HAL_SUCCESS;

_exit:
    return QL_HAL_FAILED;
}

ql_hal_errcode_e ql_hal_uart_read(uint8_t port, uint8_t* data, uint32_t data_len) {
    HAL_ASSERT(NULL != data);
    HAL_ASSERT(QL_UART_SUCCESS == ql_uart_read((ql_uart_port_number_e)port, data, data_len));
    return QL_HAL_SUCCESS;

_exit:
    return QL_HAL_FAILED;
}

ql_hal_errcode_e ql_hal_uart_availableForWrite(uint8_t port, uint32_t* len) {
    *len = MAX_UART_WRITE_SZIE;
    return QL_HAL_SUCCESS;
}

ql_hal_errcode_e ql_hal_uart_flush(uint8_t port) {
    return QL_HAL_SUCCESS;
}

ql_hal_errcode_e ql_hal_uart_write(uint8_t port, const uint8_t* data, uint32_t data_len, uint32_t* write_len) {
    HAL_ASSERT(NULL != write_len);
    *write_len = 0;
    HAL_ASSERT(QL_UART_SUCCESS == ql_uart_write((ql_uart_port_number_e)port, (uint8*)data, data_len));
    *write_len = data_len;
    return QL_HAL_SUCCESS;

_exit:
    return QL_HAL_FAILED;
}
