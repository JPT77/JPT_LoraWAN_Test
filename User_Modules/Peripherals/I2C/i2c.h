/**
 * @file i2c.h
 * @author Marcel Maas
 * @date 20. February 2020
 * @brief I2C control functions.
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "main.h"

#include "stm32wlxx_hal_gpio.h"
#include "stm32wlxx_hal_i2c.h"

typedef enum
{
  I2C_DEFAULT_CLOCK,
  I2C_CLOCK_6kHz,
  I2C_CLOCK_10kHz,
  I2C_CLOCK_40kHz,
  I2C_CLOCK_100kHz,
  I2C_CLOCK_400kHz,
  I2C_CLOCK_1MHz,
} I2C_CLOCK_t;

void MX_I2C2_Init( I2C_CLOCK_t i2c_clock );
void MX_I2C2_DeInit( void );   
   
void i2c_mod2_init( void );
void i2c_mod2_deinit( void );   


/*!
  * @brief I2C write register function for I2C2.
  * @param slave_addr I2C addresse of slave to write at
  * @param reg_addr Start addresse for writing
  * @param reg_addr_size Size of reg_addr in byte
  * @param tx_data Pointer to the data to write
  * @param tx_len
  */
HAL_StatusTypeDef i2c_mod2_write_register( uint8_t slave_addr, uint16_t reg_addr, uint8_t reg_addr_size, uint8_t *tx_data, uint16_t tx_len );

/*!
  * @brief I2C read register function for I2C2.
  * @param slave_addr I2C addresse of slave to read from
  * @param reg_addr Start addresse for reading
  * @param rx_data Pointer to an array that should store the data
  * @param rx_len
  */
HAL_StatusTypeDef i2c_mod2_read_register( uint8_t slave_addr, uint16_t reg_addr, uint8_t reg_addr_size, uint8_t *rx_data, uint16_t rx_len );

/*!
  * @brief I2C receive function for I2C2.
  * @param transmitter_addr Addresse to receive from
  * @param rx_data Pointer to an array that should store the data
  * @param rx_len Number of byte to receive
  * @todo test
  */
void i2c_mod2_receive( uint8_t transmitter_addr, uint8_t *rx_data, uint16_t rx_len );

/*!
  * @brief I2C transmit function for I2C2.
  * @param receiver_addr Target I2C addresse
  * @param tx_data Array containing data to transmit
  * @param tx_len Number of byte to transmit
  * @todo test
  */
void i2c_mod2_transmit( uint8_t receiver_addr, uint8_t *tx_data, uint16_t tx_len );

#endif /* I2C_H_ */
