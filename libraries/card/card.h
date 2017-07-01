// connection.h

#ifndef _CARD_h
#define _CARD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class card 
{
	int pin;
	int preSet_Cards[10] = {1, 2, 12, 4, 15, 6, 7, 13, 9, 5555};
public:
	int Id_card;

	card(int pin);
	~card();
	bool checkForCard(void);
	int getId();
	void post2server(String payload);
	String getFromServer(void);
};


#endif

