#ifndef __QL_HAL_MISC_H__
#define __QL_HAL_MISC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_hal.h"
#include "FreeRTOS.h"
#include "task.h"

extern int bk_rand();
extern void yield(void);

BaseType_t xTaskCreateUniversal( TaskFunction_t pxTaskCode,
                        const char * const pcName,
                        const uint32_t usStackDepth,
                        void * const pvParameters,
                        UBaseType_t uxPriority,
                        TaskHandle_t * const pxCreatedTask,
                        const BaseType_t xCoreID );

#ifdef __cplusplus
}
#endif

#endif
