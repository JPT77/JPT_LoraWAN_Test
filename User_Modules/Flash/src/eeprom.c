/**
* @file eeprom.c
* @brief Source file for eeprom functions.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/*---------------------------------------------------------------------------*/

/*
 *  Includes ------------------------------------------------------------------
 */

#include "eeprom_emul.h"
#include "eeprom.h"
#include "flash_user_func.h"
#include <stdio.h>

/*
 *  Definitions ---------------------------------------------------------------
 */

/*
 *  Typedefs ------------------------------------------------------------------
 */

/*
 *  Variables -----------------------------------------------------------------
 */

__IO uint32_t ErasingOnGoing = 0;

/*
 *  Prototypes ----------------------------------------------------------------
 */

void EEPROM_Error_Handler( void );


/**
  * @brief  Checks the persist data init value to set the interval timer
  *         value after the first system start to default
  * @param[in] u32_checkvalue The value to be checked against.
  * @retval EEPRON Status
  */
EE_Status EEPROM_emulation_init( void )
{
  EE_Status ee_status = EE_OK;
  /* Unlock the Flash Program Erase controller */
  HAL_FLASH_Unlock();

  /* Clear OPTVERR bit */
  __HAL_FLASH_CLEAR_FLAG( FLASH_FLAG_OPTVERR );
  while( __HAL_FLASH_GET_FLAG( FLASH_FLAG_OPTVERR ) != RESET );
  
  /* Set EEPROM emulation firmware to erase all potentially incompletely erased
   pages if the system came from an asynchronous reset. Conditional erase is
   safe to use if all Flash operations where completed before the system reset */
  if( __HAL_PWR_GET_FLAG( PWR_FLAG_SB ) == RESET )
  {
    /* System reset comes from a power-on reset: Forced Erase */
    /* Initialize EEPROM emulation driver (mandatory) */
    ee_status = EE_Init( EE_FORCED_ERASE );
    if( ee_status != EE_OK ) { EEPROM_Error_Handler(); }
  }
  else
  {
    /* Clear the STANDBY flag */
    __HAL_PWR_CLEAR_FLAG( PWR_FLAG_SB );
    
    /* System reset comes from a Standby wakeup: Conditional Erase*/
    /* Initialize EEPROM emulation driver (mandatory) */
    ee_status = EE_Init( EE_CONDITIONAL_ERASE );
    if( ee_status != EE_OK ) { EEPROM_Error_Handler(); }
  }
  
  /* Lock the Flash Program Erase controller */
  HAL_FLASH_Lock();
  
  return( ee_status );
}

EE_Status EEPROM_emulation_reset( void )
{
  EE_Status ee_status = EE_OK;

  /* Wait any cleanup is completed before accessing flash again */
  while( ErasingOnGoing == 1 )
  {
    ;
  }

  HAL_FLASH_Unlock();
  
  /* Clear OPTVERR bit */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR) != RESET) ;
  
  ee_status = EE_Format( EE_FORCED_ERASE );
  
  /* Start cleanup polling mode, if cleanup is needed */
  if ((ee_status & EE_STATUSMASK_CLEANUP) == EE_STATUSMASK_CLEANUP) {ErasingOnGoing = 0;ee_status|= EE_CleanUp();}
  if ((ee_status & EE_STATUSMASK_ERROR) == EE_STATUSMASK_ERROR) {EEPROM_Error_Handler();}
  
  /* Lock the Flash Program Erase controller */
  HAL_FLASH_Lock();
  
  return( ee_status );
}

/**
  * @brief  Checks the persist data init value to set the interval timer
  *         value after the first system start to default
  * @param[in] u32_checkvalue The value to be checked against.
  * @retval EEPROM Status
  */
