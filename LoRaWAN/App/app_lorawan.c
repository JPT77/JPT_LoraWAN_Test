/**
  ******************************************************************************
  * @file    app_lorawan.c
  * @author  MCD Application Team
  * @brief   Application of the LRWAN Middleware
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
#include "app_lorawan.h"
#include "app.h"
#include "sys_app.h"
#include "stm32_seq.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void MX_LoRaWAN_Init( void )
{
  SystemApp_Init();

  app_init();
}

void MX_LoRaWAN_Process( void )
{
  UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );
}

/* Private Functions Definition ----------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
