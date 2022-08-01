/********************************************************************************** 
 * Author : SCS
 * Date : 2018.09.30  
**********************************************************************************/

#ifndef ETBOARD_SIMPLE_MQTT_H
#define ETBOARD_SIMPLE_MQTT_H

#include <Arduino.h>
#include <String>
#include <ArduinoJson.h>

class ETBOARD_SIMPLE_MQTT {

  private:	
    
  
  public:
    String mqtt_prefix;
    String mac_address;
    
  	ETBOARD_SIMPLE_MQTT();
  	void setup(void);
    String get_topic_prefix(void);
    void onConnectionEstablished(void);
    void send_analog_a0(void);
    void publish(const String &topic, const String &payload);
    void subscribe_mode(void);
    void send_analog(void);
    void send_digital(void);
    void recv_digital(void);
    boolean is_changed_digital(void);
    void loop(void);
};  

#endif

//=================================================================================
// End of Line
//=================================================================================
