/**
 * @file hdc2080_low_level.c
 * @brief HDC2080 low level functions.
 */

#include "hdc2080_low_level.h"

#include "hdc2080.h"
#include "i2c.h"
#include "hw_gpio.h"

#define HDC2080_DRDY_Pin        GPIO_PIN_13
#define HDC2080_DRDY_GPIO_Port  GPIOC

static bool gpio_int_2 = false;
static GPIO_InitTypeDef initStruct = { 0 };

static void get_drdy( bool *init );
static void en_interrupt( void );
static void get_gpio_int_2( void *context );

/*!
 * @brief Initialize low level (board specifics) of HDC2080.
 */
void hdc2080_init_low_level( void )
{
  i2c_mod2_init(); // Init the I2C Hardware

  hdc2080_low_level_functions_TypeDef low_level_funcptr;
  low_level_funcptr.delay_ms    = HAL_Delay;
  low_level_funcptr.drdy        = get_drdy;
  low_level_funcptr.en_drdy     = hdc2080_gpio_drdy_enable;
  low_level_funcptr.en_int      = en_interrupt;
  low_level_funcptr.i2c_read    = i2c_mod2_read_register;
  low_level_funcptr.i2c_write   = i2c_mod2_write_register;

  hdc2080_set_low_level_functions( low_level_funcptr );
}

/*!
 * @brief Deinitialize low level (board specifics) of HDC2080.
 */
void hdc2080_deinit_low_level( void )
{
  i2c_mod2_deinit();
  HW_GPIO_Deinit( HDC2080_DRDY_GPIO_Port, HDC2080_DRDY_Pin, 0 );
}

/*!
 * @brief Enable data ready pin.
 */
void hdc2080_gpio_drdy_enable( void )
{  
  initStruct.Mode   = GPIO_MODE_IT_FALLING;
  initStruct.Pull   = GPIO_PULLUP;
  initStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  
  HW_GPIO_Init( HDC2080_DRDY_GPIO_Port, HDC2080_DRDY_Pin, &initStruct );
  HW_GPIO_SetIrq( HDC2080_DRDY_GPIO_Port, HDC2080_DRDY_Pin, 10, get_gpio_int_2 );
  
}

/*!
 * @brief Disable data ready pin.
 */
void hdc2080_gpio_drdy_disable( void )
{
  HW_GPIO_Deinit( HDC2080_DRDY_GPIO_Port, HDC2080_DRDY_Pin, 0 );
}

/*!
 * @brief Wait for valid data indicated by DRDY.
 */
static void get_drdy( bool *init )
{
  if( *init )
  {
      // Don`t replace this delay with lp_delay_ms.
      HAL_Delay( 2 );
      *init = false;
  }
  else
  {
      // Enter STOP2 Mode
  }

  while ( !gpio_int_2 )
		;
  gpio_int_2 = false;
}

static void en_interrupt( void )
{
  /* EXTI interrupt init */
  HW_GPIO_disable_irq( HDC2080_DRDY_Pin );
  HW_GPIO_SetIrq( HDC2080_DRDY_GPIO_Port, HDC2080_DRDY_Pin, 10, get_gpio_int_2 );
}

static void get_gpio_int_2( void *context )
{
  gpio_int_2 = true;
}
