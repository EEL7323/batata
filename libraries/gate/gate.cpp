// 
// 
// 

#include "gate.h"


gate::gate(bool mode, int Id, int sensor)
{
	gate_func = mode;
	gate_id = Id;
	pinMode(gate_id, OUTPUT);
	gate_sensor = sensor;
	pinMode(gate_sensor, INPUT);
	return;
}
gate::~gate()
{
	return;
}

void gate::release(int card_id)
{
	String payload;
	status = GATE_RELEASED;
	digitalWrite(gate_id, HIGH);
	while (!digitalRead(gate_sensor))
	{
		delay(100);
	}
	if (gate_func = ENTREE_GATE) payload = String(card_id) + ",3";
	else payload = String(card_id) + ",4";
	gate::postentry(payload);

	gate::block();
	return;
}

void gate::block(void)
{
	status = GATE_BLOCKED;
	digitalWrite(gate_id, LOW);
	return;
}

bool gate::getStatus(void)
{
	return status;
}


void gate::postentry(String payload)
{
	HTTPClient http;
	Serial.println(payload);
	http.begin("http://192.168.137.1:8081/server/back-end/php/ruInOut.php");
	int code_returned = http.POST(payload);
	Serial.println(code_returned);
	String response = http.getString();
	Serial.println(response);
	return;
}
