/*
 * freq.c
 *
 *  Created on: May 14, 2025
 *      Author: tharun kanna
 */
#include "freq.h"
#include<stdio.h>
#include<string.h>
#include "main.h"
extern UART_HandleTypeDef huart1;

uint32_t ic_val1 = 0, ic_val2 = 0;
uint32_t diff = 0;
uint8_t is_captured = 0;
float frequency = 0;
char msg[200];



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
        if (is_captured == 0)
        {
        	void PULSE_Init(TIM_HandleTypeDef *htim);
        	float Get_PulseWidth(void);  // returns seconds
        	void HAL_TIM_IC_PulseCallback(TIM_HandleTypeDef *htim);
   ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
            is_captured = 1;
        }
        else
        {
            ic_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);

            if (ic_val2 >= ic_val1)
                diff = ic_val2 - ic_val1;
            else
                diff = (0xFFFF - ic_val1) + ic_val2;

            frequency = 1000000.0f / diff;
            float period = diff / 1000000.0f;


                       float pulse_width = period / 2.0f;

                       float on_time = pulse_width;
                       float off_time = period - pulse_width;

                       snprintf(msg, sizeof(msg), "Freq: %.2f Hz | Time Period: %.6f s | PW: %.6f s  | on-time: %.2f s | off-time: %.2f s \r\n",frequency, period, pulse_width, on_time,off_time);
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
            HAL_Delay(100);
            is_captured = 0;
        }
    }
}



