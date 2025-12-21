/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Bleeper board GPIO driver implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
/**
  ******************************************************************************
  * @file    hw_gpio.c
  * @author  MCD Application Team
  * @brief   driver for GPIO
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes -------------------------------------------------------------------*/
#include "hw_gpio.h"

/* Private typedef ------------------------------------------------------------*/
/* Private define -------------------------------------------------------------*/

/* Private macro --------------------------------------------------------------*/
#define RCC_GPIO_CLK_ENABLE( __GPIO_PORT__ )                    \
do {                                                            \
    switch( __GPIO_PORT__ )                                     \
    {                                                           \
      case GPIOA_BASE: __HAL_RCC_GPIOA_CLK_ENABLE(); break;     \
      case GPIOB_BASE: __HAL_RCC_GPIOB_CLK_ENABLE(); break;     \
      case GPIOC_BASE: __HAL_RCC_GPIOC_CLK_ENABLE(); break;     \
      case GPIOH_BASE: default:  __HAL_RCC_GPIOH_CLK_ENABLE();  \
    }                                                           \
  } while( 0 )

#define RCC_GPIO_CLK_DISABLE( __GPIO_PORT__ )                   \
do {                                                            \
    switch( __GPIO_PORT__ )                                     \
    {                                                           \
      case GPIOA_BASE: __HAL_RCC_GPIOA_CLK_DISABLE(); break;    \
      case GPIOB_BASE: __HAL_RCC_GPIOB_CLK_DISABLE(); break;    \
      case GPIOC_BASE: __HAL_RCC_GPIOC_CLK_DISABLE(); break;    \
      case GPIOH_BASE: default:  __HAL_RCC_GPIOH_CLK_DISABLE(); \
    }                                                           \
  } while( 0 )

/* Private variables ----------------------------------------------------------*/
static GpioIrqHandler *GpioIrq[16] = { NULL };

/* Private function prototypes ------------------------------------------------*/
static uint8_t HW_GPIO_GetBitPos( uint16_t GPIO_Pin );

/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  Gets IRQ number as a function of the GPIO_Pin.
  * @param  GPIO_Pin: Specifies the pins connected to the EXTI line.
  * @retval IRQ number
  */
IRQn_Type MSP_GetIRQn( uint16_t GPIO_Pin )
{
  switch ( GPIO_Pin )
  {
    case GPIO_PIN_0:
      return EXTI0_IRQn;
    case GPIO_PIN_1:
      return EXTI1_IRQn;
    case GPIO_PIN_2:
      return EXTI2_IRQn;
    case GPIO_PIN_3:
      return EXTI3_IRQn;
    case GPIO_PIN_4:
      return EXTI4_IRQn;
    case GPIO_PIN_5:
    case GPIO_PIN_6:
    case GPIO_PIN_7:
    case GPIO_PIN_8:
    case GPIO_PIN_9:
      return EXTI9_5_IRQn;
    case GPIO_PIN_10:
    case GPIO_PIN_11:
    case GPIO_PIN_12:
    case GPIO_PIN_13:
    case GPIO_PIN_14:
    case GPIO_PIN_15:
    default:
      return EXTI15_10_IRQn;
  }
}

/*!
 * @brief Initializes the given GPIO object
 *
 * @param  GPIOx: where x can be (A..C and H)
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @param [IN] initStruct  GPIO_InitTypeDef intit structure
 * @retval none
 */
void HW_GPIO_Init( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_InitTypeDef *initStruct )
{
  RCC_GPIO_CLK_ENABLE( ( uint32_t ) GPIOx );

  initStruct->Pin = GPIO_Pin;

  HAL_GPIO_Init( GPIOx, initStruct );
}

/*!
 * @brief Deinitializes the given GPIO
 *
 * @param  GPIOx: where x can be (A..C and H)
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @param [IN] value reset value
 * @retval none
 */
void HW_GPIO_Deinit( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t value )
{
  HW_GPIO_Write( GPIOx, GPIO_Pin, value );
  
  /* DeInit the GPIO pin */
  HAL_GPIO_DeInit( GPIOx, GPIO_Pin );
}

/*!
 * @brief Records the interrupt handler for the GPIO  object
 *
 * @param  GPIOx: where x can be (A..C and H)
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @param [IN] prio       NVIC priority (0 is highest)
 * @param [IN] irqHandler  points to the  function to execute
 * @retval none
 */
