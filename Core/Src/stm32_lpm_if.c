/**
  ******************************************************************************
  * @file    stm32_lpm_if.c
  * @author  MCD Application Team
  * @brief   Low layer function to enter/exit low power modes (stop, sleep)
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpm.h"
#include "stm32_lpm_if.h"
#include "usart_if.h"
#include "sys_conf.h"

#include "user_conf.h"
#include "app.h"

/* External variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/**
  * @brief Power driver callbacks handler
  */
const struct UTIL_LPM_Driver_s UTIL_PowerDriver =
{
  PWR_EnterSleepMode,
  PWR_ExitSleepMode,

  PWR_EnterStopMode,
  PWR_ExitStopMode,

  PWR_EnterOffMode,
  PWR_ExitOffMode,
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
lpm_callbacks_t *lpm_cbs = NULL;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void lpm_init_cb( lpm_callbacks_t *lpm_callbacks )
{
  if( lpm_callbacks != NULL )
  {
    lpm_cbs = lpm_callbacks;
  }
}

void PWR_EnterOffMode(void)
{
}

void PWR_ExitOffMode(void)
{
}

void PWR_EnterStopMode(void)
{
  /* Suspend sysTick : work around for degugger problem in dual core (tickets 71085,  72038, 71087 ) */
  HAL_SuspendTick();
  /* Clear Status Flag before entering STOP/STANDBY Mode */
  LL_PWR_ClearFlag_C1STOP_C1STB();

  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
//  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
}

void PWR_ExitStopMode(void)
{
  SystemClock_Config();

  /* Resume sysTick : work around for degugger problem in dual core */
  HAL_ResumeTick();
  /*Not retained periph:
    ADC interface
    DAC interface USARTx, TIMx, i2Cx, SPIx
    SRAM ctrls, DMAx, DMAMux, AES, RNG, HSEM  */

  /* Resume not retained USARTx and DMA */
#if defined ( APP_LOG_ENABLED ) && ( APP_LOG_ENABLED == 1 )
  vcom_Resume();
#endif

  if( lpm_cbs != NULL )
  {
    lpm_cbs->lpm_exit_stop_mode();
  }
}

void PWR_EnterSleepMode(void)
{
  /* Suspend sysTick */
  HAL_SuspendTick();

  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void PWR_ExitSleepMode(void)
{
  /* Suspend sysTick */
  HAL_ResumeTick();
}

/* Private Functions Definition -----------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
