
#include "card.h"


/* This is the constructor of the card class
* It sets the cardReader pin as an input
*/
card::card(int cardReader_pin)
{
	pin = cardReader_pin;
	pinMode(cardReader_pin, INPUT);
	randomSeed(millis());
	return;
}

/* This is the destructor of the card class
* It does not do anything by now
*/
card::~card()
{
	return;
}


/* This is the check for card method
* It check if a card is near the card reader
* and returns 1 when the card gets closer
* That means that someone is trying to get into RU
*/
bool card::checkForCard()
{
	bool val = digitalRead(pin);
	delay(100);
	return val;
}

/* This is the request from client method
* It fills a response string with the message
* sent from the client (app)
*/

int card::getId()
{
	int val = random(0, 9);
	return preSet_Cards[val];
}



void card::post2server(String payload)
{
	HTTPClient http;
	http.begin("http://192.168.137.1:8081/server/back-end/php/enterRU.php");
	int code_returned = http.POST(payload);
}

String card::getFromServer(void)
{
	HTTPClient http;
	http.begin("http://192.168.137.1:8081/server/back-end/php/enterRU.php");
	int httpCode = http.GET();
	String payload = http.getString();
	return payload;
}