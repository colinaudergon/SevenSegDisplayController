Simple project allowing to control up to 8 seven segments displays using only 3 pins of the arduino.

This library uses the timer 2 in order to print multiple character on the seven segment display. 
Call the "saveDisplayNumber()" function with the number you wish to print in your main app. And place the "displayNbr()" inside the ISR(TIMER2_COMPA_vect) function call.

The hardware is based on two 74HC595 shif registers.

![image](https://github.com/colinaudergon/SevenSegDisplayController/assets/63097004/890fea8b-08ff-4809-87ca-6763f4bd5559)
