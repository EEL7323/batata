#include <ArduinoJson.h>
#include "connection.h"
#include "captcha.h"
#include "bstree.h"
#include "card.h"
#include "gate.h"



/***Set connection parameters***/

// const char* ssid = "steins";
//const char* password = "12345678";
const char* ssid = "GVT-5527";
const char* password = "J445143561";
//const char* ssid = "VIVO-E290";
//const char* password = "0003000640";
//const char* ssid = "Batata";
//const char* password = "batata2017";
//const char* ssid = "Monteiro";
//const char* password = "11o666o9o17o";
//const char* ssid = "LEONARDO";
int server_port = 8564; //Server Listens in Port 80
connection serverForApp(server_port); //Instantiates an connection object

/***End connection parameters***/

/***Instantiates a new captcha code***/
captcha nearCode;
String code = "";
/***      End            ***/

/***Instantiates a new tree of users***/
bstree Users;
student *RuClient;
/***      End         ***/

/***Instantiates a new cardReader***/
card CardReader(5); //Connect it to pin 8
/***        End         ***/

/*** Instantiates a few gates***/
gate entree(ENTREE_GATE, 3, 14);
//gate exit_gate(EXIT_GATE, 4, 3);
/***        End       ***/

/**Function prototipes**/
void checkTime(void);
void synchronize(void);
void solveEvent(short int eventTY, unsigned int IdEvent, unsigned int card_id, unsigned short int deltaA, unsigned short int deltaC);
/**     End        **/

/*Begins the code setup*/
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

bool Ru_open = false;

void loop() {

	synchronize();
	delay(2000);
	/*
	static bool Sync = false;
	static String request = "";
	
	/Syncronization Process/
	while ((!Ru_open) && (!Sync)) //When Ru is closed and no Sync was made do it
	{
		/For now we only fill the tree one time/
		for (unsigned int i = 0; i < 5; i++)
		{
			Users.insert(i, 3, 3, Users.root);
		}
		Ru_open = serverForApp.getTime();
		Sync = true;
	}

	checkTime();

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
				Serial.println("Captcha Correto"); //debug feature
				// Send an ok message to the client app
				

				// Now we search for the user that is trying to enter
				RuClient = Users.find(serverForApp.app_card, Users.root);
				// If there is no user with this card registered it should return
				if (RuClient != NULL)
				{
					Serial.println("User:");
					Serial.println(RuClient->card_id);
					Serial.println("App Credits:");
					Serial.println(RuClient->app_credit);
					// Otherwise we check if his credits in the app exists
					if (RuClient->app_credit > 0)
					{
						//Then we debit the credits and post it to the webServer
						Users.debitCredits(serverForApp.app_card, 'a', Users.root);
						//Tell the app that the user is allowed to enter
						serverForApp.write2Client("y");
						//Finally we release the gate
						entree.release(RuClient->card_id);
						Serial.println("End transaction");
					}
					else serverForApp.write2Client("ooc");
				}
				else serverForApp.write2Client("unr");
			}
			else serverForApp.write2Client("n"); //Captcha incorrect
			delay(1);
			request = "";
			code = nearCode.newCaptcha();
			Serial.println(code);
		}//End App connection

		//Check if a card is trying to connect
		else if (CardReader.checkForCard())
		{
			//Now we should get the card Id
			int cardId = CardReader.getId();
			Serial.print("Card Id:");
			Serial.println(cardId);

			// Then we look for our user
			RuClient = Users.find(cardId, Users.root);

			// If there is no user with this card registered it should return
			if (RuClient != NULL)
			{
				Serial.println("He is in the tree");
				// Otherwise we check if his credits in the card exists
				if (RuClient->card_credit > 0)
				{
					//Then we debit the credits and post it to the webServer
					Users.debitCredits(cardId, 'c', Users.root);
					Serial.println("User:");
					Serial.println(cardId);
					Serial.println("Credits:");
					Serial.println(RuClient->card_credit);
					//Finally we release the gate
					entree.release(RuClient->card_id);
					Serial.println("End transaction");
				}
				else Serial.println("Out of credits");
			}
			else Serial.println("User not in our domains yet");
		} //End Card
		
		checkTime();
		delay(1);
	}//End Ru_Open
	*/

}

