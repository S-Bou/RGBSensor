/*############################## CONNECTIONS ################################################################*/
/*

	- ESP8266 for wifi -> USART1:
		RX   -> Wire orange from wifi to PD8 USART2_TX
		TX   -> Wire brown  from wifi to PD9 USART2_RX
		VCC  -> 3.3V
		CHPD -> 3.3V
		GND  -> GND
*/
/*##############################################################################################*/
#include "sendwifi.h"
/*##############################################################################################*/
float distance = 66.66;

uint32_t numTicks;
uint8_t chanel=66;
uint8_t rxData[LONGDATA];
extern uint8_t	rojo, verde, azul, morado, total;

/*##############################################################################################*/
//When is completed a receive will enter here
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)
	{
    find_str(rxData);
		//HAL_UART_Transmit_DMA(&huart2, (uint8_t *)rxData, LONGDATA);	
		HAL_UART_Receive_DMA(&HUARTNUM, (uint8_t *)rxData, LONGDATA);
		
		HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
	}
}
/*##############################################################################################*/
void InitESP(void)
{
	char uartComAT[100];
	
	HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_SET);
	
	sprintf(uartComAT, "AT+CIPMUX=1\r\n");
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(1000);
	
	sprintf(uartComAT, "AT+CIPSERVER=1,80\r\n");
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(1000);
	
	HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_RESET);
}

/*##############################################################################################*/
void find_str(uint8_t dataRX[]) 
{ 
	HAL_GPIO_WritePin(OrangeLed_GPIO_Port, OrangeLed_Pin, GPIO_PIN_SET);
	int i=0;
	for(i=0; i<400; i++)
	{		
		if(dataRX[i]=='+' && dataRX[i+1]=='I' && dataRX[i+2]=='P' && dataRX[i+3]=='D')
		{
			chanel = dataRX[i+5];
		}
  }	
	HAL_GPIO_WritePin(OrangeLed_GPIO_Port, OrangeLed_Pin, GPIO_PIN_RESET);
}
/*##############################################################################################*/
void UpPage(void)											
{									
	char page[700];
	char uartComAT[700];
	
	//"<head><meta http-equiv=\"refresh\" content=\"15\">" //For reset page each 15 sec
	//<body style="background-color:red;"></body>		 //For change background color 
	
  sprintf(page, 
	"<!doctype html>"
	"<html>"
	"<head>"
			"<meta http-equiv=\"refresh\" content=\";URL=refrescar.html\">"
	"</head>"
	"<body><h2 align=\"center\" style=\"background-color:silver;>"
			"<p style=\"color:Black\";><font size = 7>Clasificación por colores:</font></p>"
			"<hr>"
			"<p style=\"color:Red\";>Rojo: %d</p>"
			"<p style=\"color:Green\";>Verde: %d</p>"
			"<p style=\"color:Blue\";>Azul: %d</p>"
			"<p style=\"color:Purple\";>Morado: %d</p>"
			"<hr>"
			"<p style=\"color:Black\";>Total: %d</p>"
				"<form method=\"get\"action=\"http://192.168.1.16/\">"
					"<button type=\"submit\" style=\"height: 50px; width: 150px;\">"
						"<b>Reiniciar página</b>"
					"</button>"
				"</form>"
	"<h2></body></html>", rojo, verde, azul, morado, total);             	
	
	sprintf(uartComAT, "AT+CIPSEND=%d,%d\r\n", chanel-48, strlen(page));
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(500);
	sprintf(uartComAT, "%s\r\n", page);
	HAL_Delay(500);
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(2000);
	sprintf(uartComAT, "AT+CIPCLOSE=%d\r\n", chanel-48);
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	
	HAL_GPIO_WritePin(OrangeLed_GPIO_Port, OrangeLed_Pin, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(OrangeLed_GPIO_Port, OrangeLed_Pin, GPIO_PIN_RESET);
}
/*##############################################################################################*/



