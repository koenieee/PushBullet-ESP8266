#include <ESP8266WiFi.h>
#include <PushBullet.h>
#include <WiFiClientSecure.h>


WiFiClientSecure client;

#define PORT 443

// wifi connection variables
const char* ssid     = "Ziggo21670";
const char* password = "7zqxqyVJVpkq";

boolean wifiConnected = false;

boolean connectWifi();

PushBullet pb = PushBullet("dYla0gPdoQhLxUk2KwMY4kzWRimg80Em", &client, 443);


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
    Serial.println("Pushbullet notifiing");
    pb.sendNormalPush("Test berichtje", "TEST", PUSH_TYPE::NOTE);
    delay(5000);
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