void HW_GPIO_SetIrq( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t prio,  GpioIrqHandler *irqHandler )
{
  IRQn_Type IRQnb;

  uint32_t BitPos = HW_GPIO_GetBitPos( GPIO_Pin );

  if( irqHandler != NULL )
  {
    GpioIrq[BitPos] = irqHandler;

    IRQnb = MSP_GetIRQn( GPIO_Pin );

    HAL_NVIC_SetPriority( IRQnb, prio, 0 );

    __HAL_GPIO_EXTI_CLEAR_IT( GPIO_Pin );
    HAL_NVIC_ClearPendingIRQ( IRQnb );

    HAL_NVIC_EnableIRQ( IRQnb );
  }
  else
  {
    GpioIrq[BitPos] = NULL;
  }
}

/*!
 * @brief Enable IRQ according to GPIO_Pin. Call HW_GPIO_SetIrq before this function
 * @param  GPIO_Pin: specifies the IRQ  to be enabled.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @retval none
 */
void HW_GPIO_enable_irq( uint16_t GPIO_Pin )
{
  IRQn_Type IRQnb;

  IRQnb = MSP_GetIRQn( GPIO_Pin );
  
  __HAL_GPIO_EXTI_CLEAR_IT( GPIO_Pin );
  HAL_NVIC_ClearPendingIRQ( IRQnb );

  HAL_NVIC_EnableIRQ( IRQnb );
}

/*!
 * @brief Disable IRQ according to GPIO_Pin. Call HW_GPIO_SetIrq before this function
 * @param  GPIO_Pin: specifies the IRQ  to be disabled.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @retval none
 */
void HW_GPIO_disable_irq( uint16_t GPIO_Pin )
{
  IRQn_Type IRQnb;

  IRQnb = MSP_GetIRQn( GPIO_Pin );
  
  HAL_NVIC_DisableIRQ( IRQnb );
  
  __HAL_GPIO_EXTI_CLEAR_IT( GPIO_Pin );
  HAL_NVIC_ClearPendingIRQ( IRQnb );
}

/*!
 * @brief Execute the interrupt from the object
 *
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @retval none
 */
void HW_GPIO_IrqHandler( uint16_t GPIO_Pin )
{
  uint32_t BitPos = HW_GPIO_GetBitPos( GPIO_Pin );

  if( GpioIrq[BitPos]  != NULL )
  {
    GpioIrq[BitPos]( NULL );
  }
}

/*!
 * @brief Writes the given value to the GPIO output
 *
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @param [IN] value New GPIO output value
 * @retval none
 */
void HW_GPIO_Write( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,  uint32_t value )
{
  HAL_GPIO_WritePin( GPIOx, GPIO_Pin, ( GPIO_PinState )value );
}

/*!
 * @brief Reads the current GPIO input value
 *
 * @param  GPIOx: where x can be (A..C and H)
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @retval value   Current GPIO input value
 */
uint32_t HW_GPIO_Read( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin )
{
  return HAL_GPIO_ReadPin( GPIOx, GPIO_Pin );
}

/*!
 * @brief Toggle the current GPIO pin
 *
 * @param  GPIOx: where x can be (A..C and H)
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @retval none
 */
void HW_GPIO_Toggle( GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin )
{
  HAL_GPIO_TogglePin( GPIOx, GPIO_Pin );
}

/* Private functions ---------------------------------------------------------*/

/*!
 * @brief Get the position of the bit set in the GPIO_Pin
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @retval the position of the bit
 */
static uint8_t HW_GPIO_GetBitPos( uint16_t GPIO_Pin )
{
  uint8_t PinPos = 0;

  if( ( GPIO_Pin & 0xFF00 ) != 0 )
  {
    PinPos |= 0x8;
  }
  if( ( GPIO_Pin & 0xF0F0 ) != 0 )
  {
    PinPos |= 0x4;
  }
  if( ( GPIO_Pin & 0xCCCC ) != 0 )
  {
    PinPos |= 0x2;
  }
  if( ( GPIO_Pin & 0xAAAA ) != 0 )
  {
    PinPos |= 0x1;
  }

  return PinPos;
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected to the EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  HW_GPIO_IrqHandler(GPIO_Pin);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
