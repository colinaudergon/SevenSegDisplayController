#include <Arduino.h>
#include "sevenSegController.h"

#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4

SevenSegController segCtrl(SHIFT_DATA,SHIFT_CLK,SHIFT_LATCH,2,true);

void setup()
{
  segCtrl.begin();
}

void loop()
{

  segCtrl.saveDisplaynumber(25);
}

ISR(TIMER2_COMPA_vect) {

  segCtrl.displayNbr();
}



