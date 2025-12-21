/**
  ******************************************************************************
  * @file    stm32_lpm_if.h
  * @author  MCD Application Team
  * @brief   Header for Low Power Manager interface configuration
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
#ifndef __STM32_LPM_IF_H__
#define __STM32_LPM_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_lpm.h"

/* Exported types ------------------------------------------------------------*/
  typedef struct lpm_callbacks_s
{
  void ( *lpm_exit_stop_mode )( void );
} lpm_callbacks_t;
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void lpm_init_cb( lpm_callbacks_t *lpm_callbacks );

/**
  * @brief Enters Low Power Off Mode
  */
void PWR_EnterOffMode(void);

/**
  * @brief Exits Low Power Off Mode
  */
void PWR_ExitOffMode(void);

/**
  * @brief Enters Low Power Stop Mode
  * @note ARM exists the function when waking up
  */
void PWR_EnterStopMode(void);

/**
  * @brief Exits Low Power Stop Mode
  * @note Enable the pll at 32MHz
  */
void PWR_ExitStopMode(void);

/**
  * @brief Enters Low Power Sleep Mode
  * @note ARM exits the function when waking up
  */
void PWR_EnterSleepMode(void);

/**
  * @brief Exits Low Power Sleep Mode
  * @note ARM exits the function when waking up
  */
void PWR_ExitSleepMode(void);

#ifdef __cplusplus
}
#endif

#endif /*__STM32_LPM_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
