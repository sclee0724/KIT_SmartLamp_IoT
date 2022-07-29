/********************************************************************************** 
 * Author : SCS
 * Date : 2018.09.30  
**********************************************************************************/

#ifndef OLED_U8G2_H
#define OLED_U8G2_H

#include <Arduino.h>
#include <String>

class OLED_U8G2{

private:	
  String lineString[3];

public:
	OLED_U8G2();
	void setup(void);
	void setLine(int line, String buffer);
	void display();	
};

#endif

//=================================================================================
// End of Line
//=================================================================================