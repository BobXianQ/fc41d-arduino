#ifndef _QL_HAL_ADC_H_
#define _QL_HAL_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_hal.h"

int analogRead(uint8_t pin);
void analogReadResolution(int resolution);
#ifdef __cplusplus
}
#endif
#endif
