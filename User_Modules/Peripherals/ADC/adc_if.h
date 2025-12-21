/**
  ******************************************************************************
  * @file    adc_if.h
  * @author  MCD Application Team
  * @brief   Header for ADC interface configuration
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_IF_H__
#define __ADC_IF_H__

#ifdef __cplusplus
extern "C" {
#endif
  
// Includes --------------------------------------------------------------------
#include "adc.h"

// Exported types --------------------------------------------------------------
// Exported constants ----------------------------------------------------------
// External variables ----------------------------------------------------------
// Exported macro --------------------------------------------------------------
// Exported functions prototypes -----------------------------------------------

/**
  * @brief  Initializes the ADC input
  */
void adc_init_measurement( void );

/**
  * @brief DeInitializes the ADC
  */
void adc_deinit_measurement( void );

/**
  * @brief Get the current supply voltage
  * @return value supply voltage in linear scale
  */
uint16_t adc_get_supply_level( void );

/**
  * @brief Get the current channel level
  * @return value channel level in linear scale
  */
uint16_t adc_get_channel_level( uint32_t u32_channel );

#ifdef __cplusplus
}
#endif

#endif /* __ADC_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

