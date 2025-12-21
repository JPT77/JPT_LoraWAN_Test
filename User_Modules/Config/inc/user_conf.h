/**
* @file user_config.h
* @brief Header file for LoRaWAN project configuration.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/** @addtogroup USER_CONFIG
* @{
**/
/*---------------------------------------------------------------------------*/

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

// Includes --------------------------------------------------------------------
// Definitions -----------------------------------------------------------------

// sys_conf.h
#define USER_CONF_APP_LOG_ENABLED               1
#define USER_CONF_DEBUGGER_ON                   0                                                       // 1 = enables the debbugger, 0 = the debugger is OFF (lower consumption)
#define USER_CONF_LOW_POWER_DISABLE             0                                                       // 0 = LowPowerMode enabled. MCU enters stop2 mode, 1 = LowPowerMode disabled. MCU enters sleep mode only

// Commissioning.h / se-identity.h
#define USER_CONF_STATIC_DEVICE_EUI             1                                                       // 1 = DevEui is LORAWAN_DEVICE_EUI, 0 = DevEui is automatically set with a value provided by MCU platform

#define USER_CONF_LORAWAN_DEVICE_EUI            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define USER_CONF_LORAWAN_JOIN_EUI              { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define USER_CONF_LORAWAN_APP_KEY               00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00

#define USER_VERBOSE_LEVEL                      VLEVEL_L

// Typedefs --------------------------------------------------------------------
// Variables -------------------------------------------------------------------
// Prototypes ------------------------------------------------------------------

#endif /* __USER_CONFIG_H__ */
