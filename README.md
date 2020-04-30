# Envrionment Monitoring and Control

##### Group Members : Ridhi Shah, Vatsal Sheth, Amreeta Sengupta

##    Final Project Status Summary

   - The bluetooth Mesh node has been initialized for LPN functionality and to establish friendship with the friend node. Software timer has been configured for re-establishing friendship with friend nodes in case of failure. 
   - Fire Sensor has been interfaced and Digital output pin is connected to pin 12 of the gpioPortD  and Analog output has been connected to pin 9 of gpioPortC which is Analog channel 9 port. 
   - GPIO interrupt was configured and the potentiometer value on the fire sensor was set such as to define an extreme value to a given environmental situation and an interrupt is raised when the signal exceeds this extreme value. 
   - For monitoring the continuous values on the fire sensor the analog output pin is used. ADC configuration has been done for 12-bit ADC with 2.5 volts internal reference to read these analog values from the fire sensor. 
   - Command table for BLE communication using generic ON/OFF and level models has been implemented. 
   - Set up is done for the Generic ON/OFF model. The Alert message when extreme value is crossed has been sent to the friend using Generic ON/OFF model. 
   - Generic Level Model has also been implemented to send fire sensor values from the ADC to the friend node for continuous monitoring.
   - Load power management has been taken care of by configuring the LETIMER to take ADC readings every 3 seconds. 
   - Communication has been established between LPN and friend nodes and transmission of the alert message and data has been verified. 
   - Persistent memory has been set up and functions have been added to load and store the last state of the fire sensor in memory
   - Integration of all this LPN functionality along with the friend node and other LPN has been verified and working of the entire project has been checked and validated. 
   - Project Validation plan has been updated and executed successfully. 



##### Checked repository contains the following code implementation:
     
   - gpio.c - Contains GPIO configuration and interrupt handler for interrupts Enabled for Push button factory rest and PD12 Digital                   output pin of the fire sensor
   - gpio.h - Contains necessary header files and macro definitions for implementation of GPIO functions. 
   - adc.c -  Contains 12-bit single mode ADC configuation and function to read ADC data from the CHx9 APORT of the ADC connected to the                 flame sensor analog output pin
   - adc.h -  Contains necessary header files and macro definitions for implementation of ADC functions. 
   - app.c -  Contains Bluetooth mesh implementation of the LPN node for communicating alert and data  with friend node using Generic                      ON/OFF and Generic Level model
     
      In addition to the above files the repository contains all the supporting files for bluetooth mesh implementation and SDK support. 


##### Shared Folder Link : https://drive.google.com/drive/u/0/folders/1LwaKyDHQj_LD0B7xC4oMFjqPXb2wJUsY

##### Individual Folder Link : https://drive.google.com/drive/folders/1NRAWJLthj0o71-OZ1gPeN7XCEyt4xyPn
