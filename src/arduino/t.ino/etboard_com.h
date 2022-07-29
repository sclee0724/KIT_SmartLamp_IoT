/********************************************************************************** 
 * Author : SCS
 * Date : 2018.09.30  
**********************************************************************************/

#ifndef ETBOARD_COM_H
#define ETBOARD_COM_H

#include <Arduino.h>
//#include <String>

class ETBOARD_COM {

  

  private:	
    //String lineString[3];
  
  public:
  	ETBOARD_COM();
  	void setup(void);
  	void fast_blink_led(void);
    void normal_blink_led(void);
    void print_board_information(const char* hardware_version, const char* firmware_verion);
    //#include "p_part1.h"
    void slow_blink_led(void);
  
};

#endif

//=================================================================================
// End of Line
//=================================================================================
`