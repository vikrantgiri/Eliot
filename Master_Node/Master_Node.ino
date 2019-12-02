#include <Wire.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <ArduinoJson.h>

int sdaPin = 4;
int sclPin = 5;
const char* ssid = "connecting...";
const char* pass = "pagalhaitu";
  char jsonChar[500];
StaticJsonBuffer<200> jsonBuffer;
JsonObject& PulseDataReq= jsonBuffer.createObject();
//Wifi Module
WiFiClient net;
MQTTClient client(256);
long lastMsg = 0;
char msg[50];
int value = 0;

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

  //client.subscribe("/hello");
  client.subscribe("MDataS");
  client.subscribe("RState");
  client.subscribe("SRStatus");
  // client.unsubscribe("/hello");
}
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(topic.equals("MDataS"))
  {
    Wire.beginTransmission(9);
     Serial.println(payload.charAt(0));
     Wire.write("P");
  Wire.write(payload.charAt(0));
  Wire.endTransmission();
  delay(50);
   Serial.println("Receive data");
  Wire.requestFrom(9,32);
  String response = "";
  while (Wire.available()) {
      char b = Wire.read();
      response += b;
  }
  
 Serial.println(response.substring(0,response.lastIndexOf('}')+1));
    client.publish("MDataR",response.substring(0,response.lastIndexOf('}')+1));
    }
   else if(topic.equals("RState"))
  {
    Wire.beginTransmission(9);
     Serial.println(payload.charAt(0));
     Wire.write("R");
  Wire.write(payload.charAt(0));
  Wire.endTransmission();
  delay(50);
   Serial.println("Receive data");
  Wire.requestFrom(9,32);
  String response = "";
  while (Wire.available()) {
      char b = Wire.read();
      response += b;
  }
  
 //Serial.println(response.substring(0,response.lastIndexOf('}')+1));
    client.publish("GRState",response.substring(0,response.lastIndexOf('}')+1));
    }
       else if(topic.equals("SRStatus"))
  {
    Serial.print("Inside SRStatus");
    Wire.beginTransmission(9);
    Serial.println(payload.charAt(0));
     Wire.write("S");
  Wire.write(payload.charAt(0));
  Wire.endTransmission();
  delay(50);
   Serial.println("Receive data");
  Wire.requestFrom(9,32);
  String response = "";
  while (Wire.available()) {
      char b = Wire.read();
      response += b;
  }
 Serial.println(response.substring(0,response.lastIndexOf('}')+1));
    client.publish("RRStatus",response.substring(0,response.lastIndexOf('}')+1));
    }
}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}



void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  Wire.begin(sdaPin,sclPin);
  Wire.setClockStretchLimit(15000);
  Serial.println("I2C Master on NodeMCU ready!");
  //Connect To Wifi
  WiFi.begin(ssid, pass);
//Meta Data
  String ClientId = "987766";
      
      JsonObject& root = jsonBuffer.createObject();
      root["ClientID"] = ClientId;
      root["Status"] = "Connected";
      
      root["SSID"]=ssid;
      root["Latitude"]="1080";
      root["Longitude"]="3332";

      client.begin("iot.eclipse.org", net);
  client.onMessage(messageReceived);

  connect();
String IP = String(WiFi.localIP());
      Serial.println(WiFi.localIP());
      root["IP"]=IP;
      root.printTo(Serial);
      root.printTo((char*)jsonChar, root.measureLength() + 1);
      Serial.println(jsonChar);
 client.publish("stackmentalist/MetaData", "world");
    client.publish("stackmentalist/MetaData", jsonChar );
}



void loop() {

//  long now = millis();
//  if (now - lastMsg > 2000) {
//    lastMsg = now;
//    ++value;
//    snprintf (msg, 75, "hello world #%ld", value);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
//    client.publish("TopicPubout", msg);
//  }
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
delay(50);

//  Serial.println("Write data");
//  Wire.beginTransmission(9);
//  Wire.write("sent by master");
//  Wire.endTransmission();
  
}

