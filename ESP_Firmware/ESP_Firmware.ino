#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


String noPage = "";
String yesPage = "";
byte mikeFlag = false;
const byte STAT_BTN = 0;
const byte STAT_LED = 2;
const byte RED_LED = 13;
const byte BLUE_LED = 14;
const byte USER_BTN = 16;
const byte GREEN_LED = 12;
unsigned long counter = 0;
const char* password = "";
const char* ssid = "RIT-Legacy";


ESP8266WebServer server(80);


void handleRoot() {
  if (mikeFlag) server.send(200, "text/html", yesPage);
  else server.send(200, "text/html", noPage);
}


void mikeIn () {
  mikeFlag = true;
  server.send(200, "text/plain", "Status changed to Mike is here!");
}


void mikeOut () {
  mikeFlag = false;
  server.send(200, "text/plain", "Status changed to Mike is not here!");
}


void setup(void) {
  yesPage += "<html><head><meta http-equiv='refresh' content='5'><title>IsMikeHere";
  yesPage += "</title><style>body {background-color: #00FF44; text-align: center;}";
  yesPage += "p {color: black; font-weight: bold; font-size: 200px;}";
  yesPage += "</style></head><body><br><br><br><p>YES!</p></body></html>";

  noPage += "<html><head><meta http-equiv='refresh' content='5'><title>IsMikeHere";
  noPage += "</title><style>body {background-color: #FF0000; text-align: center;}";
  noPage += "p {color: black; font-weight: bold; font-size: 200px;}";
  noPage += "</style></head><body><br><br><br><p>NO!</p></body></html>";

  Serial.begin(115200); Serial.print("\n\r");

  pinMode(USER_BTN, INPUT);
  pinMode(RED_LED, OUTPUT); digitalWrite(RED_LED, LOW);
  pinMode(BLUE_LED, OUTPUT); digitalWrite(BLUE_LED, LOW);
  pinMode(STAT_LED, OUTPUT); digitalWrite(STAT_LED, HIGH);
  pinMode(GREEN_LED, OUTPUT); digitalWrite(GREEN_LED, LOW);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to: "); Serial.print(ssid); Serial.print("\n\r");
  while (WiFi.status() != WL_CONNECTED) {
    delay(250); Serial.print(".");
    digitalWrite(STAT_LED, !digitalRead(STAT_LED));
  }
  digitalWrite(STAT_LED, LOW);
  Serial.print("\n\r Connected to: "); Serial.print(ssid); Serial.print("\n\r");
  Serial.print("IP address: "); Serial.print(WiFi.localIP()); Serial.print("\n\r");

  server.on("/in", mikeIn);  //please dont abuse this, it helps the people in the lab
  server.on("/out", mikeOut);    //please dont abuse this, it helps the people in the lab
  server.on("/", handleRoot);
  server.begin();

  Serial.print("HTTP server started");  Serial.print("\n\r");
  Serial.print("Go to 'ismikehere.today'");  Serial.print("\n\r");
}


void loop(void) {
  server.handleClient();
  if (digitalRead(STAT_BTN) == 0 && counter > 30000) {
    Serial.print("\n\rConnected to: "); Serial.print(ssid); Serial.print("\n\r");
    Serial.print("IP address: "); Serial.print(WiFi.localIP()); Serial.print("\n\r");
    counter = 0;
  }
  if (digitalRead(USER_BTN) == 0 && counter > 30000) {
    counter = 0; mikeFlag = !mikeFlag;
  }
  if (mikeFlag) {
    digitalWrite(RED_LED, LOW); digitalWrite(GREEN_LED, HIGH);
  }
  else {
    digitalWrite(RED_LED, HIGH); digitalWrite(GREEN_LED, LOW);
  }
  counter++;
}

