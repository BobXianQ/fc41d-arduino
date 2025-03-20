#ifndef __QL_HAL_H__
#define __QL_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef enum {
    QL_HAL_SUCCESS = 0,
    QL_HAL_FAILED = -1
}ql_hal_errcode_e;

#include "uart_pub.h"
#include "ql_type.h"
#include "ql_osi.h"
#include "ql_hal_os.h"
#include "ql_hal_log.h"
#include "ql_hal_uart.h"
#include "ql_hal_misc.h"
#include "ql_hal_gpio.h"
#include "ql_hal_adc.h"
#include "ql_hal_pwm.h"

extern void quec_app_main(void* argv);

#ifdef __cplusplus
}
#endif

#endif
