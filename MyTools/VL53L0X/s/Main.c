/* ========================================
PSoC4 ToF Sensor VL53L0X
By Vlad Radoiu @  https://vladradoiu.wordpress.com/
*/

#include "project.h"
#include <stdio.h>

#include <VL53L0X.h>

//Use in timed(continuous) or single mode
#define CONTINUOUS

uint16 dist = 0;


double K_s = 1.0;

double T_s = 8.0;

double dist_f_old = 0.0;

double dist_f_new = 0.0;

uint16 dist_f_new_ = 0;

// Timer1ms
CY_ISR( timer_isr )
{
   	TIMER_STATUS;     // Clear timer status
    timeout_start_ms++;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    TIMER_Init();    
    isr_StartEx( timer_isr );
    TIMER_Start(); 
    LCD_Start();
    LCD_ClearDisplay();
    
    I2C_Start();
    UART_Start();
    UART_PutString("Hello\n");
    
    VL53L0X_init(1); //initialize in 2.8V mode, default 500ms timeout
    
    #ifdef CONTINUOUS
    VL53L0X_startContinuous(100); // timed continuous mode, if 0 scan as fast as posible
    #endif
    
    CyDelay(50); // wait for the device to start
    
    for(;;)
    {
        #ifdef CONTINUOUS
        dist = VL53L0X_readRangeContinuousMillimeters();
        #else
        dist = VL53L0X_readRangeSingleMillimeters();
        #endif
        
        dist_f_new  = dist_f_old + (K_s/T_s)*((double)dist-dist_f_old);//lowpass filter
        
        dist_f_old  = dist_f_new;
     
        dist_f_new_ = (uint16)dist_f_new-4;
        
        // serial print distance
        char buf[40];
        sprintf(buf, "Distance = %d  %d \n", dist, dist_f_new_); 
        LCD_Position(0u,0u);
        
        LCD_PrintString("Dist1 ");
        LCD_PrintNumber(dist_f_new_);
        LCD_Position(1u,0u);
        LCD_PrintString("Dist2 ");
     
        LCD_PrintNumber(dist);
        UART_PutString(buf);
        CyDelay(100);          
    }
}

/* [] END OF FILE */
