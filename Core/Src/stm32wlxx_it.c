/**
  ******************************************************************************
  * @file    stm32wlxx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32wlxx_it.h"
#include "stm32wlxx_ll_usart.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
extern SUBGHZ_HandleTypeDef hsubghz;

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler( void )
{
  /* Go to infinite loop when NMI occurs in case: */
  /* Go to infinite loop when NMI occurs */
  while( 1 )
  {
    ;
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler( void )
{
  /* HardFault_IRQn 0 */
  while ( 1 )
  {
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler( void )
{
  /* MemoryManagement_IRQn 0 */
  while ( 1 )
  {
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler( void )
{
  /* BusFault_IRQn 0 */
  while ( 1 )
  {
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler( void )
{
  /* UsageFault_IRQn 0 */
  while ( 1 )
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler( void )
{
  /* SVCall_IRQn 0 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler( void )
{
  /* DebugMonitor_IRQn 0 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler( void )
{
  /* PendSV_IRQn 0 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler( void )
{
  /* SysTick_IRQn 0 */
  HAL_IncTick();
}

/******************************************************************************/
/* STM32WLxx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32wlxx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RTC Tamper, RTC TimeStamp, LSECSS and RTC SSRU Interrupts.
  */
void TAMP_STAMP_LSECSS_SSRU_IRQHandler( void )
{
  /* TAMP_STAMP_LSECSS_SSRU_IRQn 0 */
  HAL_RTCEx_SSRUIRQHandler( &hrtc );
}

/**
  * @brief This function handles USART1 Interrupt.
  */
void USART1_IRQHandler( void )
{
  HAL_UART_IRQHandler( &huart1 );
}

void USART2_IRQHandler( void )
{
  HAL_UART_IRQHandler( &huart2 );
}

/**
  * @brief This function handles RTC Alarms (A and B) Interrupt.
  */
void RTC_Alarm_IRQHandler( void )
{
  /* RTC_Alarm_IRQn 0 */
  HAL_RTC_AlarmIRQHandler( &hrtc );
}

/**
  * @brief This function handles SUBGHZ Radio Interrupt.
  */
void SUBGHZ_Radio_IRQHandler( void )
{
  /* SUBGHZ_Radio_IRQn 0 */
  HAL_SUBGHZ_IRQHandler( &hsubghz );
}

void EXTI0_IRQHandler( void )
{
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}

void EXTI1_IRQHandler( void )
{
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_1 );
}

void EXTI2_IRQHandler( void )
{
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_2 );
}

void EXTI3_IRQHandler( void )
{
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_3 );
}

void EXTI4_IRQHandler( void )
{
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_4 );
}


void EXTI9_5_IRQHandler( void )
{
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_5 );
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_6 );
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_7 );
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_8 );
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_9 );
}

void EXTI15_10_IRQHandler( void )
{
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_10 );
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_11 );
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_12 );
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_13 );
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_14 );
  HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_15 );
}

/**
  * @brief  This function handles Flash interrupt request.
  * @param  None
  * @retval None
  */
void FLASH_IRQHandler( void )
{    
  HAL_FLASH_IRQHandler();
}

/**
  * @brief  This function handles PVD interrupt request.
  * @param  None
  * @retval None
  */
void PVD_PVM_IRQHandler( void )
{
  /* Loop inside the handler to prevent the Cortex from using the Flash,
     allowing the flash interface to finish any ongoing transfer. */
  while( __HAL_PWR_GET_FLAG( PWR_FLAG_PVDO ) != RESET )
  {
    ;
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
