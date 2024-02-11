Simple project allowing to control up to 8 seven segments displays using only 3 pins of the arduino.

This library uses the timer 2 in order to print multiple character on the seven segment display. 
Call the "saveDisplayNumber()" function with the number you wish to print in your main app. And place the "displayNbr()" inside the ISR(TIMER2_COMPA_vect) function call.
