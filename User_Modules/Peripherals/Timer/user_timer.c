/**
* @file user_timer.c
* @brief Source file for user timer.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/** @addtogroup USER_TIMER
* @{
**/
/*---------------------------------------------------------------------------*/

// Includes --------------------------------------------------------------------
#include "main.h"
#include "user_timer.h"
#include "stm32_timer.h"
#include "sys_app.h"
#include "timer_if.h"
// Definitions -----------------------------------------------------------------
// Typedefs --------------------------------------------------------------------
// Variables -------------------------------------------------------------------
void user_timer_stop( UTIL_TIMER_Object_t *TimerObject, uint32_t *u32_remaining_time )
{
  UTIL_TIMER_GetRemainingTime( TimerObject, u32_remaining_time );
  *u32_remaining_time = TIMER_IF_Convert_Tick2ms( *u32_remaining_time );
  UTIL_TIMER_Stop( TimerObject );
}

void user_timer_restart( UTIL_TIMER_Object_t *TimerObject, uint32_t *u32_remaining_time, uint32_t u32_new_period_value )
{
  if( u32_new_period_value != 0)
  {
    if( *u32_remaining_time == 0 )
    {
      UTIL_TIMER_SetPeriod( TimerObject, u32_new_period_value );
    }
    else
    {
      UTIL_TIMER_SetPeriod( TimerObject, *u32_remaining_time );
      *u32_remaining_time = 0;
    }
    UTIL_TIMER_Start( TimerObject );
  }
  else
  {
    UTIL_TIMER_Stop( TimerObject );
  }
}
