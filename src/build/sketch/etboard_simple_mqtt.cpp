#line 1 "e:\\LSC\\ETboard_Iot_SmartLight\\src\\arduino\\temperatur_iot\\etboard_simple_mqtt.cpp"
/********************************************************************************** 
 * Author : SCS
 * Date : 2018.09.30  
 * Description : SSD1306 OLED Display  
 * Reference: FontUsage.ino in u8g2 examples 
 **********************************************************************************/
#include "etboard_simple_mqtt.h"

#include "EspMQTTClient.h"
EspMQTTClient client(
  "iptime-guest",
  "0617212484",
  "broker.hivemq.com",  // MQTT Broker server ip
  "",                   // Can be omitted if not needed  // Username
  "",                   // Can be omitted if not needed  // Password
  "",                   // Client name that uniquely identify your device
  1883                  // The MQTT port, default to 1883. this line can be omitted
);

#define MAX_ANALOG 8
//int analogs[MAX_ANALOG]={36, 39, 32, 33, 34, 35, 25, 26};

#define MAX_DIGITAL 10           // 0  1   2   3   4   5   6   7   8   9  
int digitals[MAX_DIGITAL]      = {-1, -1, D2, D3, D4, D5, D6, D7, D8, D9};
int digitals_mode[MAX_DIGITAL] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
int digitals_value[MAX_DIGITAL]= {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
String mode_val = "automatic";

//=================================================================================
ETBOARD_SIMPLE_MQTT::ETBOARD_SIMPLE_MQTT() 
//=================================================================================	
{

}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::setup(void) 
//=================================================================================
{
  // 2021.12.19 : SCS : dt->data, et->etboard, smpl -> simple
  mqtt_prefix = "dt/et/smpl";
  mac_address = "";
    
  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  //client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  //client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  //client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

//=================================================================================
String ETBOARD_SIMPLE_MQTT::get_topic_prefix(void)
//=================================================================================
{
  String prefix = "";
  prefix = mqtt_prefix + "/" + mac_address.substring(9); // only last 3 bytes
  return prefix;
}

//=================================================================================
// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
//=================================================================================
void ETBOARD_SIMPLE_MQTT::onConnectionEstablished(void)
//=================================================================================
{

  /*
  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish(get_topic_prefix() 
        + "/info/version",  String(board_hardware_verion) + "/" + String(board_firmware_verion));
  });
  */
  
  mac_address = WiFi.macAddress();

  recv_digital();
  recv_system_value();
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::send_analog_a0(void)
//=================================================================================
{
  int sensorValue = 0; 
  sensorValue = analogRead(A0);
  client.publish("etboard/a0", String(sensorValue));
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::publish(const String &topic, const String &payload)
//=================================================================================
{  
  client.publish(get_topic_prefix() + topic, payload);
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::send_analog(void)
//=================================================================================
{
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject(); 
  
  JsonArray& values = JSONencoder.createNestedArray("values");

  int sensorValue = 0; 
  sensorValue = analogRead(A0); values.add(sensorValue);
  sensorValue = analogRead(A1); values.add(sensorValue);
  sensorValue = analogRead(A2); values.add(sensorValue);
  sensorValue = analogRead(A3); values.add(sensorValue);
  sensorValue = analogRead(A4); values.add(sensorValue);
  sensorValue = analogRead(A5); values.add(sensorValue);
 
  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
 
  if (client.publish(get_topic_prefix() + "/analog", JSONmessageBuffer) == true) {
    //Serial.println("Success sending message");
  } else {
    //Serial.println("Error sending message");
  }
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::send_digital(void)
//=================================================================================
{    
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject(); 
 
  JsonArray& values = JSONencoder.createNestedArray("values");

  for (int pinNumber = 0; pinNumber < MAX_DIGITAL; pinNumber++) {  
    values.add(digitals_value[pinNumber]);
  }   
  
  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
 
  if (client.publish(get_topic_prefix() + "/digital", JSONmessageBuffer) == true) {
    //Serial.println("Success sending message");
  } else {
    //Serial.println("Error sending message");
  }
}


//=================================================================================
void ETBOARD_SIMPLE_MQTT::recv_digital(void)
//=================================================================================
{
  client.subscribe(get_topic_prefix() + "/D2", [](const String & payload) {
  pinMode(D2, OUTPUT);
  if (payload == "0") digitalWrite(D2, LOW);
    else digitalWrite(D2, HIGH);
  });

  client.subscribe(get_topic_prefix() + "/D3", [](const String & payload) {
  pinMode(D3, OUTPUT);
  if (payload == "0") digitalWrite(D3, LOW);
    else digitalWrite(D3, HIGH);
  });

  client.subscribe(get_topic_prefix() + "/D4", [](const String & payload) {
  pinMode(D4, OUTPUT);
  if (payload == "0") digitalWrite(D4, LOW);
    else digitalWrite(D4, HIGH);
  });

  client.subscribe(get_topic_prefix() + "/D5", [](const String & payload) {
  pinMode(D5, OUTPUT);
  if (payload == "0") digitalWrite(D5, LOW);
    else digitalWrite(D5, HIGH);
  });
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::recv_system_value(void)
//=================================================================================
{
client.subscribe(get_topic_prefix() + "/mode", [&](const String & payload) {
  pinMode(D5, OUTPUT);
  if (payload == "Auto"){
    mode_val = "automatic";
    digitalWrite(D5, HIGH);
  }
    else{
      mode_val = "manual";
      digitalWrite(D5, LOW);
    }
  });
}

//=================================================================================
boolean ETBOARD_SIMPLE_MQTT::is_changed_digital(void)
//=================================================================================
{
  // check if different current and previous value
  boolean bFound = false;
  for (int pinNumber = 6; pinNumber < MAX_DIGITAL; pinNumber++) {  
      int pin = digitals[pinNumber];  
      if(pin >= 0) {        
        pinMode(pin,INPUT);  
        int val = digitalRead(pin); 

        // 2018.10.15 : SCS : To reverse 4 buttons's value on board is pull-up circuit
        if( pinNumber >=6 && pinNumber <= 9) 
        {
          val = !val;
        }

        if (digitals_value[pinNumber] != val) {  
          digitals_value[pinNumber] = val;     
          bFound = true;
        }      
     }
  }      
  return bFound;
  
}

//=================================================================================
void ETBOARD_SIMPLE_MQTT::loop(void)
//=================================================================================
{
  client.loop();
}

 
//=================================================================================
// End of Line
//=================================================================================
