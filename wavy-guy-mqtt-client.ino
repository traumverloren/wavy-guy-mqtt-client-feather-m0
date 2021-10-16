#include <MQTTClient.h>
#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h" 

#define PIN        12

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

WiFiClient net;
MQTTClient client;

void connect();  // <- predefine connect() for setup()

void setup() {
  WiFi.setPins(8,7,4,2);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  client.begin(IP_ADDRESS, PORT, net);
  client.onMessage(messageReceived);

  connect();
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("wavy-guy-mqtt-client", "", "")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  client.subscribe("wavy-gravy");
}

void messageReceived(String &topic, String &payload) {
  const char* delimiter = ",";
  String incomingMode = payload.substring(0,payload.indexOf(delimiter));
  String colorValue   = payload.substring(incomingMode.length()+2,payload.length());
  Serial.println("topic: " + topic);
  Serial.println("payload: " + incomingMode);
}


void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
}
