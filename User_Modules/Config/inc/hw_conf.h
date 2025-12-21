/**
* @file hw_config.h
* @brief Header file for LoRaWAN project configuration.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

/** @addtogroup USER_CONFIG
* @{
**/
/*---------------------------------------------------------------------------*/
#ifndef __HW_CONFIG_H__
#define __HW_CONFIG_H__

#define STATE_BUTTON_GPIO_PORT                  GPIOA               // Port of State Button
#define STATE_BUTTON_GPIO_PIN                   GPIO_PIN_0          // Pin of State Button

#define LED_GREEN_GPIO_PORT                     GPIOB               // Port of green LED
#define LED_GREEN_GPIO_PIN                      GPIO_PIN_3          // Pin of green LED
#define LED_RED_GPIO_PORT                       GPIOA               // Port of red LED
#define LED_RED_GPIO_PIN                        GPIO_PIN_15         // Pin of red LED

#define EN_BAT_VOLT_GPIO_PORT                   GPIOB               // Port of Enable Battery Voltage
#define EN_BAT_VOLT_GPIO_PIN                    GPIO_PIN_12         // Pin of Enable Battery Voltage

#define ADC_CHANNEL_BAT_VOLTAGE                 ADC_CHANNEL_1       // Channel where battery is connected (PB14)

#define USART1_BAUDRATE                         115200              // VCOM and bootloader connection
#define USART1_RX_Pin                           GPIO_PIN_10         // VCOM and bootloader connection
#define USART1_RX_GPIO_Port                     GPIOA               // VCOM and bootloader connection
#define USART1_TX_Pin                           GPIO_PIN_9          // VCOM and bootloader connection
#define USART1_TX_GPIO_Port                     GPIOA               // VCOM and bootloader connection

#define RTC_N_PREDIV_S                          10
#define RTC_PREDIV_S                            ( ( 1 << RTC_N_PREDIV_S ) - 1 )
#define RTC_PREDIV_A                            ( ( 1 << ( 15 - RTC_N_PREDIV_S ) ) - 1 )

#endif /* __HW_CONFIG_H__ */
