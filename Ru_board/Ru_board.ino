#include "connection.h"
#include "captcha.h"
#include "bstree.h"


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

/***End connection parameters***/

/***Instantiates a new captcha code***/
captcha nearCode;
String code = "";
/***End***/

bstree Users;
student *RuClient;
student *root;

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

	static bool Ru_open = false;
	static bool Sync = false;
	static String request = "";
	
	/***Syncronization Process**/
	while ((!Ru_open) && (!Sync)) //When Ru is closed and no Sync was made do it
	{
		/***For now we only fill the tree one time***/
		for (unsigned int i = 0; i < 5; i++)
		{
			Users.insert(i, 3, 3, Users.root);
		}
		Ru_open = true;
		Sync = true;
	}

	while (Ru_open)
	{
		// Check if a client has connected
		if (serverForApp.checkForClient()) 
		{
			
			// When a Client connects we wait for the captcha and card_id
			while (request == "")
			{
				request = serverForApp.requestFromClient();
			}
			Serial.println(request);//debug feature

			// Now we should check if the captcha is correct
			if (nearCode.checkCaptcha(request)) {
				Serial.println("y"); //debug feature
				// Send an ok message to the client app
				serverForApp.write2Client("y");

				// Now we search for the user that is trying to enter
				RuClient = Users.find(serverForApp.app_card, Users.root);
				// If there is no user with this card registered it should return
				if (RuClient != NULL)
				{
					// Otherwise we check if their credits in the app exists
					if (RuClient->app_credit > 0)
					{
						//Then we debit the credits and post it to the webServer
						Users.debitCredits(1, 'a', Users.root);
						serverForApp.post2server(String(RuClient->card_id));
						//Finally we release the gate (To develop)
					}
					else serverForApp.write2Client("Out of credits");
				}
				else serverForApp.write2Client("User not in our domains yet");
			}
			else serverForApp.write2Client("n"); //Captcha incorrect
			delay(1);
			request = "";
			code = nearCode.newCaptcha();
			Serial.println(code);
		}

	}
}

