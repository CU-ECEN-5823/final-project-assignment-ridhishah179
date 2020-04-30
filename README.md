# Envrionment Monitoring and Control

##### Group Members : Ridhi Shah, Vatsal Sheth, Amreeta Sengupta

## Project Status 

   - The bluetooth Mesh node has been initialized for LPN functionality and to establish friendship with the friend node. Software timer has been configured for re-establishing friendship with friend nodes in case of failure. 


##### Checked repository contains the following code implementation:
     
   - gpio.c - Contains GPIO configuration and interrupt handler for interrupts Enabled for Push button factory rest and PD12 Digital                   output pin of the fire sensor
   - gpio.h - Contains necessary header files and macro definitions for implementation of GPIO functions. 
   - adc.c -  Contains 12-bit single mode ADC configuation and function to read ADC data from the CHx9 APORT of the ADC connected to the                 flame sensor analog output pin
   - adc.h -  Contains necessary header files and macro definitions for implementation of ADC functions. 
   - app.c -  Contains Bluetooth mesh implementation of the LPN node for communicating alert and data  with friend node using Generic                      ON/OFF and Generic Level model
     
      In addition to the above files the repository contains all the supporting files for bluetooth mesh implementation and SDK support. 


##### Shared Folder Link : https://drive.google.com/drive/u/0/folders/1LwaKyDHQj_LD0B7xC4oMFjqPXb2wJUsY

##### Individual Folder Link : https://drive.google.com/drive/folders/1NRAWJLthj0o71-OZ1gPeN7XCEyt4xyPn
