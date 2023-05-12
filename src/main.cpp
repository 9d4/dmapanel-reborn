
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLr7lELXbt"
#define BLYNK_DEVICE_NAME "dmapanel"

#define BLYNK_FIRMWARE_VERSION "1.0.0"

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

#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#include "BlynkEdgent.h"
#include "BlynkEvents.h"
#include "TimeAlarm.h"

void setup()
{
  Serial.begin(9600);
  delay(100);

  BlynkEdgent.begin();
  beginEvents();
  timeClient.begin();
  timeClient.setTimeOffset(7 * 60 * 60); // TODO: dynamic value and save to EEPROM instead
  timeClient.setUpdateInterval(60 * 1000);
}

void loop()
{
  BlynkEdgent.run();
  timeClient.update();

  if (timeClient.isTimeSet()) {
    runAlarms(timeClient);
  }
}
