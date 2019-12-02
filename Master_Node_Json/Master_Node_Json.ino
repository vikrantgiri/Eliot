#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <ArduinoJson.h>

const char ssid[] = "Avinash@";
const char pass[] = "akk@2653";
char jsonChar[500];


StaticJsonBuffer<200> jsonBuffer;


WiFiClient net;
MQTTClient client(256);

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  String ClientId = "987766";
  
      JsonObject& root = jsonBuffer.createObject();
      root["ClientID"] = ClientId;
      root["Status"] = "Connected";
      
      root["SSID"]=ssid;
      root["Latitude"]="1080";
      root["Longitude"]="3332";
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("broker.hivemq.com", net);
  client.onMessage(messageReceived);

  connect();
  String IP = String(WiFi.localIP());
  root["IP"]=IP;
      root.printTo(Serial);
      root.printTo((char*)jsonChar, root.measureLength() + 1);
      Serial.println(jsonChar);
 client.publish("stackmentalist/MetaData", "world");
    client.publish("stackmentalist/MetaData", jsonChar );
}


void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
      
  
  
  
//  if (millis() - lastMillis > 1000) {
//    lastMillis = millis();
//    Serial.println(jsonChar);
//      
//   
//
//  }
}
