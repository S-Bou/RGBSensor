/*
	Init declaring function that start PWM, in this case with timer 3 (chanel 1):
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
*/

#include "servos.h"

void PositionServo(uint8_t angle)
{
	htim3.Instance->CCR1 = 25;
}
