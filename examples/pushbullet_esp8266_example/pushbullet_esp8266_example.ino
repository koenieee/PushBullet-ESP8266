#include <ESP8266WiFi.h>
#include <PushBullet.h>
#include <WiFiClientSecure.h>


WiFiClientSecure client;

#define PORT 443

// wifi connection variables
const char* ssid     = "SSID";
const char* password = "KEY";

boolean wifiConnected = false;

int incomingByte = 0;
boolean connectWifi();

PushBullet pb = PushBullet("API_KEY", &client, 443);


void setup() {
  // Initialise Serial connection
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  pb.openConnection();


  if (!pb.checkConnection()) {
    Serial.println("Failed to connect to pushbullet.com");
    return;
  }

}

void loop() {
  // check if the WiFi connection were successful
  if (wifiConnected) {
    if (Serial.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial.read();
      String result = "";
      switch (incomingByte) {
        case 'l':
          Serial.println("Pushbullet link pushing");
          result = pb.sendLinkPush("Hello, from google", "Google", "http://google.nl");
          break;
        case 'n':
          Serial.println("Pushbullet note pushing");
          result = pb.sendNotePush("Hello, from me", "Message");
          break;
        case 'c':
          Serial.println("Pushbullet coping clipboard pushing");
          result = pb.copyToClipboard("Hello from above our magnificent planet Earth.");
          break;
        case 's':
          Serial.println("Pushbullet sending sms");
          result = pb.sendSMSPush("This is an SMS from the ESP8266", "+00000", "TARGET", "USER");
          break;
        case 'a':
          Serial.println("Adding device to pushbullet server");
          result = pb.registerThisDevice("DEVICE_NAME", "DEVICE_ID");
          break;
        case 'g':
          Serial.println("Getting latest push:");
          //char * res = (char*)pb.getLatestPushed("PUSH_DATE", 1).c_str();
          pushesJsonObject pjo = pb.convertJsonToStruct();
          Serial.println("Name is: " + pjo.byWho);
          Serial.println("Message is: " + pjo.message);
          Serial.println("Title is: " + pjo.title);
          break;
      }
      Serial.println("Result was: " + result);
      delay(5000);
    }
  }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 30) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}