
#ifndef _STUDENT_h
#define _STUDENT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class student{
public:
	unsigned int card_id;
	student* left;
	student* right;
	unsigned short int height;
	unsigned short int card_credit;
	unsigned short int app_credit;
	student();
};

#endif;
