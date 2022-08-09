#line 1 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\etboard_com.cpp"
/********************************************************************************** 
 * Author : SCS
 * Date : 2018.09.30  
 * Description : SSD1306 OLED Display  
 * Reference: FontUsage.ino in u8g2 examples 
 **********************************************************************************/

#include "etboard_com.h"

//=================================================================================
// for Board Operation LED
//=================================================================================
#define LED_BLINK_INTERVAL 500
#define LED_BUILTIN 5

//=================================================================================
ETBOARD_COM::ETBOARD_COM() 
//=================================================================================	
{

}

//=================================================================================
void ETBOARD_COM::setup(void) 
	//=================================================================================
{
  pinMode(LED_BUILTIN, OUTPUT);  
  Serial.begin(115200);
}

//=================================================================================
void ETBOARD_COM::fast_blink_led(void) 
//=================================================================================
{
  for(int i=0; i<10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(50);                         // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(50);                         // wait for a second
  }
}

//==============================================================================
void ETBOARD_COM::normal_blink_led(void) 
//==============================================================================
{
  static uint8_t ledState = LOW;             // ledState used to set the LED
  static unsigned long previousMillis = 0;   // will store last time LED was updated
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= LED_BLINK_INTERVAL) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);  
  }    
}

//=================================================================================
void ETBOARD_COM::print_board_information(const char* hardware_version, const char* firmware_verion)
//=================================================================================
{
  Serial.print("\n\n");  
  Serial.print("Welcome to ");
  Serial.print(hardware_version);
  Serial.println(" !!!");
  Serial.print("Firmware version is ");
  Serial.println(firmware_verion);  
}
 
//=================================================================================
// End of Line
//=================================================================================
