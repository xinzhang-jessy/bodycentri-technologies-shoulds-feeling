#include <WiFiNINA.h>
#define PubNub_BASE_CLIENT WiFiClient
#include <PubNub.h>
#include <ArduinoJson.h>

//Name of your access point
char ssid[] = "HIVE";
//Wifi password
char pass[] = "zhanghan";

int status = WL_IDLE_STATUS;       // the Wifi radio's status
int serverCheckRate = 1000; //how often to publish/read data on PN
unsigned long lastSendCheck; //time of last publish
unsigned long lastMsgTimestamp; // time of last message timestamp

int lastFeeling = 0;
int fsrPin1 = A0;   
int fsrPin2 = A7;
StaticJsonDocument<200> outMessage; // The JSON from the outgoing message

// pubnub keys
char pubkey[] = "pub-c-b1c6095a-ac3d-469e-b1ac-5a24129b7dc1";
char subkey[] = "sub-c-1d4781da-915c-11eb-83ad-9af11ba6f334";

char publishChannel[] = "shoulder_feeling"; // channel to publish YOUR data

int pinVal1;
int pinVal2;

void setup(void) {
  Serial.begin(9600);
  connectToPubNub();
}

void loop(void) {
  pinVal1 = analogRead(fsrPin1);
  pinVal2 = analogRead(fsrPin2);
  
  Serial.print("Analog reading = ");
  Serial.print(pinVal1);
  Serial.print(",");
  Serial.println(pinVal2);
  
  int feeling1 = 0;
  Serial.print("feeling 1:");
  Serial.print(pinVal1);
  if (pinVal1 < 10) {
    Serial.println(" - No pressure");
    feeling1 = 1;
  } else if (pinVal1 < 200) {
    Serial.println(" - Light touch");
    feeling1 = 2;
  } else if (pinVal1 < 500) {
    Serial.println(" - Light squeeze");
    feeling1 = 3;
  } else if (pinVal1 < 800) {
    Serial.println(" - Medium squeeze");
    feeling1 = 4;
  } else {
    feeling1= 5;
    Serial.println(" - Big squeeze");
  }

  int feeling2 = 0;
  Serial.print("feeling 2:");
  Serial.print(pinVal2);
  if (pinVal2 < 10) {
    Serial.println(" - No pressure");
    feeling2 = 1;
  } else if (pinVal2 < 200) {
    Serial.println(" - Light touch");
    feeling2 = 2;
  } else if (pinVal2 < 500) {
    Serial.println(" - Light squeeze");
    feeling2 = 3;
  } else if (pinVal2 < 800) {
    Serial.println(" - Medium squeeze");
    feeling2 = 4;
  } else {
    feeling2 = 5;
    Serial.println(" - Big squeeze");
  }
  
  int feeling = 0;
  if (feeling1 > feeling2){
    feeling = feeling1;
  }else {
    feeling = feeling2;
  }
  
  delay(200);

  if (feeling != lastFeeling) {
    if ((millis() - lastSendCheck) >= serverCheckRate) {
      sendMessage( publishChannel, feeling );
      lastFeeling = feeling;
      lastSendCheck = millis();
    }
  }
}

void connectToPubNub()
{
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to the network, SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    Serial.print("*");
    // wait 2 seconds for connection:
    delay(2000);
  }
  // once you are connected :
  Serial.println();
  Serial.print("You're connected to ");
  Serial.println(ssid);
  PubNub.begin(pubkey, subkey);
  Serial.println("Connected to PubNub Server");
}

/*
void sendAfterDelay(int pollingRate)
{
  if ((millis() - lastSendCheck) >= pollingRate)
  {
    sendMessage(publishChannel);
    lastSendCheck = millis();
  }
}
*/

void sendMessage(char channel[], int feeling)
{
  Serial.print("Sending Message to ");
  Serial.print(channel);
  Serial.println(" Channel");

  char msg[100]; // variable for the JSON to be serialized into for your outgoing message

  // assemble the JSON to publish
  outMessage["feeling"] = feeling; // first key value is sender: publisher
  outMessage["timestamp"] = millis();

  serializeJson(outMessage, msg); // serialize JSON to send - sending is the JSON object, and it is serializing it to the char msg
  Serial.println(msg);

  WiFiClient* client = PubNub.publish(channel, msg); // publish the variable char
  if (!client)
  {
    Serial.println("publishing error"); // if there is an error print it out
  }
  else
  {
    Serial.print("   ***SUCCESS");
  }
}
