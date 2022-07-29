// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN D9     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println(F("\n\n\n\n\n\n\n"));
  Serial.println(F("=========================================="));
  Serial.println(F(" ETboard V1.1"));
  Serial.println(F(" http://et.ketri.re.kr"));
  Serial.println(F(" #ETboard #ESP32 #Arduino #아두이노키트 #온도"));  
  Serial.println(F("=========================================="));
  Serial.println(F(" DHT11 온도 센서 테스트"));
  Serial.println(F(" DHT11 센서를 D9 pin에 연결하십시오."));    
  Serial.println(F("=========================================="));
  Serial.println(F(" DHTxx test start....!!"));
  Serial.println(F("==========================================\n"));
  
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();  
  float t = dht.readTemperature();  
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println();

}
