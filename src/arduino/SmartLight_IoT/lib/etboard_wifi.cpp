/******************************************************************************************
 * FileName     : etboard_wifi.cpp
 * Description  : ETboard WiFi Manager
 * Author       : SCS
 * Created Date : 2022.08.06
 * Reference    : 
 * Modified     : 
 * Modified     : 
******************************************************************************************/

#include "etboard_wifi.h"

WiFiManagerParameter * custom_mqtt_server;
WiFiManagerParameter * custom_mqtt_port;
WiFiManagerParameter * custom_mqtt_user;
WiFiManagerParameter * custom_mqtt_pass;
  
bool ETBOARD_WIFI::shouldSaveConfig = false;

#define TRIGGER_PIN D9

WiFiManager wifiManager;

//=================================================================================
//callback notifying us of the need to save config
//=================================================================================
void ETBOARD_WIFI::save_config_cb() 
//=================================================================================
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

//=================================================================================
// for Board Operation LED
//=================================================================================
#define LED_BLINK_INTERVAL 500
#define LED_BUILTIN 5

//=================================================================================
ETBOARD_WIFI::ETBOARD_WIFI() 
//=================================================================================	
{
  custom_mqtt_server = new WiFiManagerParameter("server", "mqtt server", mqtt_server, 40);
  custom_mqtt_port = new WiFiManagerParameter("port", "mqtt port", mqtt_port, 6);
  custom_mqtt_user = new WiFiManagerParameter("user", "mqtt user", mqtt_user, 20);
  custom_mqtt_pass = new WiFiManagerParameter("pass", "mqtt pass", mqtt_pass, 20);
  
  shouldSaveConfig = false;
}

//=================================================================================
void ETBOARD_WIFI::setup(void) 
//=================================================================================
{
  Serial.println("\n");    
  Serial.println("================================================================");
  Serial.println(" WiFi setup starting..... ");  
  Serial.println("================================================================");
  
  //clean FS for testing 
  //SPIFFS.format();
  pinMode(TRIGGER_PIN, INPUT);

  checkButton();
   
  load_config();

  wifi_config();
 
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    //read updated parameters    
    strcpy(mqtt_server, custom_mqtt_server->getValue());
    strcpy(mqtt_port, custom_mqtt_port->getValue());
    strcpy(mqtt_user, custom_mqtt_user->getValue());
    strcpy(mqtt_pass, custom_mqtt_pass->getValue());
    
    save_config();    
  }

  Serial.println("");
  Serial.println("----------------------------------------------------------------");
  Serial.print(" ETboard local ip address is [");
  Serial.print(WiFi.localIP());
  Serial.println("]");
  Serial.println("----------------------------------------------------------------");
  Serial.println("");
  Serial.println("================================================================");
  Serial.println(" WiFi setup ending ..... ");  
  Serial.println("================================================================");
  
}

//=================================================================================
void ETBOARD_WIFI::checkButton()
//=================================================================================
{
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wifiManager.resetSettings();
        // 2022.08.06
        SPIFFS.format();
        ESP.restart();
      }
    }
  }
}

//=================================================================================
void ETBOARD_WIFI::load_config()
//=================================================================================
{
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_pass, json["mqtt_pass"]);
          Serial.print("mqtt_server: ");
          Serial.println(mqtt_server);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
}

//=================================================================================
void ETBOARD_WIFI::save_config()
//=================================================================================
{
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_user"] = mqtt_user;
    json["mqtt_pass"] = mqtt_pass;    
    
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();    
    //end save
}

//=================================================================================
void ETBOARD_WIFI::wifi_config()
//=================================================================================
{
  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length  
  Serial.println(mqtt_server);

  /*
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqtt_user, 20);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", mqtt_pass, 20);
  */
  
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  //WiFiManager wifiManager;

// Reset Wifi settings for testing  
//  wifiManager.resetSettings();

  //set config save notify callback
  wifiManager.setSaveConfigCallback(save_config_cb);

  //set static ip
//  wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here  
  wifiManager.addParameter(custom_mqtt_server);
  wifiManager.addParameter(custom_mqtt_port);
  wifiManager.addParameter(custom_mqtt_user);
  wifiManager.addParameter(custom_mqtt_pass);
  

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimum quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);
  wifiManager.setTimeout(15);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
}
 
//=================================================================================
// End of Line
//=================================================================================
