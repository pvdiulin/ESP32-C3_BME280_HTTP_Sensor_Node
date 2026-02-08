#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

void connectWiFi() {

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  WiFi.config(
    INADDR_NONE,
    INADDR_NONE,
    INADDR_NONE,
    IPAddress(8, 8, 8, 8),
    IPAddress(1, 1, 1, 1)
  );

  Serial.println(WiFi.localIP());
}

void sendJson() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi reconnect...");
    WiFi.reconnect();
    delay(1000);
    return;
  }

  StaticJsonDocument<200> doc;
  doc["device"] = "esp32-c3";
  doc["uptime"] = millis();
  doc["freeHeap"] = ESP.getFreeHeap();

  String payload;
  serializeJson(doc, payload);

  for (int attempt = 1; attempt <= 3; attempt++) {

    Serial.printf("\nAttempt %d\n", attempt);

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

  connectWiFi();
}

void loop() {
  sendJson();
  delay(SEND_INTERVAL);
}