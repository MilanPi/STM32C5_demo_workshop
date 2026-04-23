/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  Calls target system initialization then loop in main.
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/*Application running status -----------------------------*/
typedef volatile enum
{
  EXEC_STATUS_ERROR     = -1,  /* problem encountered         */
  EXEC_STATUS_UNKNOWN   =  0,  /* default value               */
  EXEC_STATUS_INIT_OK   =  1,  /* app_init ran as expected    */
  EXEC_STATUS_OK        =  2   /* application ran as expected */
} app_status_t;
/* Private define ------------------------------------------------------------*/
#define VREF_ANALOG      3300
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static hal_adc_handle_t *hADC1;

__IO uint8_t adcDataXferCpt                 = 0;

uint16_t     adcRawValues;
uint16_t     adcConvertedValues_mVolt;
/* Private functions prototype -----------------------------------------------*/

/**
  * brief:  The application entry point.
  * retval: none but we specify int to comply with C99 standard
  */
int main(void)
{
  /** System Init: this code placed in targets folder initializes your system.
    * It calls the initialization (and sets the initial configuration) of the peripherals.
    * You can use STM32CubeMX to generate and call this code or not in this project.
    * It also contains the HAL initialization and the initial clock configuration.
    */
  if (mx_system_init() != SYSTEM_OK)
  {
    return (-1);
  }
  else
  {
    /*
      * You can start your application code here
      */
    hADC1 = mx_adc1_gethandle();
    
    if (HAL_ADC_Start(hADC1) != HAL_OK)
    {
        return EXEC_STATUS_ERROR;
    }

    if (HAL_ADC_Calibrate(hADC1) != HAL_OK)
    {
        return EXEC_STATUS_ERROR;
    }

    if (HAL_ADC_REG_StartConv_DMA(hADC1, (uint8_t *)&adcRawValues,
                                  sizeof(uint16_t)) != HAL_OK)
    {
        return EXEC_STATUS_ERROR;
    }
    while (1) 
    {
      if (adcDataXferCpt == 1)
      {
        adcConvertedValues_mVolt= HAL_ADC_CALC_DATA_TO_VOLTAGE(VREF_ANALOG, adcRawValues, HAL_ADC_RESOLUTION_12_BIT);
        adcDataXferCpt = 0;
      }
    }
  }
} /* end main */

/*ADC complete transfer callback*/
void HAL_ADC_REG_DataTransferCpltCallback(hal_adc_handle_t *hadc)
{
  adcDataXferCpt = 1;
}
