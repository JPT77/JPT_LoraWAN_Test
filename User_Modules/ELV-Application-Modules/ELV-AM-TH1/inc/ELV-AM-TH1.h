/**
* @file ELV-AM-TH1.h
* @brief Header file for Application functions.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ELV_AM_TH1_H_
#define ELV_AM_TH1_H_

#ifdef __cplusplus
 extern "C" {
#endif

// Includes --------------------------------------------------------------------
#include "103AT_2B_Values.h"

// Definitions -----------------------------------------------------------------
// Exported types --------------------------------------------------------------
typedef struct th1_data_values
{
 uint16_t u16_operating_voltage;    // Current system operating voltage
 int16_t i16_ntc_temperature;       // Current temperature of the external NTC sensor
 int16_t i16_HDC2080_temperature;   // Current temperature from the I2C sensor
 uint8_t u8_HDC2080_humidity;       // Current humidity from the I2C sensor
}th1_data_values_t;

// Exported macro --------------------------------------------------------------
// Exported functions ----------------------------------------------------------
void elv_am_th1_init( void );
bool elv_am_th1_is_present( void );
void elv_am_th1_do_measurements( th1_data_values_t *t_th1_data_values );

#ifdef __cplusplus
}
#endif

/**
  * @}
  */
#endif /* ELV_AM_TH1_H_ */
