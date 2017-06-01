#include <connection.h>

const char* ssid = "steins";
const char* password = "12345678";

int ledPin = 2; // GPIO13

IPAddress cell_ip(192, 168, 15, 201);
int cell_port = 80;
connection cell_conn(cell_port);

void setup() {
	Serial.begin(115200);
	delay(10);

	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);

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
	cell_conn.startServer();
	Serial.println("Server started");

	// Print the IP address
	Serial.print("Use this URL to connect: ");
	Serial.print("http://");
	Serial.print(WiFi.localIP());
	Serial.println("/");

}

String request = "";
String captcha = "";
void loop() {
	// Check if a client has connected

	captcha = "12345";
	//captcha = String(rand());
	Serial.println(captcha);

	while (!cell_conn.checkForClient());




	while (request == "")
	{
		request = cell_conn.requestFromClient();
	}

	Serial.println(request);

	if (request == captcha) {
		Serial.println("y");
		cell_conn.write2Client("y");
	}
	else {
		Serial.println("n");
		cell_conn.write2Client("n");
	}



	// Return the response
	//cell_conn.write2Client(request);

	delay(1);
	Serial.println("Client disonnected");
	Serial.println("");
	request = "";
}