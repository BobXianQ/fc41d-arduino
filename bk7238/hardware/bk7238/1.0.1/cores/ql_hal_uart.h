#ifndef __QL_HAL_UART_H__
#define __QL_HAL_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_hal.h"

int ql_hal_uart_set_baud(uint8_t uart, int baud);

#define log_d(format, ...)  do {} while(0)
#define log_i(format, ...) do {} while(0)
#define log_w(format, ...) do {} while(0)
#define log_e(format, ...) do {} while(0)
#define log_n(format, ...) do {} while(0)
#define log_v(format, ...) do {} while(0)

#ifdef __cplusplus
}
#endif

#endif
