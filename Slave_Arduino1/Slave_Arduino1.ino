 #include<Wire.h>
 #include<ArduinoJson.h>
 #include<TimeLib.h>
 #define ANSWERSIZE 500
 #include <EEPROM.h>
 


int RelayState;
const int Relay[9]={13,12,8,7,4,2,1,0,3};
const int Meter[9]={14,15,16,17,11,10,9,6,5};
const int Status[9]={1,0,0,0,0,0,0,0,0};
char Code;
long PulseData[9];
char PulseJSON[500];
StaticJsonBuffer<500> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
void setup() {
Wire.begin(9);
Wire.onRequest(requestEvent); // data request to slave
Wire.onReceive(receiveEvent); // data slave received
Serial.begin(9600);
Serial.println("I2C Slave ready!");
memset(PulseJSON, 0, sizeof(PulseJSON));

for(int j=0;j<9;j++)
{
PulseData[j] = EEPROM.read(j);;
pinMode(Relay[j],OUTPUT);
}
}

void receiveEvent(int countToRead) {
while (0 < Wire.available()) {
Code=(char) Wire.read();
int index;
  index=(char)Wire.read()-48;
  
if( Code=='P')
{
getPulse(index);
}
else if(Code=='R')
{
  String Temp;
  if(digitalRead(Relay[index])==HIGH)
  {
    Temp="{\"0\"}";
    digitalWrite(Relay[index],LOW);
    Serial.print(Temp);
//    for(int k=0;k<7;k++)
//    RelayState[k]=Temp.charAt[k];
  }
  else
  {
    Temp="{\"1\"}";
    digitalWrite(Relay[index],HIGH);
    Serial.print(Temp);
//    for(int k=0;k<7;k++)
//    RelayState[k]=Temp.charAt[k];
  }
 // RelayState=getRelayStatus(index);
}
else if(Code=='S')
{
  
getRelayStatus(index);
  
}
//Serial.print((char)x);
}
Serial.println();
//Serial.println("Receive event");
}
void getPulse(int MeterIndex)
{
      String m = "M";
      root[m += MeterIndex ] = PulseData[MeterIndex];
      root.printTo(Serial);
     
      
      root.printTo((char*)PulseJSON, root.measureLength() + 1);
      Serial.println(PulseJSON);
      Wire.write(PulseJSON);
      jsonBuffer.clear();

}
void getRelayStatus(int RelayIndex)
{
    int relaystatus=1-digitalRead(Relay[RelayIndex]);
      String r = "R";
      root[r += RelayIndex ] = relaystatus;
      root.printTo(Serial);
     
      
      root.printTo((char*)PulseJSON, root.measureLength() + 1);
    
      Serial.println(PulseJSON);
      Wire.write(PulseJSON);
      jsonBuffer.clear();

}
void requestEvent() {
  Serial.println("I m Requested");

if(Code=='P')
Wire.write(PulseJSON);
else if(Code=='R')
Wire.write(PulseJSON);
else if(Code=='S')
Wire.write(PulseJSON);
Serial.println("Request event");


}



void loop() {

  //Serial.println(analogRead(A0));
  for(int i=0;i<9;i++)
  {
    if(analogRead(Meter[i])>550&&Status[i]==1)
  {
  
    PulseData[i]++;
    delay(200);
    Serial.println("Pluse Data:-");
    Serial.print(PulseData[i]);
    Serial.print("thisis it");
    
    EEPROM.write(i, PulseData[i]);        
  }
  
}

//delay(100);
//relayState(Relay[1],1);
//delay(2000);
//relayState(Relay[1],0);
//delay(1000);
//Serial.println("Request event11");
EEPROM.write(7, 8);
//Serial.println(EEPROM.read(7));
//for (int i = 0; i < 9; i++) {
  //  Serial.println(PulseData[i]);
 // }
  
}
