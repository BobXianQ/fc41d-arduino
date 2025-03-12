#ifndef __QL_HAL_LOG_H__
#define __QL_HAL_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_hal.h"

#define LOG_ERROR                           0x00
#define LOG_INFO                            0x01
#define LOG_WARN                            0x02
#define LOG_DEBUG                           0x03

#define QL_HAL_LOG_LEVEL                    (LOG_INFO)

#define QL_HAL_PRINTF(fmt, ...)             os_printf(fmt, ##__VA_ARGS__)

#define QL_HAL_LOG(level, fmt, ...)         \
do{                                         \
    if (level <= QL_HAL_LOG_LEVEL) {        \
        QL_HAL_PRINTF(fmt, ##__VA_ARGS__);  \
    }                                       \
} while (0)

#ifdef __cplusplus
}
#endif
#endif
