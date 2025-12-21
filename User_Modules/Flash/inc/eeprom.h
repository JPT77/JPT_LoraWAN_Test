/**
* @file eeprom.h
* @brief Header file for eeprom functions.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/** @addtogroup EEPROM
* @{
**/
/*---------------------------------------------------------------------------*/

#ifndef __EEPROM_H__
#define __EEPROM_H__

/*
 *  Includes ------------------------------------------------------------------
 */
#include "eeprom_emul.h"

/*
 *  Definitions ---------------------------------------------------------------
 */

/*
 *  Typedefs ------------------------------------------------------------------
 */

/*
 *  Variables ----------------------------------------------------------------
 */

/*
 *  Prototypes ----------------------------------------------------------------
 */
extern void EEPROM_Error_Handler( void );

EE_Status EEPROM_emulation_init( void );
EE_Status EEPROM_emulation_reset( void );
EE_Status EEPROM_check_persist_data_init( uint32_t u32_checkvalue, bool b_force_default );

EE_Status EEPROM_write_ee_variable_32bits( uint16_t u16_virtual_address, uint32_t u32_new_data );
EE_Status EEPROM_write_ee_variable_16bits( uint16_t u16_virtual_address, uint16_t u16_new_data );
EE_Status EEPROM_write_ee_variable_8bits( uint16_t u16_virtual_address, uint8_t u8_new_data );
EE_Status EEPROM_read_ee_variable_32bits( uint16_t u16_virtual_address, uint32_t *u32_data );
EE_Status EEPROM_read_ee_variable_16bits( uint16_t u16_virtual_address, uint16_t *u16_data );
EE_Status EEPROM_read_ee_variable_8bits( uint16_t u16_virtual_address, uint8_t *u8_data );

EE_Status EEPROM_start_cleanup_polling_mode_if_needed( EE_Status ee_status );

#endif /* __EEPROM_H__ */
