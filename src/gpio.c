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
#define	LED0_port gpioPortF
#define LED0_pin 4
#define LED1_port gpioPortF
#define LED1_pin 5
#define LCD_PORT gpioPortD
#define LCD_PIN 15
#define Extcomin_port gpioPortD
#define Extcomin_pin  13



void gpioInit()
{
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);
	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);
	button_pressed = 0;
}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
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




void Enable_switch_interrupt(void)
{

	 GPIOINT_Init();

	  GPIO_PinModeSet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, gpioModeInputPull, 1);
	  GPIO_PinModeSet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN, gpioModeInputPull, 1);

	  /* configure interrupt for PB0 and PB1, both falling and rising edges */
	  GPIO_ExtIntConfig(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, BSP_BUTTON0_PIN, true, true, true);
	  GPIO_ExtIntConfig(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN, BSP_BUTTON1_PIN, true, true, true);

	  /* register the callback function that is invoked when interrupt occurs */
	  GPIOINT_CallbackRegister(BSP_BUTTON0_PIN, callback_func);
	  GPIOINT_CallbackRegister(BSP_BUTTON1_PIN, callback_func1);


}
//Interrupt handler for push button
void callback_func()
{
	CORE_ATOMIC_IRQ_DISABLE();
    LOG_INFO("HERE");
    button_pressed = 1;
    LOG_INFO(" val %d",GPIO_PinInGet(gpioPortF, 6));
    gecko_external_signal(button_pressed);
    CORE_ATOMIC_IRQ_ENABLE();
}

void callback_func1()
{
	CORE_ATOMIC_IRQ_DISABLE();
    LOG_INFO("HERE");
    LOG_INFO(" val %d",GPIO_PinInGet(gpioPortF, 7));
    CORE_ATOMIC_IRQ_ENABLE();
}