EE_Status EEPROM_check_persist_data_init( uint32_t u32_checkvalue, bool b_force_default )
{
  EE_Status ee_status = EE_OK;
  uint32_t u32_temp_data = 0;
  
  /* Wait any cleanup is completed before accessing flash again */
  while( ErasingOnGoing == 1 )
  {
    ;
  }
  
  /* Unlock the Flash Program Erase controller */
  HAL_FLASH_Unlock();
  
  /* Clear OPTVERR bit */
  __HAL_FLASH_CLEAR_FLAG( FLASH_FLAG_OPTVERR );
  while( __HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR ) != RESET )
  {
    ;
  }
  
  ee_status = EE_ReadVariable32bits( EEPROM_EMU_DATA_INIT_ADDRESS, &u32_temp_data );

  if( ( EE_OK == ee_status ) || ( EE_NO_DATA == ee_status ) )
  {
    /* Check the persist data init value */
    if( u32_checkvalue != u32_temp_data || b_force_default )
    {
      ee_status = EE_Format( EE_FORCED_ERASE );
      ee_status = EEPROM_start_cleanup_polling_mode_if_needed( ee_status );
      
      ee_status = flash_user_func_eeprom_data_set_default();      

      if( EE_OK == ee_status )
      {
        /* Set the persist data init value to defined value */
        u32_temp_data = EEPROM_EMU_DATA_INIT_VALUE;
        ee_status = EE_WriteVariable32bits( EEPROM_EMU_DATA_INIT_ADDRESS, u32_temp_data );
      
        /* Start cleanup polling mode, if cleanup is needed */
        if( ( ee_status & EE_STATUSMASK_CLEANUP ) == EE_STATUSMASK_CLEANUP )
        {
          ErasingOnGoing = 0;
          ee_status|= EE_CleanUp();
        }
        if( ( ee_status & EE_STATUSMASK_ERROR ) == EE_STATUSMASK_ERROR )
        {
          EEPROM_Error_Handler();
        }
      }
    }
  }
  else
  {
    
  }
  
  /* Lock the Flash Program Erase controller */
  HAL_FLASH_Lock();
  
  return( ee_status );
}

/**
  * @brief  Stores new data into a emulated EEPROM variable
  * @param[in] u16_virtual_address The virtual address to the variable.
  * @param[in] u32_new_data The new data value to be set.
  * @retval EEPROM Status 
  */
EE_Status EEPROM_write_ee_variable_32bits( uint16_t u16_virtual_address, uint32_t u32_new_data )
{
  EE_Status ee_status = EE_OK;
  
  /* Wait any cleanup is completed before accessing flash again */
  while (ErasingOnGoing == 1) { }
  
  /* Unlock the Flash Program Erase controller */
  HAL_FLASH_Unlock();
  
  /* Clear OPTVERR bit */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR) != RESET) ;
  
  /* Writes the new data into the variable */
  ee_status = EE_WriteVariable32bits(u16_virtual_address, u32_new_data);
  
  /* Start cleanup polling mode, if cleanup is needed */
  if ((ee_status & EE_STATUSMASK_CLEANUP) == EE_STATUSMASK_CLEANUP) {ErasingOnGoing = 0;ee_status|= EE_CleanUp();}
  if ((ee_status & EE_STATUSMASK_ERROR) == EE_STATUSMASK_ERROR) {EEPROM_Error_Handler();}
  
  /* Lock the Flash Program Erase controller */
  HAL_FLASH_Lock();
  
  return( ee_status );
}


/**
  * @brief  Stores new data into a emulated EEPROM variable
  * @param[in] u16_virtual_address The virtual address to the variable.
  * @param[in] u16_new_data The new data value to be set.
  * @retval EEPROM Status 
  */
EE_Status EEPROM_write_ee_variable_16bits( uint16_t u16_virtual_address, uint16_t u16_new_data )
{
  EE_Status ee_status = EE_OK;
  
  /* Wait any cleanup is completed before accessing flash again */
  while (ErasingOnGoing == 1) { }
  
  /* Unlock the Flash Program Erase controller */
  HAL_FLASH_Unlock();
  
  /* Clear OPTVERR bit */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR) != RESET) ;
  
  /* Writes the new data into the variable */
  ee_status = EE_WriteVariable16bits(u16_virtual_address, u16_new_data);
  
  /* Start cleanup polling mode, if cleanup is needed */
  if ((ee_status & EE_STATUSMASK_CLEANUP) == EE_STATUSMASK_CLEANUP) {ErasingOnGoing = 0;ee_status|= EE_CleanUp();}
  if ((ee_status & EE_STATUSMASK_ERROR) == EE_STATUSMASK_ERROR) {EEPROM_Error_Handler();}
  
  /* Lock the Flash Program Erase controller */
  HAL_FLASH_Lock();
  
  return( ee_status );
}


