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

#define log_d(format, ...) QL_HAL_LOG(LOG_DEBUG, format, ##__VA_ARGS__)
#define log_i(format, ...) QL_HAL_LOG(LOG_INFO,  format, ##__VA_ARGS__)
#define log_w(format, ...) QL_HAL_LOG(LOG_WARN,  format, ##__VA_ARGS__)
#define log_e(format, ...) QL_HAL_LOG(LOG_ERROR, format, ##__VA_ARGS__)
#define log_n(format, ...) do {} while(0)
#define log_v(format, ...) do {} while(0)

#define HAL_ASSERT(cond)                                                            \
do{                                                                                 \
    if (!(cond)) {                                                                  \
        QL_HAL_LOG(LOG_ERROR, "ERROR %s:%d !("#cond")\r\n", __func__, __LINE__);     \
        goto _exit;                                                                 \
    }                                                                               \
} while (0)

#ifdef __cplusplus
}
#endif
#endif
