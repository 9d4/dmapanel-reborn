
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLr7lELXbt"
#define BLYNK_DEVICE_NAME "dmapanel"

#define BLYNK_FIRMWARE_VERSION "1.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

// Terminal widget datastream
#define VIRTPIN_TERMINAL V99

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

#include "BlynkEdgent.h"
#include "BlynkEvents.h"
#include "LocalOTA.h"
#include "MQTT.h"

void setup()
{
  Serial.begin(9600);
  delay(100);

  BlynkEdgent.begin();
  beginEvents();
  localOTASetup();
  mqttBegin();
}

void configTimeRun() {
  time_t now = time(nullptr);
  if (now < 100000 && WiFi.status() == WL_CONNECTED) {
    // Synchronize time useing SNTP. This is necessary to verify that
    // the TLS certificates offered by the server are currently valid
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print("Waiting for NTP time sync: ");
    while (now < 100000) {
        delay(100);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println();
    Serial.println("time synced");
  }
}

void loop()
{
  BlynkEdgent.run();
  configTimeRun();
  localOTALoop();
  mqttLoop();
}