/**
  * @brief  Stores new data into a emulated EEPROM variable
  * @param[in] u16_virtual_address The virtual address to the variable.
  * @param[in] u8_new_data The new data value to be set.
  * @retval EEPROM Status 
  */
EE_Status EEPROM_write_ee_variable_8bits( uint16_t u16_virtual_address, uint8_t u8_new_data )
{
  EE_Status ee_status = EE_OK;
  
  /* Wait any cleanup is completed before accessing flash again */
  while (ErasingOnGoing == 1) { }
  
  /* Unlock the Flash Program Erase controller */
  HAL_FLASH_Unlock();
  
  /* Clear OPTVERR bit */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR) != RESET) ;
  
  /* Writes the new data into the variable */
  ee_status = EE_WriteVariable8bits(u16_virtual_address, u8_new_data);
  
  /* Start cleanup polling mode, if cleanup is needed */
  if ((ee_status & EE_STATUSMASK_CLEANUP) == EE_STATUSMASK_CLEANUP) {ErasingOnGoing = 0;ee_status|= EE_CleanUp();}
  if ((ee_status & EE_STATUSMASK_ERROR) == EE_STATUSMASK_ERROR) {EEPROM_Error_Handler();}
  
  /* Lock the Flash Program Erase controller */
  HAL_FLASH_Lock();
  
  return( ee_status );
}


/**
  * @brief  Reads data from the emulated EEPROM variable
  * @param[in] u16_virtual_address The virtual address to the variable.
  * @param[out] u32_data Pointer to the variable which gets the data.
  * @retval EEPROM Status 
  */
EE_Status EEPROM_read_ee_variable_32bits( uint16_t u16_virtual_address, uint32_t *u32_data )
{
  EE_Status ee_status = EE_OK;
  
  /* Wait any cleanup is completed before accessing flash again */
  while (ErasingOnGoing == 1) { }
  
  /* Writes the new data into the variable */
  ee_status = EE_ReadVariable32bits(u16_virtual_address, u32_data);
  if(ee_status != EE_OK) {EEPROM_Error_Handler();}
  
  return( ee_status );
}


/**
  * @brief  Reads data from the emulated EEPROM variable
  * @param[in] u16_virtual_address The virtual address to the variable.
  * @param[out] u16_data Pointer to the variable which gets the data.
  * @retval EEPROM Status 
  */
EE_Status EEPROM_read_ee_variable_16bits( uint16_t u16_virtual_address, uint16_t *u16_data )
{
  EE_Status ee_status = EE_OK;
  
  /* Wait any cleanup is completed before accessing flash again */
  while (ErasingOnGoing == 1) { }
  
  /* Writes the new data into the variable */
  ee_status = EE_ReadVariable16bits(u16_virtual_address, u16_data);
  if(ee_status != EE_OK) {EEPROM_Error_Handler();}
  
  return( ee_status );
}

/**
  * @brief  Reads data from the emulated EEPROM variable
  * @param[in] u16_virtual_address The virtual address to the variable.
  * @param[out] u8_data Pointer to the variable which gets the data.
  * @retval EEPROM Status 
  */
EE_Status EEPROM_read_ee_variable_8bits( uint16_t u16_virtual_address, uint8_t *u8_data )
{
  EE_Status ee_status = EE_OK;
  
  /* Wait any cleanup is completed before accessing flash again */
  while (ErasingOnGoing == 1) { }
  
  /* Writes the new data into the variable */
  ee_status = EE_ReadVariable8bits(u16_virtual_address, u8_data);
  if(ee_status != EE_OK) {EEPROM_Error_Handler();}
  
  return( ee_status );
}

EE_Status EEPROM_start_cleanup_polling_mode_if_needed( EE_Status ee_status )
{
  if( ( ee_status & EE_STATUSMASK_CLEANUP ) == EE_STATUSMASK_CLEANUP ) { ErasingOnGoing = 0; ee_status|= EE_CleanUp(); }
  if( ( ee_status & EE_STATUSMASK_ERROR ) == EE_STATUSMASK_ERROR ) { EEPROM_Error_Handler(); }
  
  return ee_status;
}
