#include <Arduino.h>

#include "etboard_com.h"


//=================================================================================
void ETBOARD_COM::slow_blink_led(void) 
//=================================================================================
{
  for(int i=0; i<10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(250);                         // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(250);                         // wait for a second
  }
}
