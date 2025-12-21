/**
* @file led.h
* @brief Header file for Application functions.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Definitions ---------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void leds_init( void );
void leds_deinit( void );
void leds_off( void );

void led_red_en( uint8_t en );
void led_green_en( uint8_t en );
void led_orange_en( uint8_t en );

void led_red_toggle( void );
void led_green_toggle( void );


#ifdef __cplusplus
}
#endif

/**
  * @}
  */

#endif /* __LED_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
