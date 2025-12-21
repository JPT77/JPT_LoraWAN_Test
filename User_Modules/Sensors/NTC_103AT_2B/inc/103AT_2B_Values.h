/**
* @file    103AT_2B_Values.h
* @brief   Module for calculating the temperature using an NTC resistor of type 103AT 2B.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

#ifndef NTC_103AT_2B_VALUES_H
#define NTC_103AT_2B_VALUES_H


/**
 * The definitions for the used look up table must be different to 0x00.
 * Because each undefined activation status will be 0x00.
**/
#define ARRAY1            0x01  /* Array with values for -20�C to +50�C */
#define ARRAY2            0x02  /* Array with values for   0�C to +100�C */
#define COMPLETE_ARRAY    0x03  /* Array with values for -50�C to +100�C */


#define USED_LOOK_UP_TABLE     COMPLETE_ARRAY

#ifdef USED_LOOK_UP_TABLE
  #if( USED_LOOK_UP_TABLE == 0x00 )
    #error "103AT_2B_Values.h: USED_LOOK_UP_TABLE must be different to 0x00!!!"
  #elif( USED_LOOK_UP_TABLE == ARRAY1 )
    
  #elif( USED_LOOK_UP_TABLE == ARRAY2 )
    
  #elif( USED_LOOK_UP_TABLE == COMPLETE_ARRAY )
    
  #else
    #error "103AT_2B_Values.h: USED_LOOK_UP_TABLE was incorrectly defined !!!"
  #endif
#else
  #error "103AT_2B_Values.h: USED_LOOK_UP_TABLE was not defined !!!"
#endif /* USED_LOOK_UP_TABLE */

#define SERIES_RESTISTANCE      10000
#define ADC_MAX_VALUE           4095

#define TEMPERATURE_UNKNOWN     0x8000
#define TEMPERATURE_OVERFLOW    0x8001
#define TEMPERATURE_UNDERFLOW   0x8002



extern int16_t NTC_103AT_2B_VALUES_get_temperature(uint16_t u16_ADC);
extern int16_t NTC_103AT_2B_VALUES_get_temperature_2( uint16_t u16_operating_voltage, uint16_t u16_ntc_voltage );

#endif /* NTC_103AT_2B_VALUES_H */
