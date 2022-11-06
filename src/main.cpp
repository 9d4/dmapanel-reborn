
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLr7lELXbt"
#define BLYNK_DEVICE_NAME "dmapanel"

#define BLYNK_FIRMWARE_VERSION "0.1.2"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include "BlynkEvents.h"

void setup()
{
  Serial.begin(9600);
  delay(100);

  BlynkEdgent.begin();
  beginEvents();
}

void loop()
{
  BlynkEdgent.run();
}
