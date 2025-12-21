/**
 * @file i2c.c
 * @brief I2C control functions.
 */

#include "i2c.h"



I2C_HandleTypeDef hi2c2;
static void wait_for_i2c_rdy( void );

static uint32_t u32_timing = 0x20303E5D;
const uint32_t I2C_CLOCK_TIMINGS[] = { 0x20303E5D, 0xF000F8F9, 0x9010DEFF, 0x20308EFD, 0x20303E5D, 0x2010091A, 0x20000209};

/*!
 * @brief Initialize I2C module
 */
void i2c_mod2_init( void )
{
  MX_I2C2_Init( I2C_CLOCK_100kHz );
  
  wait_for_i2c_rdy();
}

/*!
 * @brief Deinitialize I2C module
 */
void i2c_mod2_deinit( void )
{
  if( HAL_I2C_DeInit( &hi2c2 ) != HAL_OK )
  {
    Error_Handler();
  }
}


/*!
  * @brief I2C write register function for I2C2.
  * @param slave_addr I2C addresse of slave to write at
  * @param reg_addr Start addresse to write at
  * @param reg_addr_size Size of reg_addr in byte
  * @param tx_data Pointer to the data to write
  * @param tx_len Size of tx_data in byte
  */
HAL_StatusTypeDef i2c_mod2_write_register( uint8_t slave_addr, uint16_t reg_addr, uint8_t reg_addr_size, uint8_t *tx_data, uint16_t tx_len )
{
  HAL_StatusTypeDef ret;
  ret = HAL_I2C_Mem_Write( &hi2c2, slave_addr, reg_addr, reg_addr_size, tx_data, tx_len, 1000 );
  wait_for_i2c_rdy();
  
  return ret;
}

/*!
  * @brief I2C read register function for I2C2.
  * @param slave_addr I2C addresse of slave to read from
  * @param reg_addr Start addresse for reading
  * @param reg_addr_size Size of reg_addr in byte
  * @param rx_data Pointer to an array that should store the data
  * @param rx_len Size of rx_data in byte
  */
HAL_StatusTypeDef i2c_mod2_read_register( uint8_t slave_addr, uint16_t reg_addr, uint8_t reg_addr_size, uint8_t *rx_data, uint16_t rx_len )
{
  HAL_StatusTypeDef ret;
  ret = HAL_I2C_Mem_Read( &hi2c2, slave_addr, reg_addr, reg_addr_size, rx_data, rx_len, 1000 );
  wait_for_i2c_rdy();
  
  return ret;
}


/*!
  * @brief I2C receive function for I2C2.
  * @param transmitter_addr Addresse to receive from
  * @param rx_data Pointer to an array that should store the data
  * @param rx_len Number of byte to receive
  * @todo test
  */
void i2c_mod2_receive( uint8_t transmitter_addr, uint8_t *rx_data, uint16_t rx_len )
{
  HAL_I2C_Master_Receive( &hi2c2, transmitter_addr, rx_data, rx_len, 2000 );
}

/*!
  * @brief I2C transmit function for I2C2.
  * @param receiver_addr Target I2C addresse
  * @param tx_data Array containing data to transmit
  * @param tx_len Number of byte to transmit
  * @todo test
  */
void i2c_mod2_transmit( uint8_t receiver_addr, uint8_t *tx_data, uint16_t tx_len )
{
  HAL_I2C_Master_Transmit( &hi2c2, receiver_addr, tx_data, tx_len, 2000 );
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
void MX_I2C2_Init( I2C_CLOCK_t i2c_clock )
{
  u32_timing                    = I2C_CLOCK_TIMINGS[i2c_clock];
  hi2c2.Instance                = I2C2;
  hi2c2.Init.Timing             = u32_timing;
  hi2c2.Init.OwnAddress1        = 0;
  hi2c2.Init.AddressingMode     = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode    = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2        = 0;
  hi2c2.Init.OwnAddress2Masks   = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode    = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode      = I2C_NOSTRETCH_DISABLE;//I2C_NOSTRETCH_ENABLE;

  if( HAL_I2C_Init( &hi2c2 ) != HAL_OK )
  {
    Error_Handler();
  }
  
//  hi2c2.Instance->CR1 |= I2C_FLAG_TXE;

  /** Configure Analogue filter */
  if( HAL_I2CEx_ConfigAnalogFilter( &hi2c2, I2C_ANALOGFILTER_ENABLE ) != HAL_OK )
  {
    Error_Handler();
  }

  /** Configure Digital filter */
  if( HAL_I2CEx_ConfigDigitalFilter( &hi2c2, 0 ) != HAL_OK )
  {
    Error_Handler();
  }
}

/**
 * @brief I2C2 DeInitialization Function
 * @param None
 * @retval None
 */
void MX_I2C2_DeInit( void )
{
  if( HAL_I2C_DeInit( &hi2c2 ) != HAL_OK )
  {
    Error_Handler();
  }
}

static void wait_for_i2c_rdy( void )
{
  HAL_I2C_StateTypeDef value;
  do
  {
    value = HAL_I2C_GetState( &hi2c2 );
  }
  while( value != HAL_I2C_STATE_READY );
}

/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit( I2C_HandleTypeDef* hi2c )
{
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  if( hi2c->Instance == I2C2 )
  {  
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2C2 GPIO Configuration    
    PA11     ------> I2C2_SDA
    PA12     ------> I2C2_SCL
    */
    GPIO_InitStruct.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;//GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;

    HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    /* Peripheral clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  }

}

/**
* @brief I2C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit( I2C_HandleTypeDef* hi2c )
{
  if( hi2c->Instance == I2C2 )
  {
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();
  
    /**I2C2 GPIO Configuration    
    PA11     ------> I2C1_SDA
    PA12     ------> I2C1_SCL
    */
#warning "i2c pins not deinitialised -> check current consumption"
//    De-/init of i2c Pin A12 disables the interrupt configuration from bma_int2 which is Pin B12
//    HAL_GPIO_DeInit( GPIOB, GPIO_PIN_11 | GPIO_PIN_12 );
  }
}
