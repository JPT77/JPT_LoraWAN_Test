/**
 * @file hdc2080_defs.h
 * @author Marcel Maas
 * @date 13. February 2019
 * @brief HDC2080 definitions.
 */

#ifndef HDC2080_DEFS_H_
#define HDC2080_DEFS_H_

/*----------------------[HDC2080 Various]----------------------*/
#define HDC2080_TEMP_MIN_VALUE        -40
#define HDC2080_TEMP_MAX_VALUE        125
#define HDC2080_HUM_MIN_VALUE         0
#define HDC2080_HUM_MAX_VALUE         100

#define HDC2080_TEMP_DELTA_MIN        0.5
#define HDC2080_HUM_DELTA_MIN         3

#define HDC2080_2POW16                65536
#define HDC2080_2POW8                 256

#define HDC2080_NOT_CONFIGURED        0
#define HDC2080_CONFIGURED_SINGLE     1
#define HDC2080_CONFIGURED_CONTINUOUS 2
/*-------------------[HDC2080 Various - END]-------------------*/

/*----------------------[HDC2080 Address]----------------------*/
#define HDC2080_ADDR_GND_CONNECT	(0x40 << 1)
#define HDC2080_ADDR_VDD_CONNECT	(0x41 << 1)
/*-------------------[HDC2080 Address - END]-------------------*/

/*----------------------[HDC2080 Register]----------------------*/
#define HDC2080_TEMPERATURE_LOW       0x00	// R
#define HDC2080_TEMPERATURE_HIGH      0x01	// R
#define HDC2080_HUMIDITY_LOW          0x02	// R
#define HDC2080_HUMIDITY_HIGH         0x03	// R
#define HDC2080_INTERRUPT_DRDY        0x04	// R
#define HDC2080_TEMPERATURE_MAX       0x05	// R
#define HDC2080_HUMIDITY_MAX          0x06	// R
#define HDC2080_INTERRUPT_ENABLE      0x07	// R/W
#define HDC2080_TEMP_OFFSET_ADJUST    0x08	// R/W
#define HDC2080_HUM_OFFSET_ADJUST     0x09	// R/W
#define HDC2080_TEMP_THR_LOW          0x0A	// R/W
#define HDC2080_TEMP_THR_HIGH         0x0B	// R/W
#define HDC2080_RH_THR_LOW            0x0C	// R/W
#define HDC2080_RH_THR_HIGH           0x0D	// R/W
#define HDC2080_RESET_DRDY_INT_CONF   0x0E	// R/W
#define HDC2080_MEASUREMENT_CONF      0x0F	// R/W
#define HDC2080_MANUFACTURER_ID_LOW   0xFC	// R
#define HDC2080_MANUFACTURER_ID_HIGH  0xFD	// R
#define HDC2080_DEVICE_ID_LOW         0xFE	// R
#define HDC2080_DEVICE_ID_HIGH        0xFF	// R
/*-------------------[HDC2080 Register - END]-------------------*/

/*--------------------[HDC2080 Sample Rate]--------------------*/
#define HDC2080_MANUAL                0x00
#define HDC2080_TWO_MINS              0x10
#define HDC2080_ONE_MINS              0x20
#define HDC2080_TEN_SECONDS           0x30

#define HDC2080_SAMPLE_RATE_DEL_MASK  0x8F
/*-----------------[HDC2080 Sample Rate - END]-----------------*/

#endif /* HDC2080_DEFS_H_ */
