// captcha.h

#ifndef _CAPTCHA_h
#define _CAPTCHA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class captcha
{
	String curr_captcha;
public:
	String code;

	captcha();
	~captcha();
	String newCaptcha();
	bool checkCaptcha(String);
	
};

#endif

