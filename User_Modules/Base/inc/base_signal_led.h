/**
* @file base_signal_led.h
* @brief Header file for LED signals.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/** @addtogroup BASE_SIGNAL_LED
* @{
**/
/*---------------------------------------------------------------------------*/

#ifndef __BASE_SIGNAL_LED_H__
#define __BASE_SIGNAL_LED_H__

// Includes --------------------------------------------------------------------
#include <stdbool.h>
// Definitions -----------------------------------------------------------------
#define SIGNAL_LED_JOINING_PROCESS_ON                         100
#define SIGNAL_LED_JOINING_PROCESS_OFF                        400
#define SIGNAL_LED_JOINING_OK                                 500
#define SIGNAL_LED_JOINING_NOK                                500

#define SIGNAL_LED_BUTTON_STEP_1_ON                           100
#define SIGNAL_LED_BUTTON_STEP_1_OFF                          100
#define SIGNAL_LED_BUTTON_STEP_2_ON                           200
#define SIGNAL_LED_BUTTON_STEP_2_OFF                          800

#define SIGNAL_LED_BUTTON_STEP_1_TIME_LIMIT                   5000
#define SIGNAL_LED_BUTTON_STEP_2_TIME_LIMIT                   8000
// Typedefs --------------------------------------------------------------------
typedef enum
{
  SIGNAL_LED_ID_UNDEFINED = 0,
  SIGNAL_LED_ID_JOIN_PROCESS,
  SIGNAL_LED_ID_JOIN_OK,
  SIGNAL_LED_ID_JOIN_NOK,
  SIGNAL_LED_ID_BUTTON_PROCESS,
  
  SIGNAL_LED_ID_NBR
} signal_led_id_t;

// Variables -------------------------------------------------------------------
// Prototypes ------------------------------------------------------------------
void signal_led_init( void );

void signal_led_start( signal_led_id_t id, void ( *cb )( void *) );
void signal_led_stop( signal_led_id_t id, bool b_call_cb );
void signal_led_reset_values( void );

#endif /* __BASE_SIGNAL_LED__ */
