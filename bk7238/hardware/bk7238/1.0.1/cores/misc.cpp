#include "Arduino.h"

unsigned long  micros()
{
    return 0;
}

unsigned long  millis()
{
    return 0;
}

void  delayMicroseconds(uint32_t us)
{
    // uint64_t m = (uint64_t)rtos_get_time_us();
    // if(us){
    //     uint64_t e = (m + us);
    //     if(m > e){ //overflow
    //         while((uint64_t)rtos_get_time_us() > e){
    //            __asm ( "NOP" );
    //         }
    //     }
    //     while((uint64_t)rtos_get_time_us() < e){
    //        __asm ( "NOP" );
    //     }
    // }
}

void delay(uint32_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}
