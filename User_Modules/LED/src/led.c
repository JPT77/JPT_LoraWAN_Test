/**
* @file led.c
* @brief Source file for Application functions.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "led.h"
#include "hw_gpio.h"
#include "hw_conf.h"
#include <stdbool.h>

/* Definitions ---------------------------------------------------------------*/
/* Typedefs ------------------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
/* Prototypes ----------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void leds_init( void )
{
  GPIO_InitTypeDef initStruct = { 0 };
  initStruct.Mode             = GPIO_MODE_OUTPUT_PP;
  initStruct.Pull             = GPIO_NOPULL;
  initStruct.Speed            = GPIO_SPEED_FREQ_VERY_HIGH;

  HW_GPIO_Init( LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN, &initStruct );
  HW_GPIO_Init( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN, &initStruct ); 

  HW_GPIO_Write( LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN, GPIO_PIN_SET );
  HW_GPIO_Write( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN, GPIO_PIN_SET );
}

void leds_deinit( void )
{
  HW_GPIO_Deinit( LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN, 1 );
  HW_GPIO_Deinit( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN, 1 ); 
}

void leds_off( void )
{
  led_red_en( false );
  led_green_en( false );
}

void led_red_en( uint8_t en )
{
  HW_GPIO_Write( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN, !en );
}

void led_red_toggle( void )
{
  HW_GPIO_Toggle( LED_RED_GPIO_PORT, LED_RED_GPIO_PIN );
}

void led_green_en( uint8_t en )
{
  HW_GPIO_Write( LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN, !en );
}


void led_green_toggle( void )
{
  HW_GPIO_Toggle( LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN );
}

void led_orange_en( uint8_t en )
{
  led_green_en( en );
  led_red_en( en );
}
