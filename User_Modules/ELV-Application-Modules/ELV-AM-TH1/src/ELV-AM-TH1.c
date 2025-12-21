/**
* @file ELV-AM-TH1.c
* @brief Source file for ELV-AM-TH1 functions.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/* Includes ------------------------------------------------------------------*/
#include "hdc2080.h"
#include "103AT_2B_Values.h"
#include "adc_if.h"
#include "base.h"
#include "ELV-AM-TH1.h"

// Definitions -----------------------------------------------------------------
#define ADC_CHANNEL_NTC_TH1   ADC_CHANNEL_10  // ADC Channel for the NTC sensor

// Typedefs --------------------------------------------------------------------
// Variables -------------------------------------------------------------------
/**
  * @brief Boolean to indicate that the HDC2080 hardware has been initialized
  */
static bool is_hdc2080_init = false;

/**
  * @brief Boolean to indicate that the HDC2080 has been detected
  */
static bool ELV_AM_TH1_present = false;

// Prototypes ------------------------------------------------------------------
void elv_am_th1_do_measurements( th1_data_values_t *t_th1_data_values );

// Exported functions ----------------------------------------------------------
/**
  * @brief  This function collects all sensor and voltage values.
  * @param[in] Pointer to the RAM variable of interval time.
  * @retval None
  */
void elv_am_th1_init( void )
{

  hdc2080_init();
  is_hdc2080_init = true;
  if( hdc2080_validate_manufacturer_device_id() == true )
  {
    ELV_AM_TH1_present = true;
    hdc2080_conf_single();
  }
}

/**
  * @brief  This function collects all sensor and voltage values.
  * @param[in] Pointer to the RAM variable of interval time.
  * @retval None
  */
bool elv_am_th1_is_present( void )
{
  return( ELV_AM_TH1_present );
}

/**
  * @brief  This function collects all sensor and voltage values.
  * @param[in] Pointer to the RAM variable of interval time.
  * @retval None
  */
void elv_am_th1_do_measurements( th1_data_values_t *t_th1_data_values )
{
  t_th1_data_values->u16_operating_voltage    = 0;  // System operating voltage
  t_th1_data_values->i16_ntc_temperature      = 0;  // Temperature of the external NTC sensor
  t_th1_data_values->i16_HDC2080_temperature  = 0;  // Temperature from the I2C sensor
  t_th1_data_values->u8_HDC2080_humidity      = 0;  // Humidity from the I2C sensor
  uint16_t u16_ntc_voltage_mv     = 0;              // Voltage across NTC to ground
  uint16_t u16_supply_voltage_mv  = 0;              // Voltage which supplies the MCU
  uint32_t u32_HDC2080_temp_hum   = 0;              // Register value of temperature and humidity from the I2C sensor
  double f64_HDC2080_temperature  = 0;              // Raw temperature from the I2C sensor

  // Measure NTC voltage
  base_init_en_adc_supply();
  base_en_adc_supply( true );
  HAL_Delay( 10 );
  u16_ntc_voltage_mv = adc_get_channel_level( ADC_CHANNEL_NTC_TH1 );
  base_deinit_en_adc_supply();

  // Get operating voltage
  u16_supply_voltage_mv = base_get_supply_level();

  // Calculate NTC temperature
  t_th1_data_values->i16_ntc_temperature = NTC_103AT_2B_VALUES_get_temperature_2( u16_supply_voltage_mv, u16_ntc_voltage_mv );

  // Get HDC2080 temperature and humidity
  hdc2080_start_conversion();
  hdc2080_wait_for_data( &is_hdc2080_init );
  u32_HDC2080_temp_hum = hdc2080_get_temperature_humidity();
  f64_HDC2080_temperature = ( hdc2080_decode_temperature( ( uint16_t )( u32_HDC2080_temp_hum >> 16 ) ) ) * 10.0;

  // Rounding HDC2080 temperature
  if( 0 > f64_HDC2080_temperature )
  {
    t_th1_data_values->i16_HDC2080_temperature = ( int16_t )( f64_HDC2080_temperature - 0.5 );
  }
  else
  {
    t_th1_data_values->i16_HDC2080_temperature = ( int16_t )( f64_HDC2080_temperature + 0.5 );
  }

  // Rounding HDC2080 humidity
  t_th1_data_values->u8_HDC2080_humidity = ( uint8_t )( hdc2080_decode_humidity( ( uint16_t )u32_HDC2080_temp_hum ) + 0.5 );
}

