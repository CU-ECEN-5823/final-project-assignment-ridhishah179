/***********************************************************************************

* @lettimer.h
* @author Ridhi Shah
* This file contains header files for configuring LETIMER0 and Macros to specify period and ON time of LED
* @references: https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/group__LETIMER.html

************************************************************************************/

#include <em_letimer.h>
#include <em_core.h>
#include <math.h>


#define PERIOD (3000) //Configure the desired period for the LED

 //extern uint16_t temp_event;

int period_expired;
/*
 * @brief: This function calculates and sets the values of the compare register according to the
 *        specified period and on time
 */
void set_compare_regs();


/*
 * @brief: This function configures and enables the LETIMR0 for blinking LED accorfing to the specified period and on Time
 */
void letimer_setup();

/*
 * @brief: This function handles the COMP0 and underflow interrupts and toggles LED accordingly
 */
void LETIMER0_IRQHandler(void);


void timerSetEventInMs(uint32_t ms_until_wakeup);
/*
 * @brief: This function provides a delay according to the time specified by user
 * @params : us_wait - Delay in microseconds
 */
void timerWaitUs(uint32_t us_wait);
uint32_t timerGetRunTimeMilliseconds();


;


