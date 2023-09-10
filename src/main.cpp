
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

// PIN to be triggerred when alarm is playing.
#define ALARM_TRIGGER_GPIO D7
// Default state
#define ALARM_TRIGGER_GPIO_DEFAULT HIGH
#define ALARM_TRIGGER_GPIO_ACTIVE LOW

// TANK
#define TANK_HIGH_PIN D5
#define TANK_LOW_PIN D6
#define TANK_PIN_DEFAULT LOW
#define TANK_PIN_ACTIVE HIGH
#define TANK_PIN_PULL HIGH

#define TANK_PUMP_PIN 2
#define TANK_PUMP_VPIN 0
#define TANK_PUMP_VPIN_HIGH 0
#define TANK_PUMP_VPIN_LOW 1

#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#include "BlynkEdgent.h"
#include "BlynkEvents.h"
#include "TimeAlarm.h"
#include "Tank.h"

void setup()
{
  Serial.begin(9600);
  delay(100);

  BlynkEdgent.begin();
  beginEvents();
  timeClient.begin();
  timeClient.setTimeOffset(7 * 60 * 60); // TODO: dynamic value and save to EEPROM instead
  timeClient.setUpdateInterval(60 * 1000);

  // ============================================
  // TANK CAPACITY METER
  // ============================================
  setupTank();
}

void loop()
{
  BlynkEdgent.run();
  timeClient.update();

  if (timeClient.isTimeSet()) {
    runAlarms(timeClient);
  }
  runTank();
}
