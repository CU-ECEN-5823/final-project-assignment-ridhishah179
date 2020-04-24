/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 *      CoAuthor : Ridhi Shah
 */
#include "gpio.h"
#include "em_gpio.h"
#include <string.h>
#include "letimer.h"
#include "gecko_ble_errors.h"
#include "display.h"
#include "gatt_db.h"
#include "native_gecko.h"
#include "gpiointerrupt.h"
/**
 * TODO: define these.  See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
 * and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
 */
#define LCD_PORT gpioPortD
#define LCD_PIN 15
#define Extcomin_port gpioPortD
#define Extcomin_pin  13
#define FIRE_SENSOR_PORT gpioPortD
#define FIRE_SENSOR_PIN 12
#define FIRE_SENSOR_VCC_PORT gpioPortD
#define FIRE_SENSOR_VCC_PIN 11


void gpioInit()
{

	GPIO_PinModeSet(FIRE_SENSOR_VCC_PORT,FIRE_SENSOR_VCC_PIN, gpioModePushPull, false);
	fire_detected = 0;
}


void gpioEnableDisplay()
{
	GPIO_PinOutSet(LCD_PORT,LCD_PIN);
}

void gpioSetDisplayExtcomin(bool high)
{
	if(high)
	{
		GPIO_PinOutSet(Extcomin_port, Extcomin_pin);
	}
	else
	{
		GPIO_PinOutClear(Extcomin_port, Extcomin_pin);
	}
}




void Enable_sensor_interrupt(void)
{
	 GPIOINT_Init();

	  GPIO_PinModeSet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, gpioModeInputPull, 1);
	  GPIO_PinModeSet(FIRE_SENSOR_PORT, FIRE_SENSOR_PIN, gpioModeInput, 1);

	  /* configure interrupt for PB0 and PB1, both falling and rising edges */
	  GPIO_ExtIntConfig(FIRE_SENSOR_PORT, FIRE_SENSOR_PIN, FIRE_SENSOR_PIN, true, true, true);
	  GPIO_ExtIntConfig(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, BSP_BUTTON0_PIN, true, true, true);

	  /* register the callback function that is invoked when interrupt occurs */
	  GPIOINT_CallbackRegister(FIRE_SENSOR_PIN, callback_func);
	  GPIOINT_CallbackRegister(BSP_BUTTON0_PIN, callback_func1);

}
//Interrupt handler for push button
void callback_func()
{
	CORE_ATOMIC_IRQ_DISABLE();
    fire_detected= 1;
    gecko_external_signal(fire_detected);
    CORE_ATOMIC_IRQ_ENABLE();
}

void callback_func1()
{
	CORE_ATOMIC_IRQ_DISABLE();
    LOG_INFO(" val %d",GPIO_PinInGet(gpioPortF, 6));
    CORE_ATOMIC_IRQ_ENABLE();
}
