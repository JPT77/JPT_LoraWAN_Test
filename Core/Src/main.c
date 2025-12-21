/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "app_lorawan.h"

/* Private includes ---------------------------------------------------------- */
/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main( void )
{
  /* MCU Configuration--------------------------------------------------------- */

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_LoRaWAN_Init();

  /* Infinite loop */
  while( 1 )
  {
    MX_LoRaWAN_Process();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config( void )
{
  HAL_StatusTypeDef hal_state = HAL_OK;
  RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
  RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

  /** Configure LSE Drive Capability */
  __HAL_RCC_LSEDRIVE_CONFIG( RCC_LSEDRIVE_LOW );

  /** Configure the main internal regulator output voltage */
  __HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE1 );

  /** Initializes the CPU, AHB and APB busses clocks */
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState        = RCC_LSE_ON;
  RCC_OscInitStruct.HSEDiv          = RCC_HSE_DIV1;//RCC_HSE_DIV2;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM        = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN        = 12;
  RCC_OscInitStruct.PLL.PLLP        = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR        = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ        = RCC_PLLQ_DIV2;

////  HAL_RCC_MCOConfig( RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1 );
//  if( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
//  {
//    Error_Handler();
//  }
  do
  {
    hal_state = HAL_RCC_OscConfig( &RCC_OscInitStruct );
  }
  while( hal_state == HAL_TIMEOUT );
  if( hal_state != HAL_OK )
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_HCLK3   | RCC_CLOCKTYPE_HCLK
                                    | RCC_CLOCKTYPE_SYSCLK  | RCC_CLOCKTYPE_PCLK1
                                    | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV4;//RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider  = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider  = RCC_SYSCLK_DIV1;

//  if( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_0 ) != HAL_OK )
//  {
//    Error_Handler();
//  }
  do
  {
    hal_state = HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_0 );
  }
  while( hal_state == HAL_TIMEOUT );
  if( hal_state != HAL_OK )
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler( void )
{
  /* User can add his own implementation to report the HAL error return state */
  while( 1 )
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed( uint8_t *file, uint32_t line )
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  while( 1 )
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
