
#ifndef __SERVOS_H
#define __SERVOS_H

#include "main.h"
#include <stdint.h>

extern TIM_HandleTypeDef htim3;

void PositionServo(uint8_t angle);

#endif /* __SERVOS_H */
