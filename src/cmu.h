/***********************************************************************************

* @cmu.h
* @author Ridhi Shah
* This file contains Header files for CMU and macros to specify Energy Mode
* @references: https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg1/html/group__CMU.html

************************************************************************************/

#include <em_cmu.h>

#define EM (2) //Configure the desired Energy Mode (EM0, EM1, EM2, EM3)

/*
 * @brief: This function configures the Clock management unit according to the energy mode specified
 */

void cmu_setup( );


