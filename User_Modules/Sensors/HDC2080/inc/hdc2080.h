/**
 * @file hdc2080.h
 * @author Marcel Maas
 * @date 13. February 2019
 * @brief HDC2080 API. Low Power Humidity and Temperature Digital Sensor.
 */

#ifndef HDC2080_H_
#define HDC2080_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hdc2080_defs.h"
#include "hdc2080_low_level.h"

typedef void ( *hdc2080_delay_fptr_t )( uint32_t period );                                      // Delay function pointer
typedef void ( *hdc2080_drdy_int_fptr_t )( bool *init );                                        // DRDY/INT function pointer
typedef void ( *hdc2080_en_drdy )( void );                                                      // Enable DRDY function pointer
typedef void ( *hdc2080_en_interruppt )( void );                                                // Enable interrupt function pointer
typedef HAL_StatusTypeDef ( *hdc2080_i2c_data_rw_fptr_t )( uint8_t slave_addr, uint16_t reg_addr, uint8_t reg_addr_size, uint8_t *rx_data, uint16_t rx_len );   // I2C Read/Write function pointer

typedef struct
{
  hdc2080_delay_fptr_t delay_ms;                                                                // Delay Milliseconds
  hdc2080_drdy_int_fptr_t drdy;                                                                 // Get State of DRDY/wait for INT
  hdc2080_en_drdy en_drdy;                                                                      // Enable DRDY gpio
  hdc2080_en_interruppt en_int;                                                                 // Enable DRDY interrupt
  hdc2080_i2c_data_rw_fptr_t i2c_read;                                                          // I2C Data Read
  hdc2080_i2c_data_rw_fptr_t i2c_write;                                                         // I2C Data Write
} hdc2080_low_level_functions_TypeDef;

void hdc2080_init( void );                                                                      // Initialize Low Level Functions
void hdc2080_deinit( void );                                                                    // Deinit HDC2080
void hdc2080_set_low_level_functions( hdc2080_low_level_functions_TypeDef low_level_funcptr );  // Set User Functions
void hdc2080_conf_single( void );                                                               // Configure HDC2080 for single acquisition
void hdc2080_conf_continuous( uint8_t sample_rate );                                            // Configure HDC2080 for continuous acquisition

void hdc2080_set_int_config( uint32_t temp_hum );                                               // Set Interrupt Configuration for Temp_Hum_Thresholds
void hdc2080_set_sample_rate( uint8_t sample_rate );                                            // Set Sample Rate
void hdc2080_set_temperature_thres( double current_temperature );                               // Set temperature thresholds
void hdc2080_set_humidity_thres( double current_humidity );                                     // Set humidity thresholds

void hdc2080_start_conversion( void );                                                          // Start Measurement
void hdc2080_end_conversion( void );                                                            // End Measurement
void hdc2080_wait_for_data( bool *init );                                                       // Wait for new Data
uint32_t hdc2080_get_temperature_humidity( void );                                              // Return Temperature and Humidity
double hdc2080_decode_temperature( uint16_t temperature );                                      // Decode Temperature to double
double hdc2080_decode_humidity( uint16_t humidity );                                            // Decode Humidity to double

void hdc2080_reset( void );                                                                     // Reset HDC2080

bool hdc2080_validate_manufacturer_device_id(void);
uint16_t hdc2080_get_manufacturer_id(void);
uint16_t hdc2080_get_device_id(void);

uint8_t hdc2080_read_register( uint8_t reg );                                                   // Read single Register
void hdc2080_read_burst( uint8_t reg, uint8_t *data, uint8_t len );                             // Read multiple Registers
void hdc2080_write_register( uint8_t reg, uint8_t data );                                       // Write single Register
void hdc2080_write_burst( uint8_t reg, uint8_t *data, uint8_t len );                            // Write multiple Registers

#endif /* HDC2080_H_ */
