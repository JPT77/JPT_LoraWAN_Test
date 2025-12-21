/**
* @file user_timer.h
* @brief Header file for user timer.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/** @addtogroup USER_TIMER
* @{
**/
/*---------------------------------------------------------------------------*/

#ifndef __USER_TIMER_H__
#define __USER_TIMER_H__

// Includes --------------------------------------------------------------------
#include "stm32_timer.h"

// Definitions -----------------------------------------------------------------
// Typedefs --------------------------------------------------------------------
// Variables -------------------------------------------------------------------
// Prototypes ------------------------------------------------------------------
void user_timer_stop( UTIL_TIMER_Object_t *TimerObject, uint32_t *u32_remaining_time );
void user_timer_restart( UTIL_TIMER_Object_t *TimerObject, uint32_t *u32_remaining_time, uint32_t u32_new_period_value );

#endif /* __USER_TIMER__ */
