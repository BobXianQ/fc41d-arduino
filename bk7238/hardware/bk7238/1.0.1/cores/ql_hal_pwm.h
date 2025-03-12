#ifndef _QL_HAL_PWM_H_
#define _QL_HAL_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_hal.h"

void analogWrite(uint8_t pin, int value);
void analogWriteResolution(uint8_t bits);

#ifdef __cplusplus
}
#endif
#endif
