#include <Arduino.h>
#line 1 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
/******************************************************************************************
 * FileName     : temperature_iot.ino
 * Description  : 이티보드로 온도를 측정하여 스마트폰으로 확인
 * Author       : SCS
 * Created Date : 2022.07.20
 * Reference    : 
 * Modified     : 
 * Modified     : 
******************************************************************************************/

//==========================================================================================
// Firmware Version
//==========================================================================================
const char* board_hardware_verion = "ETBoard_V1.1";
const char* board_firmware_verion = "etb_temp_v0.9";

//==========================================================================================
// Include Common
//==========================================================================================
#include "oled_u8g2.h"
OLED_U8G2 oled;

#include "etboard_com.h"
ETBOARD_COM etb;

#include "etboard_simple_mqtt.h"
ETBOARD_SIMPLE_MQTT mqtt;

//==========================================================================================
// Include custom
//==========================================================================================
#include "DHT.h"
#include "PubSubClient.h"

#define DHTPIN D9     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

//==========================================================================================
// Declaration
//==========================================================================================
const uint16_t NORMAL_SEND_INTERVAL = 1000 * 1;

static unsigned lastMillis = 0;

int TRIG = D9;                       
int ECHO = D8; 
int brightness_value;
int distance_value;
extern String mode_val;

//==========================================================================================
#line 54 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void setup();
#line 89 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void loop();
#line 137 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void onConnectionEstablished();
#line 145 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void display_BI();
#line 155 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void test1();
#line 164 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void send_temperature();
#line 180 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void send_distance();
#line 203 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void send_brightness();
#line 216 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void auto_mode();
#line 54 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
void setup()
//==========================================================================================
{
  //----------------------------------------------------------------------------------------
  // etboard
  //----------------------------------------------------------------------------------------  
  etb.setup();
  etb.fast_blink_led();
  etb.print_board_information(board_hardware_verion, board_firmware_verion);    

  //----------------------------------------------------------------------------------------
  // oled
  //----------------------------------------------------------------------------------------
  oled.setup();
  display_BI();  

  //----------------------------------------------------------------------------------------
  // mqtt
  //----------------------------------------------------------------------------------------
  mqtt.setup();

  //----------------------------------------------------------------------------------------
  // initialize variables
  //----------------------------------------------------------------------------------------
  lastMillis = millis();

  //----------------------------------------------------------------------------------------
  // Proximity
  //----------------------------------------------------------------------------------------
  pinMode(TRIG, OUTPUT);    
  pinMode(ECHO, INPUT);  
}


//==========================================================================================
void loop()
//==========================================================================================
{
  //----------------------------------------------------------------------------------------
  // MQTT loop
  //----------------------------------------------------------------------------------------
  mqtt.loop();
  //mqtt.recv_system_value();
  //----------------------------------------------------------------------------------------
  //  Send sensor value
  //----------------------------------------------------------------------------------------
  if (millis() - lastMillis > NORMAL_SEND_INTERVAL) {  
      //mqtt.send_analog();
      //mqtt.send_digital();
      send_distance();
      lastMillis = millis();
      if(mode_val == "automatic"){
        if(digitalRead(D3))
          mqtt.publish("/D3", "1");
        else
          mqtt.publish("/D3", "0");
          
        if(digitalRead(D2))
          mqtt.publish("/D2", "1");
        else
          mqtt.publish("/D2", "0");
      }
   }  

  //----------------------------------------------------------------------------------------
  //  Send digital sensor value if changed
  //----------------------------------------------------------------------------------------    
   if (mqtt.is_changed_digital() == true) {
      mqtt.send_digital();
      send_brightness();
      test1();      
   }
   if(mode_val == "automatic")
    auto_mode();

  //----------------------------------------------------------------------------------------
  // Blink Operation LED
  //----------------------------------------------------------------------------------------  
  etb.normal_blink_led();
}


//==========================================================================================
void onConnectionEstablished()
//==========================================================================================
{
  mqtt.onConnectionEstablished();
}


//==========================================================================================
void display_BI() 
//==========================================================================================
{
  oled.setLine(1,"<ketri.re.kr>");
  oled.setLine(2,"Welcome to");
  oled.setLine(3," ET-Board");
  oled.display();  
}

//==========================================================================================
void test1()
//==========================================================================================
{  
  int sensorValue = 0; 
  sensorValue = analogRead(A0);
  mqtt.publish("/a0", String(sensorValue));
}

//==========================================================================================
void send_temperature()
//==========================================================================================
{ 
  float h = dht.readHumidity();  
  float t = dht.readTemperature();  

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  String string_t = String(t, 2); 
  mqtt.publish("/temperature", String(string_t));
}

//==========================================================================================
void send_distance()
//==========================================================================================
{ 
  pinMode(TRIG, OUTPUT);    
  pinMode(ECHO, INPUT);  
  delay(10);
  
  digitalWrite(TRIG, LOW);
  digitalWrite(ECHO, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  float duration = pulseIn (ECHO, HIGH);      
  float distance = duration * 17 / 1000;
  distance_value = distance;

  String string_d = String(distance, 2); 
  mqtt.publish("/distance", String(string_d));
}

//==========================================================================================
void send_brightness()
//==========================================================================================
{ 
  delay(20);
  int cds = 0;
  cds = analogRead(A3);
  brightness_value = cds;

  Serial.println(cds);
  mqtt.publish("/brightness", String(cds));
}

//==========================================================================================
void auto_mode()
//==========================================================================================
{
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  
  if(brightness_value < 2000){
    digitalWrite(D3, HIGH);
    
    if(distance_value < 8){
      digitalWrite(D2, HIGH);
    }
    else{
      digitalWrite(D2, LOW);
    }
    
  }
  else {
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
  }
}


//==========================================================================================
//                                                    
// (주)한국공학기술연구원 http://et.ketri.re.kr       
//                                                    
//==========================================================================================

