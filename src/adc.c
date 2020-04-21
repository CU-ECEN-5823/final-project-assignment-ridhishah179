/***********************************************************************************

* @adc.c
* @author Ridhi Shah
* This file contains functions to read ADC Value
  to a specified period and Energy mode
* @references:
************************************************************************************/
#include "adc.h"


void Sensor_Init(void)
{
	/* Enabling the ADC0 clock*/
	CMU_ClockEnable(cmuClock_ADC0, true);

  /* Base the ADC configuration on the default setup. */
  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  ADC_InitSingle_TypeDef sInit = ADC_INITSINGLE_DEFAULT;

  init.prescale = ADC_PrescaleCalc(16000000, 0); // Init to max ADC clock

  sInit.diff       = false;        // single ended
  sInit.reference  = adcRef2V5;    // internal 2.5V reference
  sInit.resolution = adcRes12Bit;  // 12-bit resolution
  sInit.acqTime    = adcAcqTime4;  // set acquisition time to meet minimum requirement

  sInit.posSel = adcPosSelAPORT2XCH9;

   ADC_Init(ADC0, &init);
   ADC_InitSingle(ADC0, &sInit);
}

uint16_t get_adc_data()
{
	/*Enabling the ADC0 interrupt*/
	 NVIC_EnableIRQ(ADC0_IRQn);
	 ADC_IntEnable(ADC0,ADC_IF_SINGLE);
	ADC_Start(ADC0, adcStartSingle);
	if(ADC_flag)

	{

		uint32_t ADC_data = ((0xFFF)^ ADC_DataSingleGet(ADC0))+1;
		sensor_data = ((ADC_data * 2500)/4096) + 2;


	}

	return sensor_data;
}

void ADC0_IRQHandler(void)
{
	ADC_flag = true;
	/*Disabling ADC0 interrupt*/
	NVIC_DisableIRQ(ADC0_IRQn);
	ADC_IntDisable(ADC0,ADC_IF_SINGLE);
}
