/**
  ******************************************************************************
  * @file    adc_if.c
  * @author  MCD Application Team
  * @brief   Read status related to the chip (battery level, VREF, chip temperature)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

// Includes --------------------------------------------------------------------
#include "adc_if.h"
#include "sys_app.h"

// External variables ----------------------------------------------------------
/**
  * @brief ADC handle
  */
extern ADC_HandleTypeDef hadc;

// Private typedef -------------------------------------------------------------
// Private define --------------------------------------------------------------
// Private macro ---------------------------------------------------------------
// Private variables -----------------------------------------------------------
// Prototypes ------------------------------------------------------------------

/**
  * @brief This function reads the ADC channel
  * @param channel channel number to read
  * @return adc measured level value
  */
static uint32_t adc_read_channels( uint32_t u32_channel );

/**
  * @brief This function reads ADC
  * @param channel channel number to read
  * @return value voltage level in linear scale
  */
static uint16_t adc_read( uint32_t u32_channel );

// Exported functions ----------------------------------------------------------
void adc_init_measurement( void )
{
  hadc.Instance = ADC;
}

void adc_deinit_measurement( void )
{
  ;
}

uint16_t adc_get_supply_level( void )
{
  return adc_read( ADC_CHANNEL_VREFINT );
}

uint16_t adc_get_channel_level( uint32_t u32_channel )
{
  return adc_read( u32_channel );
}

// Private Functions Definition ------------------------------------------------
static uint32_t adc_read_channels( uint32_t u32_channel )
{
  uint32_t ADCxConvertedValues = 0;
  ADC_ChannelConfTypeDef sConfig = { 0 };

  MX_ADC_Init();

  /* Start Calibration */
  if(HAL_ADCEx_Calibration_Start( &hadc ) != HAL_OK )
  {
    Error_Handler();
  }

  /* Configure Regular Channel */
  sConfig.Channel = u32_channel;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if( HAL_ADC_ConfigChannel( &hadc, &sConfig ) != HAL_OK )
  {
    Error_Handler();
  }

  if( HAL_ADC_Start( &hadc ) != HAL_OK )
  {
    /* Start Error */
    Error_Handler();
  }
  /** Wait for end of conversion */
  HAL_ADC_PollForConversion( &hadc, HAL_MAX_DELAY );

  /** Wait for end of conversion */
  HAL_ADC_Stop( &hadc ) ;   /* it calls also ADC_Disable() */

  ADCxConvertedValues = HAL_ADC_GetValue( &hadc );

  HAL_ADC_DeInit( &hadc );

  return ADCxConvertedValues;
}

static uint16_t adc_read( uint32_t u32_channel )
{
  uint16_t u16_level_mv = 0;
  uint32_t u32_measured_level = 0;

  u32_measured_level = adc_read_channels( u32_channel );

  if( u32_measured_level == 0 )
  {
    u16_level_mv = 0;
  }
  else
  {
    /* Device with Reference voltage not calibrated in production: use generic parameters */
    if( u32_channel == ADC_CHANNEL_VREFINT )
    {
      if( ( uint32_t )*VREFINT_CAL_ADDR != ( uint32_t )0xFFFFU )
      {
        u16_level_mv = __LL_ADC_CALC_VREFANALOG_VOLTAGE( u32_measured_level, ADC_RESOLUTION_12B );
      }
      else
      {
        u16_level_mv = ( VREFINT_CAL_VREF * 1510 ) / u32_measured_level;
      }
    }
    else
    {
      if( ( uint32_t )*VREFINT_CAL_ADDR != ( uint32_t )0xFFFFU )
      {
        u16_level_mv = __LL_ADC_CALC_DATA_TO_VOLTAGE( adc_get_supply_level(), u32_measured_level, ADC_RESOLUTION_12B );
      }
      else
      {
        u16_level_mv = ( VREFINT_CAL_VREF * u32_measured_level ) / 4095;  // If u32_channel != VREFINT
      }
    }
  }

  return u16_level_mv;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
