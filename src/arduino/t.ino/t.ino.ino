//==========================================================================================
// Firmware Version
//==========================================================================================
const char* board_hardware_verion = "ETBoard_V1.1";
const char* board_firmware_verion = "etb_temp_v0.9";

#include "etboard_com.h"
ETBOARD_COM etb;


void setup() {
  // put your setup code here, to run once:
  //----------------------------------------------------------------------------------------
  // etboard
  //----------------------------------------------------------------------------------------  
  etb.setup();
  etb.fast_blink_led();
  etb.print_board_information(board_hardware_verion, board_firmware_verion);  

}

void loop() {
  // put your main code here, to run repeatedly:
  etb.slow_blink_led();

}
