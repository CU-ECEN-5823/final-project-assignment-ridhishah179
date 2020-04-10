/***********************************************************************************

* @cmu.c
* @author: Ridhi Shah
* This file contains functions to configure CMU according to a specified period and Energy mode
* @references: https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg1/html/group__CMU.html

************************************************************************************/
#include "cmu.h"

void cmu_setup( )
{
	//Configure CMU for modes lower than EM3
    if(EM < 3)
    {
	CMU_OscillatorEnable(cmuOsc_LFXO,true,true);		//Enable CMU osscillator for LFXO
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); 	//Select clock Source

    }

    //Configure CMU for EM3
	else
	{
	CMU_OscillatorEnable(cmuOsc_ULFRCO, true,true);  //Enable CMU osscillator for ULFRCO
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); //Select clock Source
	CMU_ClockDivSet(cmuClock_LETIMER0,cmuClkDiv_1); //Set Prescaler
	}


	//Enable Clock
	CMU_ClockEnable(cmuClock_LFA,true);
	CMU_ClockEnable(cmuClock_LETIMER0,true);

}
