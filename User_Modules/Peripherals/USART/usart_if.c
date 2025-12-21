/**
  ******************************************************************************
  * @file    usart_if.c
  * @author  MCD Application Team
  * @brief   Configuration of UART MX driver interface for hyperterminal communication
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
#include "usart_if.h"

/* External variables ---------------------------------------------------------*/
/**
  * @brief UART handle
  */
extern UART_HandleTypeDef huart1;

/**
  * @brief buffer to receive 1 character
  */
uint8_t charRx;

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief Trace driver callbacks handler
  */
const UTIL_ADV_TRACE_Driver_s UTIL_TraceDriver =
{
  vcom_Init,
  vcom_DeInit,
  vcom_ReceiveInit,
  vcom_Trace,
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
  * @brief  TX complete callback
  * @return none
  */
static void ( *TxCpltCallback )( void * );

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

UTIL_ADV_TRACE_Status_t vcom_Init( void ( *cb )( void * ) )
{
  TxCpltCallback = cb;
  MX_USART1_UART_Init();

  return UTIL_ADV_TRACE_OK;
}

UTIL_ADV_TRACE_Status_t vcom_DeInit( void )
{
  /* ##-1- Reset peripherals ################################################## */
  __HAL_RCC_USART1_FORCE_RESET();
  __HAL_RCC_USART1_RELEASE_RESET();

  /* ##-2- MspDeInit ################################################## */
  HAL_UART_MspDeInit( &huart1 );

  return UTIL_ADV_TRACE_OK;
}

UTIL_ADV_TRACE_Status_t vcom_Trace( uint8_t *p_data, uint16_t size )
{
  HAL_UART_Transmit( &huart1, p_data, size, 1000 );
  TxCpltCallback( NULL );
  
  return UTIL_ADV_TRACE_OK;
}

UTIL_ADV_TRACE_Status_t vcom_ReceiveInit( void ( *RxCb )( uint8_t *rxChar, uint16_t size, uint8_t error ) )
{
  // No RX!

  return UTIL_ADV_TRACE_OK;
}

void vcom_Resume( void )
{
  /*to re-enable lost UART settings*/
  if( HAL_UART_Init( &huart1 ) != HAL_OK )
  {
    Error_Handler();
  }
}

/* Private Functions Definition -----------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
