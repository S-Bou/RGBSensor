/*
This file contains the common defines of the application.
*/

#ifndef __SENSORS_H
#define __SENSORS_H

#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define LONGDATA 400
#define HUARTNUM huart3

extern UART_HandleTypeDef huart3;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void InitESP(void);
void find_str(uint8_t dataRX[]);
void UpPage(void);

#endif /* __SENSORS_H */
