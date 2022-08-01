# 1 "e:\\LSC\\ETboard_Temperature\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
/******************************************************************************************

 * FileName     : temperature_iot.ino

 * Description  : 이티보드로 온도를 측정하여 스마트폰으로 확인

 * Author       : SCS

 * Created Date : 2022.07.20

 * Reference    : 

 * Modified     : 

 * Modified     : 

******************************************************************************************/
# 11 "e:\\LSC\\ETboard_Temperature\\src\\arduino\\temperatur_iot\\temperatur_iot.ino"
//==========================================================================================
// Firmware Version
//==========================================================================================
const char* board_hardware_verion = "ETBoard_V1.1";
const char* board_firmware_verion = "etb_temp_v0.9";

//==========================================================================================
// Include Common
//==========================================================================================
# 21 "e:\\LSC\\ETboard_Temperature\\src\\arduino\\temperatur_iot\\temperatur_iot.ino" 2
OLED_U8G2 oled;

# 24 "e:\\LSC\\ETboard_Temperature\\src\\arduino\\temperatur_iot\\temperatur_iot.ino" 2
ETBOARD_COM etb;

# 27 "e:\\LSC\\ETboard_Temperature\\src\\arduino\\temperatur_iot\\temperatur_iot.ino" 2
ETBOARD_SIMPLE_MQTT mqtt;

//==========================================================================================
// Include custom
//==========================================================================================
# 33 "e:\\LSC\\ETboard_Temperature\\src\\arduino\\temperatur_iot\\temperatur_iot.ino" 2
# 34 "e:\\LSC\\ETboard_Temperature\\src\\arduino\\temperatur_iot\\temperatur_iot.ino" 2




DHT dht(D9 /* Digital pin connected to the DHT sensor*/, DHT11 /* DHT 11*/);

//==========================================================================================
// Declaration
//==========================================================================================
const uint16_t NORMAL_SEND_INTERVAL = 1000 * 3;

static unsigned lastMillis = 0;

int TRIG = D9;
int ECHO = D8;
int brightness_value;
int distance_value;

//==========================================================================================
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
  pinMode(TRIG, 0x02);
  pinMode(ECHO, 0x01);
}


//==========================================================================================
void loop()
//==========================================================================================
{
  //----------------------------------------------------------------------------------------
  // MQTT loop
  //----------------------------------------------------------------------------------------
  mqtt.loop();

  //----------------------------------------------------------------------------------------
  //  Send sensor value
  //----------------------------------------------------------------------------------------
  if (millis() - lastMillis > NORMAL_SEND_INTERVAL) {
      //mqtt.send_analog();
      //mqtt.send_digital();
      send_distance();
      lastMillis = millis();
   }

  //----------------------------------------------------------------------------------------
  //  Send digital sensor value if changed
  //----------------------------------------------------------------------------------------    
   if (mqtt.is_changed_digital() == true) {
      mqtt.send_digital();
      send_brightness();
      test1();
   }

  auto_mode();

  //----------------------------------------------------------------------------------------
  // Blink Operation LED
  //----------------------------------------------------------------------------------------  
  etb.normal_blink_led();
  //recv_LED();
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
    Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("Failed to read from DHT sensor!")))));
    return;
  }

  String string_t = String(t, 2);
  mqtt.publish("/temperature", String(string_t));
}

//==========================================================================================
void send_distance()
//==========================================================================================
{
  pinMode(TRIG, 0x02);
  pinMode(ECHO, 0x01);
  delay(10);

  digitalWrite(TRIG, 0x0);
  digitalWrite(ECHO, 0x0);
  delayMicroseconds(2);
  digitalWrite(TRIG, 0x1);
  delayMicroseconds(10);
  digitalWrite(TRIG, 0x0);

  float duration = pulseIn (ECHO, 0x1);
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

void auto_mode(){
  if(brightness_value < 2000){
    digitalWrite(D3,0x1);
    if(distance_value < 8) digitalWrite(D2, 0x1);
    else digitalWrite(D2, 0x0);

  }
  else digitalWrite(D3, 0x0);
}

//==========================================================================================
//                                                    
// (주)한국공학기술연구원 http://et.ketri.re.kr       
//                                                    
//==========================================================================================
