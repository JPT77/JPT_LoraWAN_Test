/**
* @file base.c
* @brief Source file for board configuration.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/** @addtogroup BASE
* @{
**/
/*---------------------------------------------------------------------------*/

// Includes --------------------------------------------------------------------
#include "main.h"
#include "base.h"
#include "hw_gpio.h"
#include "stm32_systime.h"
#include "stm32_seq.h"
#include "utilities_def.h"
#include "usart.h"
#include <stdio.h>
#include "adc_if.h"
#include "lora_info.h"
#include "secure-element.h"
#include "stm32_lpm.h"
#include "hw_conf.h"
#include "led.h"
#include "flash_user_func.h"
#include "base_signal_led.h"

// Definitions -----------------------------------------------------------------
// Typedefs --------------------------------------------------------------------
// Variables -------------------------------------------------------------------
static uint8_t AppDataBuffer[LORAWAN_APP_DATA_BUFFER_MAX_SIZE];
LmHandlerAppData_t base_app_data = { 0, 0, AppDataBuffer };

static UTIL_TIMER_Object_t debounce_timer;
static LmHandlerMsgTypes_t lora_msg_type            = LORAWAN_DEFAULT_CONFIRMED_MSG_STATE;
static base_tx_reason_t u8_base_tx_reason           = TX_REASON_UNDEFINED_EVENT;
static ActivationType_t ActivationType              = LORAWAN_DEFAULT_ACTIVATION_TYPE;
static base_callbacks_t base_cb;

LmHandlerCallbacks_t LmHandlerCallbacks =
{
  .GetBatteryLevel  = NULL,
  .GetTemperature   = NULL,
  .OnMacProcess     = base_on_mac_process_notify,
  .OnJoinRequest    = base_on_join_request,
  .OnTxData         = base_on_tx_data,
  .OnRxData         = base_on_rx_data
};

LmHandlerParams_t LmHandlerParams =
{
  .ActiveRegion     = LORAWAN_ACTIVE_REGION,
  .DefaultClass     = LORAWAN_DEFAULT_CLASS,
  .AdrEnable        = LORAWAN_DEFAULT_ADR_STATE,
  .TxDatarate       = LORAWAN_DEFAULT_DATA_RATE,
  .PingPeriodicity  = LORAWAN_DEFAULT_PING_SLOT_PERIODICITY
};

static uint16_t bounce_state    = 0;
static SysTime_t low_timestamp  = { .Seconds = 0, .SubSeconds = 0 };
static SysTime_t high_timestamp = { .Seconds = 0, .SubSeconds = 0 };

static bool b_join_state              = false;
static bool b_is_por                  = false;
static uint8_t u8_join_attempts       = BASE_JOIN_ATTEMPTS;
static bool b_is_powermodule_present  = false;

// Prototypes ------------------------------------------------------------------

void base_print_bl_and_app_version( uint8_t *app, uint8_t *version )
{  
  uint8_t bl_version[3];
  memcpy( bl_version, ( uint32_t * )BASE_BL_ADDR, 3 );

#if defined( APP_LOG_ENABLED ) && ( APP_LOG_ENABLED == 0 )
  UTIL_ADV_TRACE_Init();
#endif
  APP_PPRINTF( "ELV-BM-TRX1_BL_VERSION: v%u.%u.%u\r\n", bl_version[0], bl_version[1], bl_version[2] );
  APP_PPRINTF( "%s_APP_VERSION: v%s\r\n", app, version );
#if defined( APP_LOG_ENABLED ) && ( APP_LOG_ENABLED == 0 )
  UTIL_ADV_TRACE_DeInit();
#endif
}

void base_init( base_callbacks_t *base_callbacks, bool b_adr_enable, int8_t i8_tx_datarate )
{
  UTIL_MEM_cpy_8( ( void * )&base_cb, ( const void * )base_callbacks, sizeof( base_callbacks_t ) );
  
  UTIL_SEQ_RegTask( ( 1 << CFG_SEQ_Task_LmHandler_process_task ), UTIL_SEQ_RFU, LmHandlerProcess );
  UTIL_SEQ_RegTask( ( 1 << CFG_SEQ_Task_initial_join_retry_task ), UTIL_SEQ_RFU, base_join_retry );

  base_set_is_por( true );
  base_set_join_attempts( BASE_JOIN_ATTEMPTS );
  
  LmHandlerParams.AdrEnable   = b_adr_enable;
  LmHandlerParams.TxDatarate  = i8_tx_datarate;

  base_power_module_detection();
  base_init_lorawan();
  base_set_lorawan_euis_and_key();
  signal_led_init();
  base_init_periphs();
  base_init_timer();
}

void base_init_periphs( void )
{
  GPIO_InitTypeDef initStruct = { 0 };

  initStruct.Mode   = GPIO_MODE_INPUT;//GPIO_MODE_IT_RISING_FALLING;
  initStruct.Pull   = GPIO_PULLUP;
  initStruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;

  HW_GPIO_Init( STATE_BUTTON_GPIO_PORT, STATE_BUTTON_GPIO_PIN, &initStruct );
}

void base_init_timer( void )
{
  UTIL_TIMER_Create( &debounce_timer, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, base_debounce_button, NULL );
  UTIL_TIMER_SetPeriod( &debounce_timer, BASE_BUTTON_DEBOUNCE_PERIOD );
}

void base_init_lorawan( void )
{
  LoraInfo_Init();
  LmHandlerInit( &LmHandlerCallbacks );
  LmHandlerConfigure( &LmHandlerParams );
}

void base_join( void )
{
  signal_led_start( SIGNAL_LED_ID_JOIN_PROCESS, NULL );
  LmHandlerJoin( ActivationType );
}

LmHandlerErrorStatus_t base_tx( UTIL_TIMER_Time_t *next_tx_in )
{
  LmHandlerErrorStatus_t ret = LmHandlerSend( &base_app_data, lora_msg_type, next_tx_in, false );
  if ( ret == LORAMAC_HANDLER_SUCCESS )
  {
//    APP_LOG( TS_OFF, VLEVEL_L, "SEND REQUEST\r\n" );
  }
  else if( ret == LORAMAC_HANDLER_NO_NETWORK_JOINED )
  {
//    APP_LOG( TS_OFF, VLEVEL_L, "No network joined. Join...\r\n" );
  }
  else if( ret == LORAMAC_HANDLER_DUTYCYCLE_RESTRICTED )
  {
//    APP_LOG( TS_OFF, VLEVEL_L, "Next Tx in  : ~%d second(s)\r\n", ( *next_tx_in / 1000 ) );
  }
  else
  {
//    APP_LOG( TS_OFF, VLEVEL_L, "LmHandlerErrorStatus_t: %i\r\n", ret );
  }
  
  return ret;
}

LmHandlerAppData_t* base_get_app_data_ptr( void )
{
  return &base_app_data;
}

void base_join_ok_cb( void *context )
{
  if( base_get_is_por() )     // Attempt Join whene coming from POR
  {
    UTIL_SEQ_SetTask( ( 1 << CFG_SEQ_Task_initial_join_retry_task ), CFG_SEQ_Prio_0 );
  }
}

void base_join_nok_cb( void *context )
{
  if( base_get_is_por() )     // Attempt Join whene coming from POR
  {
    UTIL_SEQ_SetTask( ( 1 << CFG_SEQ_Task_initial_join_retry_task ), CFG_SEQ_Prio_0 );
  }
}

void base_init_en_adc_supply( void )
{
  GPIO_InitTypeDef initStruct = {0};

  initStruct.Mode   = GPIO_MODE_OUTPUT_PP;
  initStruct.Pull   = GPIO_NOPULL;
  initStruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;
  
  HW_GPIO_Init( EN_BAT_VOLT_GPIO_PORT, EN_BAT_VOLT_GPIO_PIN, &initStruct );
  HW_GPIO_Write( EN_BAT_VOLT_GPIO_PORT, EN_BAT_VOLT_GPIO_PIN, GPIO_PIN_RESET );
}

void base_deinit_en_adc_supply( void )
{
  HW_GPIO_Deinit( EN_BAT_VOLT_GPIO_PORT, EN_BAT_VOLT_GPIO_PIN, GPIO_PIN_RESET );  
}

void base_en_adc_supply( bool enable )
{
  if( enable )
  {
    HW_GPIO_Write( EN_BAT_VOLT_GPIO_PORT, EN_BAT_VOLT_GPIO_PIN, GPIO_PIN_SET );
  }
  else
  {
    HW_GPIO_Write( EN_BAT_VOLT_GPIO_PORT, EN_BAT_VOLT_GPIO_PIN, GPIO_PIN_RESET );
  }
}

void base_power_module_detection( void )
{
  if( adc_get_channel_level( ADC_CHANNEL_BAT_VOLTAGE ) < POWER_MODULE_PRESENT_THRESHOLD_LOW )
  {
    base_init_en_adc_supply();
    base_en_adc_supply( true );
    HAL_Delay( 10 );

    if( adc_get_channel_level( ADC_CHANNEL_BAT_VOLTAGE ) > POWER_MODULE_PRESENT_THRESHOLD_HIGH )
    {
      base_set_is_pm_present( true );
    }

    base_deinit_en_adc_supply();
  }
}

uint16_t base_get_supply_level( void )
{
  uint16_t bat = 0;

  if( base_get_is_pm_present() )
  {
    base_init_en_adc_supply();
    base_en_adc_supply( true );
    HAL_Delay( 10 );
    bat = 2 * adc_get_channel_level( ADC_CHANNEL_BAT_VOLTAGE );
    base_deinit_en_adc_supply();
  }
  else
  {
    bat = adc_get_supply_level();
  }
  
  return bat;
}

void base_state_button_cb( void *context )
{  
  base_disable_irqs();
  base_cb.base_disable_irqs();
  bounce_state = 1;
  UTIL_TIMER_Start( &debounce_timer );
}

void base_debounce_button( void *context )
{
  bounce_state = ( bounce_state << 1 ) | HW_GPIO_Read( STATE_BUTTON_GPIO_PORT, STATE_BUTTON_GPIO_PIN ) | 0xe000;
  if( bounce_state == 0xF000 )          // Button is LOW
  {
    // Set LOW Timestamp here
    low_timestamp = SysTimeGet();

    signal_led_start( SIGNAL_LED_ID_BUTTON_PROCESS, NULL );  
    
    HW_GPIO_enable_irq( STATE_BUTTON_GPIO_PIN );
  }
  else if( bounce_state == 0xFFFF )     // Button is HIGH
  {
    // Set HIGH Timestamp here, but only if LOW was set before
    if( low_timestamp.Seconds != 0 || low_timestamp.SubSeconds != 0 )
    {
      high_timestamp = SysTimeGet();

      signal_led_stop( SIGNAL_LED_ID_BUTTON_PROCESS, false );
      
      SysTime_t div_timestamp = SysTimeSub( high_timestamp, low_timestamp );
      uint32_t time_diff = ( div_timestamp.Seconds * 1000 ) + div_timestamp.SubSeconds;
      // Schedule TX
      if( time_diff < BASE_BUTTON_ACTION_VALID_TIME_1 )
      {
        base_cb.base_user_button_event();
      }
      // SoftReset
      else if ( time_diff >= BASE_BUTTON_ACTION_VALID_TIME_1 && time_diff < BASE_BUTTON_ACTION_VALID_TIME_2 )
      {
        flash_user_func_reset();
        HAL_NVIC_SystemReset();
      }
      else
      {
        base_enable_irqs();
        base_cb.base_enable_irqs();
      }
    }
    else
    {
      base_enable_irqs();
      base_cb.base_enable_irqs();
    }

    base_reset_timestamps();
  }
  else
  {
    UTIL_TIMER_Start( &debounce_timer );
  }
}

void base_reset_timestamps( void )
{
  low_timestamp.Seconds     = 0;
  low_timestamp.SubSeconds  = 0;
  high_timestamp.Seconds    = 0;
  high_timestamp.SubSeconds = 0;
}

void base_on_tx_data( LmHandlerTxParams_t *params )
{
  if( ( params != NULL ) && ( params->IsMcpsConfirm != 0 ) )
  {
    if( params->MsgType == LORAMAC_HANDLER_CONFIRMED_MSG )
    {
      if( params->AckReceived != 0 )
      {
      }
      else
      {
      }
    }
    else
    {
    }
  }
}

void base_on_join_request( LmHandlerJoinParams_t *join_params )
{
  if( join_params != NULL )
  {
    signal_led_stop( SIGNAL_LED_ID_JOIN_PROCESS, false );
    if( join_params->Status == LORAMAC_HANDLER_SUCCESS )
    {
      base_set_is_joined( true );
      if( join_params->Mode == ACTIVATION_TYPE_ABP )
      {
        // ABP
      }
      else
      {
        // OTAA
      }
      if( base_get_is_por() )
      {
        base_set_join_attempts( 0 );
        signal_led_start( SIGNAL_LED_ID_JOIN_OK, base_join_ok_cb );
      }
    }
    else
    {
      base_set_is_joined( false );
      if( base_get_is_por() )
      {
        base_set_join_attempts( base_get_join_attempts() - 1 );
        signal_led_start( SIGNAL_LED_ID_JOIN_NOK, base_join_nok_cb );
      }
      // JOIN FAILED
    }
  }
}

