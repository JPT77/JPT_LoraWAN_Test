/**
  ******************************************************************************
  * @file    usart_if.h
  * @author  MCD Application Team
  * @brief   Header for USART interface configuration
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
#include "stm32_adv_trace.h"
#include "usart.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_IF_H__
#define __USART_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Init the UART and associated DMA.
  * @param  cb TxCpltCallback
  * @return @ref UTIL_ADV_TRACE_Status_t
  */
UTIL_ADV_TRACE_Status_t vcom_Init(void (*cb)(void *));

/**
  * @brief  init receiver of vcom
  * @param  RxCb callback when Rx char is received
  * @return @ref UTIL_ADV_TRACE_Status_t
  */
UTIL_ADV_TRACE_Status_t vcom_ReceiveInit(void (*RxCb)(uint8_t *rxChar, uint16_t size, uint8_t error));

/**
  * @brief  DeInit the UART and associated DMA.
  * @return @ref UTIL_ADV_TRACE_Status_t
  */
UTIL_ADV_TRACE_Status_t vcom_DeInit(void);

/**
  * @brief  send buffer \p p_data of size \p size to vcom in polling mode
  * @param  p_data data to be sent
  * @param  size of buffer p_data to be sent
  */
UTIL_ADV_TRACE_Status_t vcom_Trace(uint8_t *p_data, uint16_t size);

/**
  * @brief  Resume the UART and associated DMA (used by LPM)
  */
void vcom_Resume(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __USART_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
