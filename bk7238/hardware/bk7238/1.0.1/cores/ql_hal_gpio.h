#ifndef __QL_HAL_GPIO_H__
#define __QL_HAL_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_hal.h"

#define LOW               0x0
#define HIGH              0x1

#define INPUT             0x01
#define OUTPUT            0x03 
#define PULLUP            0x04
#define INPUT_PULLUP      0x05
#define PULLDOWN          0x08
#define INPUT_PULLDOWN    0x09
#define OPEN_DRAIN        0x10
#define OUTPUT_OPEN_DRAIN 0x13
#define ANALOG            0xC0

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif
