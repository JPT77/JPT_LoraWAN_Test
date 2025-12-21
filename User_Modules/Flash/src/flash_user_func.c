/**
* @file flash_user_func.c
* @brief Source file for user flash functions.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "flash_user_func.h"
#include "app_settings.h"
#include "eeprom.h"
#include "base.h"
#include "hw_gpio.h"
#include "hw_conf.h"

/* Definitions ---------------------------------------------------------------*/
/* Typedefs ------------------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
/* Prototypes ----------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
EE_Status flash_user_func_init( bool b_force_default )
{
  EE_Status ee_status = EE_OK;
  
  ee_status = EEPROM_emulation_init();
  
  ee_status = EEPROM_check_persist_data_init( EEPROM_EMU_DATA_INIT_VALUE, b_force_default );
  
  return ee_status;
}

EE_Status flash_user_func_reinit( void )
{
  EE_Status ee_status = EE_OK;
  
  ee_status = EEPROM_emulation_init();
  
  return ee_status;
}

EE_Status flash_user_func_reset( void )
{
  EE_Status ee_status = EE_OK;

  ee_status = EEPROM_emulation_reset();
  
  return ee_status;
}

EE_Status flash_user_func_eeprom_data_set_default( void )
{
  EE_Status ee_status = EE_OK;
  
  ee_status = app_eeprom_set_defaults();
  
  return ee_status;
}

void EEPROM_Error_Handler( void )
{
  HW_GPIO_Write( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN, GPIO_PIN_SET );
  HW_GPIO_Write( LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN, GPIO_PIN_RESET );
  
  while( 1 )
  {
    HW_GPIO_Toggle( LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN );
    HW_GPIO_Toggle( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN );
    HAL_Delay( 50 );
  }
}
