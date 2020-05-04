
//#include "servosandbuttons.h"
#include "tcs34725.h"

/*############################## CONNECTIONS ################################################################*/
/*
	- Servo sensor of colours: -> Wire orange from servo to PA6 TIM3_CH1
	- Servo ramp: --------------> Wire orange from servo to PA7 TIM3_CH2
	- Button Acept: ------------> Wire green from button to PE0 GPIO_Input with Pull-up
	- Button Menu: -------------> Wire white fron button to PE5 GPIO_Input with Pull-up
*/
/*##########################################################################################################*/

bool tempStore = true;
bool tempBall = true;
uint8_t buttoncalibrate = 0;
uint8_t buttonMenuCount = 0;
extern TIM_HandleTypeDef htim3;
extern uint8_t rojo, verde, azul, morado, total;

/*###################### FUNCTIONS FOR SERVOS ###############################################################*/
/*
Steps for config timer:
	- Select timer and chanel, in this case TIM3 Channel 1.
	- Select in channel PWM Generation CH1 ->X (This establishes pin PA6 as output).
	- Need pulses for 20 ms frecuency 50 Hz, to do this configure the below parameters:
          In this case in clock configuration I have in APB1 Timer clocks 72 MHz.
          72 MHz / 50 Hz = 1 440 000 -> Preescaler = 1440-1 and ARR = 1000-1.
          Prescaler (PSC - 16 bits value) = 1440-1 
          Counter Period (ARR)            = 1000-1
	- Remember declaring function that start PWM in main file, in this case: timer 3, chanel 1:
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
*/
/*##########################################################################################################*/
void PositionServoSensor(uint8_t angle)	
{
	htim3.Instance->CCR1 = angle;
}
/*##########################################################################################################*/
void PositionServoRamp(uint8_t angle)	
{
	htim3.Instance->CCR2 = angle;
}
/*##########################################################################################################*/
void ContinuousServo(uint8_t init, uint8_t finish)
{
	for(uint8_t i=init; i<finish; i++)
	{
		htim3.Instance->CCR1 = i;
		HAL_Delay(100);
	}
}
/*########## FUNCTIONS FOR BUTTONS ##############################################################*/
/*	
	This function reduces the posibility for the button do multiple signals.
	In this proyect I establishes the pin PE0 as input (Pull-Up), with the user name "ButtonOne"
*/
void ButtonAceptPressed(void)
{
		if(HAL_GPIO_ReadPin(ButtonAcept_GPIO_Port, ButtonAcept_Pin) && tempBall)
	{
		HAL_Delay(5);
		if(HAL_GPIO_ReadPin(ButtonAcept_GPIO_Port, ButtonAcept_Pin))
		{
//			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
			tempBall = false;
		}
	}
	else if(HAL_GPIO_ReadPin(ButtonAcept_GPIO_Port, ButtonAcept_Pin)==GPIO_PIN_RESET)
	{
		HAL_Delay(5);
		if(HAL_GPIO_ReadPin(ButtonAcept_GPIO_Port, ButtonAcept_Pin)==GPIO_PIN_RESET)
		{
//			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
			//Acction when push the button:
			if(buttoncalibrate == 1){CalibrateColour();}
			ButtonMenuAction();
			tempBall = true;
		}
	}
}
/*##########################################################################################################*/
/*
	This function reduces the posibility for the button do multiple signals.
	In this proyect I establishes the pin PC13 as input (Pull-Up), with the user name "ButtonMenu"
*/
void ButtonMenuPressed(void)
{
		if(HAL_GPIO_ReadPin(ButtonMenu_GPIO_Port, ButtonMenu_Pin) && tempStore)
	{
		HAL_Delay(50);
		if(HAL_GPIO_ReadPin(ButtonMenu_GPIO_Port, ButtonMenu_Pin))
		{
			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
			tempBall = false;
		}
	}
	else if(HAL_GPIO_ReadPin(ButtonMenu_GPIO_Port, ButtonMenu_Pin)==GPIO_PIN_RESET)
	{
		HAL_Delay(50);
		if(HAL_GPIO_ReadPin(ButtonMenu_GPIO_Port, ButtonMenu_Pin)==GPIO_PIN_RESET)
		{
//			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
		  //Acction when push the button:
			ButtonMenu();
			tempBall = true;
		}
	}
}
/*##########################################################################################################*/
/*
	This function reduces the posibility for the button do multiple signals.
	In this proyect I establishes the pin PC13 as input (Pull-Up), with the user name "ButtonTwo"
*/
void ButtonMenuAction(void)
{
	SSD1306_Clear();
	
	if(buttonMenuCount == 1)	//Select in menu sorter
	{
		while(HAL_GPIO_ReadPin(ButtonMenu_GPIO_Port, ButtonMenu_Pin))
		{
				if(HAL_GPIO_ReadPin(LaserSensor_GPIO_Port, LaserSensor_Pin) == 0)
				{
						CicleColor();
				}
			HAL_GPIO_TogglePin(BlueLed_GPIO_Port, BlueLed_Pin);
			HAL_Delay(500);
		}
		buttonMenuCount = 0;
	}
	if(buttonMenuCount == 2)	//Select in menu send to web
	{
		char uartComAT[100];
		SSD1306_Clear();
		SSD1306_GotoXY (10,10);                     
		sprintf(uartComAT, "Red:    %d", rojo);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (10,20);                 
		sprintf(uartComAT, "Green:  %d", verde);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (10,30);               
		sprintf(uartComAT, "Blue:   %d", azul);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (10,40);                
		sprintf(uartComAT, "Purple: %d", morado);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (10,50);  
		total = rojo+verde+azul+morado;
		sprintf(uartComAT, "Total:  %d", total);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_UpdateScreen(); 	
		
//		InitESP();
		
		while(HAL_GPIO_ReadPin(GreenLed_GPIO_Port, GreenLed_Pin) == 0)
		{
			HAL_GPIO_TogglePin(OrangeLed_GPIO_Port, OrangeLed_Pin);
			HAL_Delay(200);
//			if(HAL_GPIO_ReadPin(ButtonTwo_GPIO_Port, ButtonTwo_Pin))
//			{
//				ButtonTwoMenu();
//			}
		}
		UpPage();	
		buttonMenuCount = 0;
	}
	if(buttonMenuCount == 3)	//Select in menu calibration
	{	
		SSD1306_Clear();
		CalibrateColour();
		buttonMenuCount = 0;
	}
	if(buttonMenuCount == 4)	//Select in menu reset counter
	{
		rojo=0; verde=0; azul=0; morado=0; total=0;
		char uartComAT[100];
		SSD1306_Clear();
		SSD1306_GotoXY (10,10);                     
		sprintf(uartComAT, "Red:    %d", rojo);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (10,20);                 
		sprintf(uartComAT, "Green:  %d", verde);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (10,30);               
		sprintf(uartComAT, "Blue:   %d", azul);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (10,40);                
		sprintf(uartComAT, "Purple: %d", morado);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (10,50);  
		total = rojo+verde+azul+morado;
		sprintf(uartComAT, "Total:  %d", total);
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_UpdateScreen(); 
		buttonMenuCount = 0;
	}
}
/*##########################################################################################################*/
/*
	Show menu in screen oled.
	In this proyect I establishes the pin PC13 as input (Pull-Up), with the user name "ButtonTwo"
*/
void ButtonMenu(void)
{
	buttonMenuCount++;
	char uartComAT[100];
	
	if(buttonMenuCount == 1)
	{
		SSD1306_Clear();
		SSD1306_GotoXY (14,2);                    
		sprintf(uartComAT, "Menu:");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_GotoXY (14, 16);                 
		sprintf(uartComAT, "1. Sorter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 28);               
		sprintf(uartComAT, "2. Send to web");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 41);                
		sprintf(uartComAT, "3. Calibration");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 53);                
		sprintf(uartComAT, "4. Reset counter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);
		SSD1306_DrawFilledCircle(5, 19, 4, 1);
		SSD1306_UpdateScreen(); 
	}
	else if(buttonMenuCount == 2)
	{
		SSD1306_Clear();
		SSD1306_GotoXY (14,2);                    
		sprintf(uartComAT, "Menu:");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_GotoXY (14, 16);                 
		sprintf(uartComAT, "1. Sorter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 28);               
		sprintf(uartComAT, "2. Send to web");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 41);                
		sprintf(uartComAT, "3. Calibration");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 53);                
		sprintf(uartComAT, "4. Reset counter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1); 
		SSD1306_DrawFilledCircle(5, 31, 4, 1);
		SSD1306_UpdateScreen(); 
	}
	else if(buttonMenuCount == 3)
	{
		SSD1306_Clear();
		SSD1306_GotoXY (14,2);                    
		sprintf(uartComAT, "Menu:");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_GotoXY (14, 16);                 
		sprintf(uartComAT, "1. Sorter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 28);               
		sprintf(uartComAT, "2. Send to web");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 41);                
		sprintf(uartComAT, "3. Calibration");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 53);                
		sprintf(uartComAT, "4. Reset counter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1); 
		SSD1306_DrawFilledCircle(5, 43, 4, 1);
		SSD1306_UpdateScreen(); 
	}
	else if(buttonMenuCount == 4)
	{
		SSD1306_Clear();
		SSD1306_GotoXY (14,2);                    
		sprintf(uartComAT, "Menu:");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_GotoXY (14, 16);                 
		sprintf(uartComAT, "1. Sorter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 28);               
		sprintf(uartComAT, "2. Send to web");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 41);                
		sprintf(uartComAT, "3. Calibration");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 53);                
		sprintf(uartComAT, "4. Reset counter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1); 
		SSD1306_DrawFilledCircle(5, 56, 4, 1);
		SSD1306_UpdateScreen();
	}
	
	if(buttonMenuCount == 5){buttonMenuCount = 0;}
}
/*########## FUNCTIONS FOR usDELAYS ########################################################################*/
//void usDelay(uint32_t uSec)
//	{													//We need define timer-> ejem: #define usTIM TIM5
//	if(uSec < 2) uSec = 2;
//	usTIM->ARR = uSec - 1;		/*Sets the value in the auto-reload register*/
//	usTIM->EGR = 1;						/*Re-initialiser the timer*/
//	usTIM->SR &= ~1;					//Reset the flag
//	usTIM->CR1 |= 1;					//Enables the counter
//	while((usTIM->SR&0x0001) != 1);
//	usTIM->SR &= ~(0x0001);
//}
/*##########################################################################################################*/
//void Delay_us(uint16_t us)
//{
//	__HAL_TIM_SET_COUNTER(&htim1, 0); //Set the counter value to 0
//	while(__HAL_TIM_GET_COUNTER(&htim1) < us);	//Wait for the counter to reach the us input in the parameter
//}
/*##########################################################################################################*/
void calibrateServos(void)
{
	uint16_t time = 2000;
	
	PositionServoRamp(SRROJO);
	HAL_Delay(time);
	PositionServoRamp(SRVERDE);
	HAL_Delay(time);
	PositionServoRamp(SRAZUL);
	HAL_Delay(time);
	PositionServoRamp(SRMORADO);
	HAL_Delay(time);
	PositionServoRamp(SRINDETERMINADO);
	HAL_Delay(time);

	for(uint8_t i=0; i<4; i++)
	{
	HAL_GPIO_WritePin(LedSensor_GPIO_Port, LedSensor_Pin, GPIO_PIN_SET);
	PositionServoSensor(POSDOS);	//Positions degrees
	HAL_Delay(1000);
		
	Read_cts34725();
  HAL_Delay(1000);
	Show_console();
	
	PositionServoSensor(POSTRES);	//Positions degrees
	HAL_Delay(500);
		
	HAL_GPIO_WritePin(LedSensor_GPIO_Port, LedSensor_Pin, GPIO_PIN_RESET);
	PositionServoSensor(POSUNO);	//Positions degrees
	HAL_Delay(1000);
	}
}
/*##########################################################################################################*/
