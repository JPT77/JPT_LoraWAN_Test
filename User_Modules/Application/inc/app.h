/**
* @file app.h
* @brief Header file for Application functions.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_H
#define __APP_H

#ifdef __cplusplus
 extern "C" {
#endif

// Includes --------------------------------------------------------------------
#include <stdbool.h>
// Definitions -----------------------------------------------------------------
#define APP_APPLICATION_NAME_STR                "ELV-BM-TRX1 JPT Lora Test"
#define APP_APPLICATION_VERSION_STR             "1.0.0"
#define APP_LORAWAN_PORT                        10                              // LoRaWAN User application port. Do not use 224. It is reserved for certification.
#define APP_LORAWAN_ADR_STATE                   LORAMAC_HANDLER_ADR_ON          // LoRaWAN Adaptive Data Rate. Please note that when ADR is enabled the end-device should be static.
#define APP_LORAWAN_DATA_RATE                   DR_0                            // LoRaWAN Default data Rate Data Rate. Please note that LORAWAN_DEFAULT_DATA_RATE is used only when LORAWAN_ADR_STATE is disabled.
#define APP_LORAMAC_CHECK_BUSY_INTERVAL         200

// Exported types --------------------------------------------------------------
// Exported macro --------------------------------------------------------------
// Exported functions ----------------------------------------------------------
void app_init( void );

void app_send_tx_data_cb( void );
void app_set_lorawan_payload( void );
void app_check_loramac_cb( void *context );

void app_post_join( void );
void app_post_loramac_busy( void );
void app_process_downlink( uint8_t port, uint8_t *buffer, uint8_t buffer_size );

void app_user_button_event( void );
void app_cyclic_event( void );

void app_on_tx_timer_event_cb( void *context );

void app_set_dutycycle( uint32_t value );
uint32_t app_get_dutycycle( void );

void app_exit_stop_mode( void );
void app_enable_irqs( void );
void app_disable_irqs( void );

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

#endif /* __APP_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
