# Envrionment Monitoring and Control

##### Group Members : Ridhi Shah, Vatsal Sheth, Amreeta Sengupta

## Project Status 

   - Fire Sensor has been interfaced and Digital output pin is connected to pin 12 of the gpioPortD  and Analog output has been              connected to pin 9 of gpioPortC which is Analog channel 9 port.    
   - GPIO interrupt was configured and the potentiometer value on the fire sensor was set such as to define an extreme value to a given     environmental situation and an interrupt is raised when the signal exceeds this extreme value. 
   - For monitoring the continuous values on the fire sensor the analog output pin is used. ADC configuration has been done for 12-bit        ADC with 2.5 volts internal reference to read these analog values from the fire sensor. 
   - The Alert message when extreme value is crossed has been sent to the friend using Generic ON/OFF model. 
   - Generic Level Model has also been implemented to send fire sensor values to the friend node for continuous monitoring.
   - Communication has been established between LPN and friend nodes and transmission of the alert message and data has been verified.


##### Checked repository contains the following code implementation:
     
     gpio.c - Contains GPIO configuration and interrupt handler for interrupts Enabled for Push button factory rest and PD12 Digital output               pin of the fire sensor
     gpio.h - Contains necessary header files and macro definitions for implementation of GPIO functions. 
     adc.c -  Contains 12-bit single mode ADC configuation and function to read ADC data from the CHx9 APORT of the ADC connected to the                 flame sensor analog output pin
     adc.h -  Contains necessary header files and macro definitions for implementation of ADC functions. 
     app.c -  Contains Bluetooth mesh implementation of the LPN node for communicating alert and data  with friend node using Generic                      ON/OFF and Generic Level model
     
      In addition to the above files the repository contains all the supporting files for bluetooth mesh implementation and SDK support. 


##### Shared Folder Link : https://drive.google.com/drive/u/0/folders/1LwaKyDHQj_LD0B7xC4oMFjqPXb2wJUsY

##### Individual Folder Link : https://drive.google.com/drive/folders/1NRAWJLthj0o71-OZ1gPeN7XCEyt4xyPn
