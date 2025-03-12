#include "ql_hal_misc.h"

void __cxa_pure_virtual()
{
    // bk_printf("=======__cxa_pure_virtual=============\r\n");
    // while(1);
}

void yield()
{
    // rtos_delay_milliseconds(2);
    //vPortYield();
}


BaseType_t xTaskCreateUniversal( TaskFunction_t pxTaskCode,
                        const char * const pcName,
                        const uint32_t usStackDepth,
                        void * const pvParameters,
                        UBaseType_t uxPriority,
                        TaskHandle_t * const pxCreatedTask,
                        const BaseType_t xCoreID )
{

    // return xTaskCreate(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask);
    return 0;
}

// void * __dso_handle = 0;
/* stub for __libc_init_array */
// void _fini(void) {}
// void _init(void) {}
