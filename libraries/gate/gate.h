// captcha.h

#ifndef _GATE_h
#define _GATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define ENTREE_GATE 1
#define EXIT_GATE   0

#define GATE_RELEASED 1
#define GATE_BLOCKED  0

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


class gate
{
	int gate_id;
	int gate_sensor;
	bool gate_func;
	bool status;
public:
	String code;

	gate(bool mode, int Id, int sensor);
	~gate();
	void release(int card_id, bool app_card);
	void block(void);
	bool getStatus();	
	void postentry(String payload);
};

#endif

