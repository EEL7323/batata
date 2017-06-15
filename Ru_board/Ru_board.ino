#include "connection.h"
#include "captcha.h"

/***Set connection parameters***/
//const char* ssid = "steins";
//const char* password = "12345678";
//const char* ssid = "GVT-5527";
//const char* password = "J445143561";
//const char* ssid = "VIVO-E290";
//const char* password = "0003000640";
const char* ssid = "Batata";
const char* password = "batata2017";

int server_port = 80; //Server Listens in Port 80
connection serverForApp(server_port); //Instantiates an connection object
/**End connection parameters**/

/***Instantiates a new captcha code***/
captcha nearCode;
String code = "";

/*Begins the coe setup*/
void setup() {
	//Start serial with 115200 baudrate
	Serial.begin(115200);
	//Wait 10ms for serial to start
	delay(10);

	/*** Connect to WiFi network ***/
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
	/**Connected**/

	/*** Start the server ***/
	serverForApp.startServer();
	Serial.println("Server started");

	/*** Print the IP address for debug ***/
	Serial.print("My IPv4 is: ");
	Serial.print(WiFi.localIP());
	Serial.println("");

	/***Generate a new captcha and print it***/
	code = nearCode.newCaptcha();
	Serial.println(code);
}

void loop() {

	static bool Ru_open = true;
	static String request = "";
	while (Ru_open)
	{
		// Check if a client has connected
		if (serverForApp.checkForClient()) {

			while (request == "")
			{
				request = serverForApp.requestFromClient();
			}
			Serial.println(request);//debug feature
			if (nearCode.checkCaptcha(request)) {
				Serial.println("y"); //debug feature
				serverForApp.write2Client("y");
			}
			else {
				Serial.println("n"); //debug feature
				serverForApp.write2Client("n");
			}
			delay(1);
			Serial.println("Client disonnected");
			Serial.println("");
			request = "";

			code = nearCode.newCaptcha();
			Serial.println(code);
		}
	}
}