void base_on_rx_data( LmHandlerAppData_t *app_data, LmHandlerRxParams_t *params )
{
  // OnRxData will be called after OnTxData
  base_reset_timestamps();
  if( ( app_data != NULL ) && ( params != NULL ) )
  {
    base_cb.base_process_downlink( app_data->Port, app_data->Buffer, app_data->BufferSize );
  }
}

void base_on_mac_process_notify( void )
{
  UTIL_SEQ_SetTask( ( 1 << CFG_SEQ_Task_LmHandler_process_task ), CFG_SEQ_Prio_0 );
}

void base_join_retry( void )
{
  if( base_get_join_attempts() > 0 )
  {
    signal_led_start( SIGNAL_LED_ID_JOIN_PROCESS, NULL );

    LmHandlerJoin( ActivationType );
  }
  else
  {
    if( base_get_is_por() )
    {
      base_set_is_por( false );
      
      GPIO_InitTypeDef initStruct = { 0 };

      initStruct.Mode   = GPIO_MODE_IT_RISING_FALLING;
      initStruct.Pull   = GPIO_PULLUP;
      initStruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;

      HW_GPIO_Init( STATE_BUTTON_GPIO_PORT, STATE_BUTTON_GPIO_PIN, &initStruct );
      HW_GPIO_SetIrq( STATE_BUTTON_GPIO_PORT, STATE_BUTTON_GPIO_PIN, 0, base_state_button_cb );

      base_cb.base_post_join();
    }
  }
}

void base_set_lorawan_euis_and_key( void )
{
  uint8_t dev_eui[8];
  uint8_t app_eui[8];
  uint8_t app_key[16];

  memcpy( dev_eui, ( uint32_t * )BASE_DEVEUI_ADDR, 8 );
  memcpy( app_eui, ( uint32_t * )BASE_APPEUI_ADDR, 8 );
  memcpy( app_key, ( uint32_t * )BASE_APPKEY_ADDR, 16 );

  if( SecureElementSetDevEui( dev_eui ) != SECURE_ELEMENT_SUCCESS )
  {
    // error
    while( 1 )
    {
      ;
    }
  }
  if( SecureElementSetJoinEui( app_eui ) != SECURE_ELEMENT_SUCCESS )
  {
    // error
    while( 1 )
    {
      ;
    }
  }
  if( SecureElementSetKey( APP_KEY, app_key ) != SECURE_ELEMENT_SUCCESS )
  {
    // error
    while( 1 )
    {
      ;
    }
  }
  if( SecureElementSetKey( NWK_KEY, app_key ) != SECURE_ELEMENT_SUCCESS )
  {
    // error
    while( 1 )
    {
      ;
    }
  }
}

void base_set_is_por( bool b_value )
{
  b_is_por = b_value;
}

bool base_get_is_por( void )
{
  return b_is_por;
}

void base_set_join_attempts( uint8_t u8_value )
{
  u8_join_attempts = u8_value;
}

uint8_t base_get_join_attempts( void )
{
  return u8_join_attempts;
}

void base_set_is_joined( bool b_value )
{
  b_join_state = b_value;
}

bool base_get_is_joined( void )
{
  return b_join_state;
}

void base_set_is_pm_present( bool b_value )
{
  b_is_powermodule_present = b_value;
}

bool base_get_is_pm_present( void )
{
  return b_is_powermodule_present;
}

void base_set_tx_reason( base_tx_reason_t u8_tx_reason )
{
  u8_base_tx_reason = u8_tx_reason;
}

base_tx_reason_t base_get_tx_reason( void )
{
  return u8_base_tx_reason;
}

void base_set_lora_msg_type( LmHandlerMsgTypes_t msg_type )
{
  lora_msg_type = msg_type;
}

void base_enable_irqs( void )
{
  HW_GPIO_enable_irq( STATE_BUTTON_GPIO_PIN );
}

void base_disable_irqs( void )
{
  HW_GPIO_disable_irq( STATE_BUTTON_GPIO_PIN );
}
