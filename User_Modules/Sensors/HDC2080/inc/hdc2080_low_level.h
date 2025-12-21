/**
 * @file hdc2080_low_level.h
 * @author Marcel Maas
 * @date 14. February 2019
 * @brief HDC2080 low level functions.
 */

#ifndef HDC2080_LOW_LEVEL_H_
#define HDC2080_LOW_LEVEL_H_

#include "main.h"

void hdc2080_init_low_level(void);
void hdc2080_deinit_low_level(void);

void hdc2080_gpio_drdy_enable(void);
void hdc2080_gpio_drdy_disable(void);

#endif /* HDC2080_LOW_LEVEL_H_ */
