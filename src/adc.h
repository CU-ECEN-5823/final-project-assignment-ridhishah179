
/***********************************************************************************

* @adc.c
* @author Ridhi Shah
* This file contains functions to read ADC Value
  to a specified period and Energy mode
* @references:https://github.com/SiliconLabs/peripheral_examples/tree/master/series0/adc/adc_single_interrupt/SimplicityStudio
************************************************************************************/

#ifndef SRC_ADC_H_
#define SRC_ADC_H_

#include "em_gpio.h"
#include "log.h"

#include "em_device.h"
#include "em_adc.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "native_gecko.h"
#include "gpio.h"

/*******************************************************************************
 * Initialize ADC Features
 ******************************************************************************/
void Sensor_Init(void);

/*******************************************************************************
 * Function to get Data from ADC
 ******************************************************************************/
uint16_t get_adc_data();

/*******************************************************************************
 * Flag for indicating ADC transfer is complete
 ******************************************************************************/
bool ADC_flag;

/*******************************************************************************
 * Variable to store ADC Data
 ******************************************************************************/
uint16_t sensor_data;

#endif /* SRC_ADC_H_ */
