/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

	#include <string.h>
	#include <stdio.h>
	#include "servos.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RED 0
#define BLUE 1
#define CLEAR 2
#define GREEN 3
#define TIME 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

	uint32_t IC_Val1 = 0;
	uint32_t IC_Val2 = 0;
	uint8_t red = 0;
	uint8_t blue = 0;
	uint8_t clear = 0;
	uint8_t green = 0;
	uint32_t color = 0;
	uint32_t Difference = 0;
	uint8_t Is_First_Capture = 0;	// Is the first captured?
	char uartComAT[100];
	
	void StoreColor(void);
	
	void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)	// If the interrupt source is channel 2
		{
			if(Is_First_Capture==0)	// If the first value is not captured
			{
				IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);	// Read the first value
				Is_First_Capture = 1;	// Set the first captured as true
				// Now changue the polarity to falling edge
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
			
			else if(Is_First_Capture==1)	// If the first is already captured
			{
				IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);	// Read the second value
				__HAL_TIM_SET_COUNTER(htim, 0);	// Reset the counter
				
				if(IC_Val2 > IC_Val1)
				{
					Difference = IC_Val2-IC_Val1;
				}
				
				Is_First_Capture = 0;	// Set it back to false
				// Set polarity to rising edge
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
				
			}
		}
	}
	
	void RegisterColor(int color)
	{
		switch (color)
    {
    	case RED:
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);
				break;
			
    	case BLUE:
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_SET);
    		break;
			
			case CLEAR:		
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);
    		break;
			
    	case GREEN:
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_SET);
    		break;
			
    	default:
    		break;
    }
		
	}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	
	void CalculeColor(void);

	// Start the inpuy-capture in interrupt mode
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);

	//Start the PWM
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
//		StoreColor();
//		CalculeColor();
		
		
		PositionServo(25);
		HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
		HAL_Delay(2000);

		htim3.Instance->CCR1 = 75;
		HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_SET);
		HAL_Delay(2000);
		
		htim3.Instance->CCR1 = 125;
		HAL_GPIO_WritePin(BlueLed_GPIO_Port, BlueLed_Pin, GPIO_PIN_SET);
		HAL_Delay(2000);
		
		HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(BlueLed_GPIO_Port, BlueLed_Pin, GPIO_PIN_RESET);
		
		HAL_Delay(2000);
		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFF-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1440-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GreenLed_Pin|OrangeLed_Pin|RedLed_Pin|BlueLed_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, S3_Pin|S2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ButtonUser_Pin */
  GPIO_InitStruct.Pin = ButtonUser_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ButtonUser_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GreenLed_Pin OrangeLed_Pin RedLed_Pin BlueLed_Pin */
  GPIO_InitStruct.Pin = GreenLed_Pin|OrangeLed_Pin|RedLed_Pin|BlueLed_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : S3_Pin S2_Pin */
  GPIO_InitStruct.Pin = S3_Pin|S2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

		void StoreColor(void)
		{
				HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
			
				RegisterColor(RED);
				sprintf(uartComAT, "Red: %d   ", Difference);
				red = Difference;
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
	
				RegisterColor(GREEN);
				sprintf(uartComAT, "Green: %d   ", Difference);
				green = Difference;
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
		
				RegisterColor(BLUE);
				sprintf(uartComAT, "Blue: %d   ", Difference);
				blue = Difference;
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
		
				RegisterColor(CLEAR);
				sprintf(uartComAT, "Clear: %d  ", Difference);
				clear = Difference;
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
			
				color = red*65536 + green*256 + blue;
				sprintf(uartComAT, "Color: %d", color);
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
				
				sprintf(uartComAT, "\r\n");
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
				
				HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
		}
		
void CalculeColor(void)
{	
	double r,g,b; // r,v,b € [0..1]
			
	double offset = 3.0/clear; // compensacion de luminosidad
	
	if((offset+(clear/red)) < 1)
	{
		r = offset+(clear/red);
	}else{r = 1.0;}

	if((offset+(clear/green)) < 1)
	{
		g = offset+(clear/green);
	}else{g = 1.0;}
	
	if((offset+(clear/blue)) < 1)
	{
		b = offset+(clear/blue);
	}else{b = 1.0;}

	// transformacion RVB -> TSL
	// r,v,b € [0..1]
	// t € [0°..360°]; s,l € [%]
	double t,s,l;
	double maxRGB, minRGB;

		if(r >= b){maxRGB = r;
		}else{maxRGB = b;}
		if(g >= maxRGB){maxRGB = g;}
		
		if(r <= b){minRGB = r;
		}else{minRGB = b;}
		if(g <= minRGB){minRGB = g;}
	
	double delta = maxRGB-minRGB;
	double somme = maxRGB+minRGB;

 // luminancia basada en valores de colores segun software de diseño grafico
 l=(somme/2.0);

 if(delta==0.0) // gris
 {
 t=s=0.0;
 }else{
 //saturacion
 if ( l < 0.5 ) s = delta / somme;
 else s = delta / ( 2.0 - delta );

 // tinte
 double del_R = ( ( ( maxRGB - r ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
 double del_G = ( ( ( maxRGB - g ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
 double del_B = ( ( ( maxRGB - b ) / 6.0 ) + ( delta / 2.0 ) ) / delta;

 if ( r == maxRGB ) t = del_B - del_G;
 else if ( g == maxRGB ) t = ( 1.0 / 3.0 ) + del_R - del_B;
 else if ( b == maxRGB ) t = ( 2.0 / 3.0 ) + del_G - del_R;

 if ( t < 0 ) t += 1.0;
 if ( t > 1 ) t -= 1.0;
 }

 // normalizacion
t*=360.0; // [0°..360°]
s*=100.0; // [%]
l*=100.0; // [%]
 
// 				sprintf(uartComAT, "Color: %f", t);
//				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
//				HAL_Delay(TIME);
// 
// 				sprintf(uartComAT, "\r\n");
//				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
//				HAL_Delay(TIME);
			
		}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
