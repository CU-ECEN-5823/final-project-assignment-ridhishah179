

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


void Sensor_Init(void);


uint16_t get_adc_data();


bool ADC_flag;

uint16_t sensor_data;

#endif /* SRC_ADC_H_ */
