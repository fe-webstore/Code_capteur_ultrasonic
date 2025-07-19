#include "SmartESP32Utils.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "N1";
const char* password = "yannyann";

// ------ variables globales ----------
AsyncWebServer server(80);
String lastCommand = "";
AutoUpdatePayload payload;
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 500;

// ------- Broches --------


const int ULTRA_TRIG = 5;
const int ULTRA_ECHO = 18;

// ------- Variables lecture capteurs --------

float distanceCM = 0;

// Pourcentages


void setup() {
  Serial.begin(115200);
  connectToWiFi(ssid, password);
  setupWebSocket(server, &lastCommand);

  pinMode(ULTRA_TRIG, OUTPUT);
  pinMode(ULTRA_ECHO, INPUT);

  
}

void loop() {
  Smartcommande();
  Autoupdate();
}

void Smartcommande() {
  if (!lastCommand.isEmpty()) {
    lastCommand = "";
    sendAutoUpdate(payload);
    payload.notif = "false";
    payload.ia = "false";
  }
}

void Autoupdate() {
  if (millis() - lastUpdateTime > updateInterval) {
    
    lireUltrason();

   
    payload.ecran4 = String(distanceCM, 2);

   


    sendAutoUpdate(payload);
    lastUpdateTime = millis();
  }
}

void lireUltrason() {
  digitalWrite(ULTRA_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRA_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_TRIG, LOW);

  long duration = pulseIn(ULTRA_ECHO, HIGH, 30000);  // timeout de 30ms
  distanceCM = duration * 0.034 / 2.0;

  if (duration == 0) {
    distanceCM = -1;  // valeur invalide
    Serial.println("Ultrason : Aucune lecture");
  } else {
    Serial.print("Distance : ");
    Serial.print(distanceCM, 2);
    Serial.println(" cm");
  }
}
