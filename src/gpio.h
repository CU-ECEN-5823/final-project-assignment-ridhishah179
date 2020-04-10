
/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 *      CoAuthor: Ridhi Shah
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>

#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED (1)
#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED (1)
int button_pressed;
void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void gpioEnableDisplay();
void gpioSetDisplayExtcomin(bool high);
void button_init();
void Enable_switch_interrupt();
void callback_func1();
void callback_func();


#endif /* SRC_GPIO_H_ */
