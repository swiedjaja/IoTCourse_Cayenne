#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <DHTesp.h>
#include "device.h"
#include "wifi_id.h"
#include "cayenne_id.h"

unsigned long nPrevMillis = 0;
DHTesp dht;

void setup() {
	Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  dht.setup(PIN_DHT, DHTesp::DHT11);
  Serial.println("Booting...");
  Serial.println("Connecting to Cayenne...");
	Cayenne.begin(MQTT_USERNAME, MQTT_PASSWORD, CAYENNE_CLIENTID, WIFI_SSID, WIFI_PASSWORD);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_OFF);
}

void loop() {
	Cayenne.loop();
}

CAYENNE_OUT_DEFAULT()
{
  float h = dht.getHumidity();
  if (dht.getStatus() == DHTesp::ERROR_NONE)
  {
    Serial.printf("Humidity: %.2f\n", h);
  	Cayenne.virtualWrite(1, h);
  }
}

// Default function for processing actuator commands from the Cayenne Dashboard.
CAYENNE_IN_DEFAULT()
{
  int nValue = getValue.asInt();

  switch (request.channel)
  {
  case 2:
    digitalWrite(LED_RED, nValue);
    break;
  case 3:
    digitalWrite(LED_GREEN, nValue);
    break;
  case 4:
    digitalWrite(LED_YELLOW, nValue);
    break;  
  }
}