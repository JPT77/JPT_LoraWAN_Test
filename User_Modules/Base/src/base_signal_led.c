/**
* @file base_signal_led.c
* @brief Source file for LED signals.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/** @addtogroup BASE
* @{
**/
/*---------------------------------------------------------------------------*/

// Includes --------------------------------------------------------------------
#include "main.h"
#include "base_signal_led.h"
#include "led.h"
#include "stm32_timer.h"
#include "stm32_systime.h"
#include "sys_app.h"

// Definitions -----------------------------------------------------------------
// Typedefs --------------------------------------------------------------------
// Variables -------------------------------------------------------------------
static UTIL_TIMER_Object_t signal_led_timer;
static void ( *signal_led_sequenz_cb )( void *) = NULL;
static signal_led_id_t signal_led_id            = SIGNAL_LED_ID_UNDEFINED;
static uint32_t signal_led_period               = 0;
static SysTime_t signal_led_timestamp           = { .Seconds = 0, .SubSeconds = 0 };
// Prototypes ------------------------------------------------------------------
static void signal_led_timer_cb( void *context );

void signal_led_init( void )
{
  leds_init();
  UTIL_TIMER_Create( &signal_led_timer, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, signal_led_timer_cb, NULL );
}

void signal_led_start( signal_led_id_t id, void ( *cb )( void *) )
{
  UTIL_TIMER_Stop( &signal_led_timer );
  leds_off();
  signal_led_reset_values();

  signal_led_id = id;

  if( cb != NULL )
  {
    signal_led_sequenz_cb = cb;
  }
  
  switch( signal_led_id )
  {
    case SIGNAL_LED_ID_JOIN_PROCESS:
    {
      signal_led_period = SIGNAL_LED_JOINING_PROCESS_ON;
      led_green_toggle();
      led_red_toggle();
    }
    break;
    case SIGNAL_LED_ID_JOIN_OK:
    {
      signal_led_period = SIGNAL_LED_JOINING_OK;
      led_green_en( true );;
    }
    break;
    case SIGNAL_LED_ID_JOIN_NOK:
    {
      signal_led_period = SIGNAL_LED_JOINING_NOK;
      led_red_en( true );
    }
    break;
    case SIGNAL_LED_ID_BUTTON_PROCESS:
    {
      signal_led_period = SIGNAL_LED_BUTTON_STEP_1_ON;
      signal_led_timestamp = SysTimeGet();
      led_green_toggle();
      led_red_toggle();
    }
    break;
    default:
    {
      signal_led_reset_values();
      return;
    }
    break;
  }
  UTIL_TIMER_SetPeriod( &signal_led_timer, signal_led_period );
  UTIL_TIMER_Start( &signal_led_timer );
}

void signal_led_stop( signal_led_id_t id, bool b_call_cb )
{
  UTILS_ENTER_CRITICAL_SECTION();
  UTIL_TIMER_Stop( &signal_led_timer );
  UTILS_EXIT_CRITICAL_SECTION();
  leds_off();
  
  if( id != SIGNAL_LED_ID_UNDEFINED )
  {
    if( b_call_cb && signal_led_sequenz_cb != NULL )
    {
      signal_led_sequenz_cb( NULL );
    }
  }

  signal_led_reset_values();
}

void signal_led_reset_values( void )
{
  signal_led_id         = SIGNAL_LED_ID_UNDEFINED;
  signal_led_period     = 0;
  signal_led_sequenz_cb = NULL;
  signal_led_timestamp  = ( SysTime_t ){ .Seconds = 0, .SubSeconds = 0 };
}

static void signal_led_timer_cb( void *context )
{
  switch( signal_led_id )
  {
    case SIGNAL_LED_ID_JOIN_PROCESS:
    {
      signal_led_period = ( signal_led_period == SIGNAL_LED_JOINING_PROCESS_ON ) ? SIGNAL_LED_JOINING_PROCESS_OFF : SIGNAL_LED_JOINING_PROCESS_ON;
      UTIL_TIMER_SetPeriod( &signal_led_timer, signal_led_period );
      led_green_toggle();
      led_red_toggle();
      UTIL_TIMER_Start( &signal_led_timer );
    }
    break;
    case SIGNAL_LED_ID_JOIN_OK:
    {
      leds_off();
      if( signal_led_sequenz_cb != NULL )
      {
        signal_led_sequenz_cb( NULL );
      }
      signal_led_reset_values();
    }
    break;
    case SIGNAL_LED_ID_JOIN_NOK:
    {
      leds_off();
      if( signal_led_sequenz_cb != NULL )
      {
        signal_led_sequenz_cb( NULL );
      }
      signal_led_reset_values();
    }
    break;
    case SIGNAL_LED_ID_BUTTON_PROCESS:
    {
      SysTime_t timestamp_now = SysTimeGet();
      SysTime_t div_timestamp = SysTimeSub( timestamp_now, signal_led_timestamp );
      uint32_t time_diff = ( div_timestamp.Seconds * 1000 ) + div_timestamp.SubSeconds;
      
      if( time_diff < SIGNAL_LED_BUTTON_STEP_1_TIME_LIMIT )
      {
        signal_led_period = ( signal_led_period == SIGNAL_LED_BUTTON_STEP_1_ON ) ? SIGNAL_LED_BUTTON_STEP_1_OFF : SIGNAL_LED_BUTTON_STEP_1_ON;
        UTIL_TIMER_SetPeriod( &signal_led_timer, signal_led_period );
        led_green_toggle();
        led_red_toggle();
        UTIL_TIMER_Start( &signal_led_timer );
      }
      else if( time_diff >= SIGNAL_LED_BUTTON_STEP_1_TIME_LIMIT && time_diff < SIGNAL_LED_BUTTON_STEP_2_TIME_LIMIT )
      {
        if( signal_led_period == SIGNAL_LED_BUTTON_STEP_2_ON || signal_led_period == SIGNAL_LED_BUTTON_STEP_2_OFF )
        {
          signal_led_period = ( signal_led_period == SIGNAL_LED_BUTTON_STEP_2_ON ) ? SIGNAL_LED_BUTTON_STEP_2_OFF : SIGNAL_LED_BUTTON_STEP_2_ON;
        }
        else
        {
          signal_led_period = SIGNAL_LED_BUTTON_STEP_2_ON;
          leds_off();
        }
        UTIL_TIMER_SetPeriod( &signal_led_timer, signal_led_period );
        led_green_toggle();
        UTIL_TIMER_Start( &signal_led_timer );
      }
      else
      {
        leds_off();
        if( signal_led_sequenz_cb != NULL )
        {
          signal_led_sequenz_cb( NULL );
        }
        signal_led_reset_values();
      }
    }
    break;
    default:
    {
    }
    break;
  }
}
