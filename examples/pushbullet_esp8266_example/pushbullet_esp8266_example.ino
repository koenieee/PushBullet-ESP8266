#include <ESP8266WiFi.h>
#include <PushBullet.h>
#include <WiFiClientSecure.h>


WiFiClientSecure client;

#define PORT 443

// wifi connection variables
const char* ssid     = "SSID";
const char* password = "PASS";

boolean wifiConnected = false;

int incomingByte = 0;
boolean connectWifi();

PushBullet pb = PushBullet("API", &client, 443);


void setup() {
  // Initialise Serial connection
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();

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

      switch (incomingByte) {
        case 'l':
          Serial.println("Pushbullet link pushing");
          pb.sendLinkPush("Hello, from google", "Google", "http://google.nl");
          delay(5000);
          break;
        case 'n':
          Serial.println("Pushbullet note pushing");
          pb.sendNotePush("Hello, from me", "Message");
          delay(5000);
          break;
        case 'c':
          Serial.println("Pushbullet coping clipboard pushing");
          pb.copyToClipboard("Hello from above our magnificent planet Earth.");
          delay(5000);
          break;
       case 's':
          Serial.println("Pushbullet sending sms");
          pb.sendSMSPush("This is an SMS from the ESP8266", "+00000", "TARGET", "USER");
          delay(5000);
          break;
      }
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
    if (i > 10) {
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
