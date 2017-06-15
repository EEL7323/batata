// connection.h

#ifndef _CONNECTION_h
#define _CONNECTION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "Server.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

class connection : public WiFiServer
{
	WiFiClient app;
	bool status;
	int port;
	int request_code;
	
public:
	int app_card;

	connection(int set_port);
	~connection();
	void startServer();
	bool getStatus();
	int checkForClient();
	String requestFromClient();
	void write2Client(String value);
	void post2server(String payload);
	String getFromServer(void);
};


#endif

