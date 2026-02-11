#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "config.h"

Adafruit_BME280 bme;

void connectWiFi() {

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection timeout.");
  }
}

void sendJson() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost. Reconnecting...");
    connectWiFi();
    return;
  }

  float temp = bme.readTemperature();
  float hum  = bme.readHumidity();
  float pres = bme.readPressure() / 100.0;

  if (isnan(temp) || isnan(hum) || isnan(pres)) {
    Serial.println("Sensor read error!");
    return;
  }

  StaticJsonDocument<256> doc;

  doc["location"] = LOCATION;
  doc["temperature"] = temp;
  doc["humidity"] = hum;
  doc["pressure"] = pres;

  String payload;
  serializeJson(doc, payload);

  for (int attempt = 1; attempt <= 3; attempt++) {

    Serial.printf("\nHTTP attempt %d\n", attempt);

    HTTPClient http;
    http.setTimeout(4000);
    http.setReuse(false);

    http.begin(API_ENDPOINT);
    http.addHeader("Content-Type", "application/json");

    int code = http.POST(payload);

    if (code > 0) {
      Serial.println("SUCCESS:");
      Serial.println(code);
      Serial.println(http.getString());

      http.end();
      return;
    }

    Serial.print("ERROR: ");
    Serial.println(http.errorToString(code));

    http.end();
    delay(500);
  }

  Serial.println("All retries failed.");
}

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println("\nESP32-C3 BME280 sender starting...");

  Wire.begin(8, 9, 400000);

  if (!bme.begin(0x76)) {
    Serial.println("BME280 not found!");
    while (true);
  }

  Serial.println("BME280 initialized.");

  connectWiFi();
}

void loop() {

  sendJson();

  delay(SEND_INTERVAL);
}
