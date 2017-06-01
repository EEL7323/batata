#include <connection.h>
#include "captcha.h"

//Set connection parameters
//const char* ssid = "steins";
//const char* password = "12345678";
const char* ssid = "GVT-5527";
const char* password = "J445143561";

int server_port = 8888;
connection serverForApp(server_port);
captcha nearCode;

void setup() {
	//Start serial with 115200 baudrate
	Serial.begin(115200);
	//Wait 10ms for serial to start
	delay(10);

	// Connect to WiFi network
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");

	// Start the server
	serverForApp.startServer();
	Serial.println("Server started");

	// Print the IP address
	Serial.print("My IPv4 is: ");
	Serial.print(WiFi.localIP());
	Serial.println("");

}
String code = "";
String request = "";
void loop() {
	// Check if a client has connected
	if (!serverForApp.checkForClient())
	{
		code = nearCode.newCaptcha();
		Serial.println(code);
		while (request == "")
		{
			request = serverForApp.requestFromClient();
		}
		Serial.println(request);//debug feature
		if (nearCode.checkCaptcha(request)) {
			Serial.println("y");
			serverForApp.write2Client("y");
		}
		else {
			Serial.println("n");
			serverForApp.write2Client("n");
		}
		delay(1);
		Serial.println("Client disonnected");
		Serial.println("");
		request = "";
	}
	
	
}

