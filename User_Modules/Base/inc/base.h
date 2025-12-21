/**
* @file base.h
* @brief Header file for board configuration.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/** @addtogroup BASE
* @{
**/
/*---------------------------------------------------------------------------*/

#ifndef __BASE_H__
#define __BASE_H__

// Includes --------------------------------------------------------------------
#include "stm32wle5xx.h"
#include "stm32wlxx_hal_gpio.h"
#include "stm32wlxx_hal_rcc.h"
#include "stm32wlxx_hal_adc.h"
   
#include <stdbool.h>
#include "stm32_timer.h"
#include "LmHandler.h"
#include "LmHandlerTypes.h"
#include "Region.h"

// Definitions -----------------------------------------------------------------
#define BASE_BL_ADDR                                0x080007D0
#define BASE_DEVEUI_ADDR                            0x080007E0
#define BASE_APPEUI_ADDR                            0x080007E8
#define BASE_APPKEY_ADDR                            0x080007F0

#define LORAWAN_DEFAULT_CONFIRMED_MSG_STATE         LORAMAC_HANDLER_UNCONFIRMED_MSG   // LoRaWAN default confirm state
#define LORAWAN_DEFAULT_ADR_STATE                   LORAMAC_HANDLER_ADR_ON            // LoRaWAN Adaptive Data Rate. Please note that when ADR is enabled the end-device should be static.
#define LORAWAN_DEFAULT_DATA_RATE                   DR_0                              // LoRaWAN Default data Rate Data Rate. Please note that LORAWAN_DEFAULT_DATA_RATE is used only when LORAWAN_ADR_STATE is disabled.
#define LORAWAN_ACTIVE_REGION                       LORAMAC_REGION_EU868              // LoraWAN application configuration (Mw is configured by lorawan_conf.h).
#define LORAWAN_DEFAULT_CLASS                       CLASS_A                           // LoRaWAN default endNode class port.
#define LORAWAN_APP_DATA_BUFFER_MAX_SIZE            51                                // User application data buffer size.
#define LORAWAN_DEFAULT_PING_SLOT_PERIODICITY       4                                 // Default Unicast ping slots periodicity.
                                                                                      // Periodicity is equal to 2^LORAWAN_DEFAULT_PING_SLOT_PERIODICITY seconds.
                                                                                      // Example: 2^3 = 8 seconds. The end-device will open an Rx slot every 8 seconds.

#define BASE_HEADER_LENGTH                          5
#define BASE_DUTYCYCLE_CORRECTION_MS                5000
#define BASE_JOIN_ATTEMPTS                          3
#define LORAWAN_DEFAULT_ACTIVATION_TYPE             ACTIVATION_TYPE_OTAA              // LoRaWAN default activation type

#define POWER_MODULE_PRESENT_THRESHOLD_LOW          50
#define POWER_MODULE_PRESENT_THRESHOLD_HIGH         500

#define BASE_BUTTON_DEBOUNCE_PERIOD                 5
#define BASE_BUTTON_ACTION_VALID_TIME_1             5000
#define BASE_BUTTON_ACTION_VALID_TIME_2             8000
// Typedefs --------------------------------------------------------------------
typedef struct base_callbacks_s
{
  void ( *base_post_join )( void );
  void ( *base_process_downlink )( uint8_t port, uint8_t *buffer, uint8_t buffer_size );
  void ( *base_user_button_event )( void );
  void ( *base_enable_irqs )( void );
  void ( *base_disable_irqs )( void );
} base_callbacks_t;

typedef enum
{
  TX_REASON_TIMER_EVENT = 0,
  TX_REASON_USER_BUTTON_EVENT,
  TX_REASON_INPUT_EVENT,
  TX_REASON_FUOTA_EVENT,
  TX_REASON_APP_CYCLE_EVENT,
  TX_REASON_TIMEOUT_EVENT,
  
  TX_REASON_UNDEFINED_EVENT = 0xFF
} base_tx_reason_t;

// Variables -------------------------------------------------------------------
// Prototypes ------------------------------------------------------------------
void base_print_bl_and_app_version( uint8_t *app, uint8_t *version );
void base_init( base_callbacks_t *base_callbacks, bool b_adr_enable, int8_t i8_tx_datarate );
void base_init_periphs( void );
void base_init_timer( void );
void base_init_lorawan( void );
void base_join( void );
LmHandlerErrorStatus_t base_tx( UTIL_TIMER_Time_t *next_tx_in );
LmHandlerAppData_t* base_get_app_data_ptr( void );
void base_join_ok_cb( void *context );
void base_join_nok_cb( void *context );

void base_init_en_adc_supply( void );
void base_deinit_en_adc_supply( void );
void base_en_adc_supply( bool enable );
void base_power_module_detection( void );
uint16_t base_get_supply_level( void );

void base_state_button_cb( void *context );
void base_debounce_button( void *context );
void base_reset_timestamps( void );

void base_on_join_request( LmHandlerJoinParams_t *join_params );
void base_on_tx_data( LmHandlerTxParams_t *params );
void base_on_rx_data( LmHandlerAppData_t *app_data, LmHandlerRxParams_t *params );
void base_on_mac_process_notify( void );
void base_join_retry( void );

void base_set_lorawan_euis_and_key( void );

void base_set_is_por( bool b_value );
bool base_get_is_por( void );
void base_set_join_attempts( uint8_t u8_value );
uint8_t base_get_join_attempts( void );
void base_set_is_joined( bool b_value );
bool base_get_is_joined( void );
void base_set_is_pm_present( bool b_value );
bool base_get_is_pm_present( void );

void base_set_tx_reason( base_tx_reason_t u8_tx_reason );
base_tx_reason_t base_get_tx_reason( void );
void base_set_lora_msg_type( LmHandlerMsgTypes_t msg_type );
void base_enable_irqs( void );
void base_disable_irqs( void );

#endif /* __BASE__ */
