/***********************************************************************************

* @lettimer.c
* @author Ridhi Shah
* This file contains functions to configure LETIMER0 for a specified period
  to a specified period and Energy mode
* @references: https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/group__LETIMER.html
			   https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg1/html/group__CORE.html

************************************************************************************/


#include "cmu.h"
#include "state_handler.h"



int freq = 32768;
int count_msec = 0;


//Configure LETIMER0
void letimer_setup()
{
	LETIMER_Init_TypeDef conf_letimer =
	{                                                                            \
		false,              /* Enable timer when initialization completes. */       \
		false,             /* Stop counter during debug halt. */                   \
		true,         	    /* Do not load COMP0 into CNT on underflow. */          \
		false,             /* Do not load COMP1 into COMP0 when REP0 reaches 0. */ \
		0,                 /* Idle value 0 for output 0. */                        \
		0,                 /* Idle value 0 for output 1. */                        \
		letimerUFOANone,   /* No action on underflow on output 0. */               \
		letimerUFOANone,   /* No action on underflow on output 1. */               \
		letimerRepeatFree, /* Count until stopped by SW. */                        \
		0                  /* Use default top Value. */                            \
	};
//	conf_letimer.comp0Top = true;

	LETIMER_Init(LETIMER0, &conf_letimer); //Initialize LETIMER0
	set_compare_regs(); // Call function to set compare register values
	LETIMER_IntClear(LETIMER0,LETIMER_IFC_COMP1|LETIMER_IFC_UF); // Clear the interrupt flags
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF ); //Enable Underflow and COMP1 interrupts
	NVIC->ISER[(((uint32_t)LETIMER0_IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)LETIMER0_IRQn) & 0x1FUL));
	LETIMER_Enable(LETIMER0, true); // Enable LETIMER0
	period_expired = 0;
}

void timerSetEventInMs(uint32_t ms_until_wakeup)
{
		//Enable COMP1 interrupts
		int ticks=0;
		int curr_ticks =  LETIMER_CounterGet(LETIMER0); //Get current value of the counter
		float val_in_sec = ms_until_wakeup/1000.0;
		float period = PERIOD/1000.0;
		//int diff = 0;

		if(EM==1 || EM==2)
		{
			ticks = (val_in_sec * freq);
		}

		else if(EM==3)
		{
			ticks = (val_in_sec * 1000);
		}

		int diff = curr_ticks - ticks;
		//handle rollover condition if no. of ticks required is greater than the current timer count
		if(diff<0)
		{
			if(EM ==1 || EM==2)
			{
				diff = (freq*period) + diff;
			}
			else
			{
				diff = (1000*period) + diff;
			}

		}


		LETIMER_CompareSet(LETIMER0,1,diff);
		LETIMER_IntClear(LETIMER0,LETIMER_IFC_COMP1);
		LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP1 );

}
//Function to get delay
void timerWaitUs(uint32_t us_wait)
{

	int ticks=0;
	int curr_ticks =  LETIMER_CounterGet(LETIMER0); //Get current value of the counter
	float val_in_sec = us_wait/1000000.0;
	float period = PERIOD/1000.0;


	if(EM==1 || EM==2)
	{
		ticks = (val_in_sec * freq);
	}

	else if(EM==3)
	{
		ticks = (val_in_sec * 1000);
	}


	int diff = curr_ticks - ticks;
	//handle rollover condition if no. of ticks required is greater than the current timer count
	if(diff<0)
	{
		if(EM ==1 || EM==2)
		{
			diff = (freq*period) + diff;
		}
		else
		{
			diff = (1000*period) + diff;
		}
	}

	while(LETIMER_CounterGet(LETIMER0) != diff); //wait till timer reaches desired count value

}

//Calculate and set Compare register values
 void set_compare_regs()
{
int comp0_val;
float period = PERIOD/1000.0;

//Calculate COMP0 and COMP1 values for modes lower than EM3
if (EM<3)
	{
		//int freq = 32768;
		int power = 0;

		//Calculate Clock Prescaler
		if(freq*period>65535)
		{
			while(freq*period > 65535)
			{
				freq=freq/2;
				power++;
			}

			CMU_ClockDivSet(cmuClock_LETIMER0,pow(2,power)); //Set Clock prescaler

		}

		comp0_val = freq*period;

	}

// Calculate COMP0 and COMP1 values for EM3
else
	{
		int freq = 1000;
		comp0_val = (freq*period);

	}

	//Set COMP0 and COMP1 values
	LETIMER_CompareSet(LETIMER0,0,comp0_val);


}

 uint32_t timerGetRunTimeMilliseconds()
 {
	 uint32_t time_msec;

	 time_msec = (LETIMER_CompareGet(LETIMER0,0))-(LETIMER_CounterGet(LETIMER0));
	 return ((time_msec*1000)/1000) + (count_msec * PERIOD *1000);
 }

//Handle COMP1 and UNDERFLOW interrupts to blink LED accordingly
void LETIMER0_IRQHandler(void)
{
	CORE_ATOMIC_IRQ_DISABLE(); //Disable Global interrupts

	int letimer_flags = LETIMER_IntGetEnabled(LETIMER0); //Read Interrupt flag register

	//Set event on underflow interrupt
	 if(letimer_flags & LETIMER_IF_UF)
	{
		// gecko_external_signal(period_expired);
		//occured_event = period_expired;
		period_expired = 1;
		count_msec = count_msec + 1;
		LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF);
		gecko_external_signal(period_expired);

	}

	 CORE_ATOMIC_IRQ_ENABLE(); //Enable global interrupts
}
