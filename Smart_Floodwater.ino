#include <ESP8266WiFi.h>

#include <KiwisIoT.h>

// Wi-Fi Settings

const char* ssid = "YOUR_WIFI_NAME";

const char* pass = "YOUR_WIFI_PASSWORD";

// KiwisIoT Topic ID

const char* topic = "YOUR_TOPIC_ID";

// KiwisIoT Object

KiwisIoT kiwisiot(ssid, pass, topic);

// Sensor Pins

#define WATER_PIN A0

#define RAIN_PIN D5

// Water Level Thresholds

int waterWarningThreshold = 400;

int waterDangerThreshold = 700;

// Rain Sensor Logic

#define RAIN_DETECTED LOW

// Variables

int waterValue = 0;

int rainValue = HIGH;

String waterLevel;

String rainStatus;

String floodStatus;

unsigned long previousMillis = 0;

const unsigned long interval = 5000;

// Setup

void setup() {

Serial.begin(115200);

pinMode(RAIN_PIN, INPUT);

Serial.println();

Serial.println("ESP8266 Flood Monitoring");

kiwisiot.begin();

Serial.println("KiwisIoT Started");

}

// Read Sensors

void readSensors() {

waterValue = analogRead(WATER_PIN);

rainValue = digitalRead(RAIN_PIN);

if (waterValue < waterWarningThreshold) {

waterLevel = "LOW";

}

else if (waterValue < waterDangerThreshold) {

waterLevel = "MEDIUM";

}

else {

waterLevel = "HIGH";

}

if (rainValue == RAIN_DETECTED) {

rainStatus = "RAIN DETECTED";

}

else {

rainStatus = "NO RAIN";

}

if (waterValue >= waterDangerThreshold) {

floodStatus = "DANGER";

}

else if (waterValue >= waterWarningThreshold ||

rainValue == RAIN_DETECTED) {

floodStatus = "WARNING";

}

else {

floodStatus = "SAFE";

}

}

// Display Readings

void displayReadings() {

Serial.println("----------------------");

Serial.print("Water Value: ");

Serial.println(waterValue);

Serial.print("Water Level: ");

Serial.println(waterLevel);

Serial.print("Rain Status: ");

Serial.println(rainStatus);

Serial.print("Flood Status: ");

Serial.println(floodStatus);

Serial.println("----------------------");

}

// Publish Data

void publishData() {

kiwisiot.send("0", waterValue);

kiwisiot.send("1", rainStatus);

kiwisiot.send("2", floodStatus);

kiwisiot.send("3", waterLevel);

Serial.println("Data sent to KiwisIoT");

}

// Main Loop

void loop() {

kiwisiot.run();

if (millis() - previousMillis >= interval) {

previousMillis = millis();

readSensors();

displayReadings();

publishData();

}

}