void checkTime(void)
{
	static unsigned long int time = millis();
	if (millis() - time > 60000)
	{
		time = millis() - time;
		Ru_open = serverForApp.getTime();
	}
}

void synchronize(void) {
	
	// First, get the events to solve.
	//HTTPClient http;
	//http.begin("http://batata.dlinkddns.com/back-end/php/loadRuEvents.php");
	//int httpCode = http.GET();
	//Serial.println(httpCode);
	//String payload = http.getString();
	//Serial.println(payload);
	String payload = "[{'eventId':'6','tagNumber':'515','type':'0','diffCredCellphone':'5','diffCredTag':'5'},{'eventId':'7','tagNumber':'1231231','type':'0','diffCredCellphone':'5','diffCredTag':'5'},{'eventId':'4', 'tagNumber' : '515', 'type' : '5', 'diffCredCellphone' : '0', 'diffCredTag' : '0'}]";
	//String payload = "{'eventId':'4', 'tagNumber' : '515', 'type' : '0', 'diffCredCellphone' : '0', 'diffCredTag' : '0'}";
	// Now parsing json:
	int startIndex = 1;
	const char* eventId;
	const char* userId;
	const char* eventType;
	const char* deltaApp;
	const char* deltaCard;
	for (int i = 0; i < payload.length(); i++) {
		//loop to check if it's a '}'
		if (payload.substring(i, i + 1) == "}")
		{	
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(payload.substring(startIndex, i+1));
			Serial.println(payload.substring(startIndex, i+1));
					
			if (!root.success())
			{
				Serial.println("parseObject() failed");
				return;
			}

			eventId = root["eventId"];
			userId = root["tagNumber"];
			eventType = root["type"];
			deltaApp = root["diffCredCellphone"];
			deltaCard = root["diffCredTag"];
			/*short int eventTY = ;
			unsigned int IdEvent = ;
			unsigned int card_id = ;
			unsigned short int deltaA = ;
			unsigned short int deltaC = ;*/
			solveEvent(atoi(eventType), atoi(eventId), atoi(userId), atoi(deltaApp), atoi(deltaCard));
			startIndex = i + 2;
		}					
	}
}

void solveEvent(short int eventTY, unsigned int IdEvent, unsigned int card_id, unsigned short int deltaA, unsigned short int deltaC)
{
	
	//HTTPClient http;
	String payload = "";
	int code_returned = 0;
	switch (eventTY)
	{
	case 0:
		//add new user
		Users.insert(card_id, deltaA, deltaC, Users.root);
		//prepare payload
		payload = String(card_id) + "," + String(Users.find(card_id, Users.root)->app_credit) + "," + String(Users.find(card_id, Users.root)->card_credit) + "," + String(IdEvent);
		//post event solved 
		//http.begin("http://batata.dlinkddns.com/back-end/php/updateCredits.php");
		//code_returned = http.POST(payload);
		Serial.println("Event solved.");
		return;

	case 1: //delete user  (send tag, 0, 0, event_id)
		Users.del(card_id, Users.root);
		//prepare paylaod
		payload = String(card_id) + "," + '0' + "," + '0' + "," + String(IdEvent);
		//post event solved
		//http.begin("http://batata.dlinkddns.com/back-end/php/updateCredits.php");
		//code_returned = http.POST(payload);
		Serial.println("Event solved.");
		return;

	case 5:
		//add bought credits
		Users.insertCredits(card_id, deltaA, deltaC, Users.root);
		//prepare parameters to send
		payload = String(card_id) + ","+String(Users.find(card_id,Users.root)->app_credit)+","+String(Users.find(card_id, Users.root)->card_credit)+","+String(IdEvent);
		//post event solved 
		//http.begin("http://batata.dlinkddns.com/back-end/php/updateCredits.php");
		//code_returned = http.POST(payload);
		Serial.println("Event solved.");
		return;

	default://invalid eventType
		// return something to warn about invalid event ?
		Serial.println("Event not solved. Wrong event type");
		return;
	}
}