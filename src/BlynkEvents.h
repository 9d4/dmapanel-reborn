#include "BlynkSimpleEsp8266_SSL.h"
#include "SoftwareSerial.h"

// used to define wheter device will follow server values or not
// 1 is true
#define SERVER_BASED_VPIN V100

const uint8_t SERIAL_COMM_TERMINATOR = 255;

struct SerialCommand
{
    char cmd;
    char arg1;
    char arg2;
};

struct PinActuatorMap
{
    uint8_t virtualPin;
    uint8_t actuatorPin;
    bool analog;
};

SoftwareSerial as; // actuator serial
PinActuatorMap paMap[] = {
    {.virtualPin = 0, .actuatorPin = 2},
    {.virtualPin = 1, .actuatorPin = 3},
    {.virtualPin = 2, .actuatorPin = 4},
    {.virtualPin = 3, .actuatorPin = 5},
    {.virtualPin = 4, .actuatorPin = 7},
    {.virtualPin = 5, .actuatorPin = 8},
    {.virtualPin = 6, .actuatorPin = 9},
    {.virtualPin = 7, .actuatorPin = 10},
    {.virtualPin = 8, .actuatorPin = 11},
    {.virtualPin = 9, .actuatorPin = 12},
    {.virtualPin = 10, .actuatorPin = 13},
    {.virtualPin = 11, .actuatorPin = 14},
    {.virtualPin = 12, .actuatorPin = 15},
    {.virtualPin = 13, .actuatorPin = 16},
    {.virtualPin = 14, .actuatorPin = 17},
    {.virtualPin = 15, .actuatorPin = 18},
    {.virtualPin = 16, .actuatorPin = 19},
};
bool ServerValue = false;

void sendToActuator(SerialCommand sc)
{
    as.write(sc.cmd);
    as.write(sc.arg1);
    as.write(sc.arg2);
    as.write(SERIAL_COMM_TERMINATOR);
}

void beginEvents()
{
    as.begin(9600, SWSERIAL_8N1, D1, D2);
    as.write('C');
    as.write(SERIAL_COMM_TERMINATOR);
}

void handleBlynkWrite(uint8_t pin, uint8_t value)
{
    uint8_t paMapLen = sizeof(paMap) / sizeof(PinActuatorMap);

    for (uint8_t i = 0; i < paMapLen; i++)
    {
        PinActuatorMap *pa = &paMap[i];
        if (pin == pa->virtualPin)
        {
            SerialCommand sc;
            sc.cmd = 0x53; // S

            if (pa->analog) {
                sc.cmd = 0x41; // A
            }

            sc.arg1 = pa->actuatorPin;
            sc.arg2 = value;
            sendToActuator(sc);
            break;
        }
    }
}

BLYNK_WRITE_DEFAULT()
{
    uint8_t pin = request.pin;
    int32_t val = param.asInt();

    handleBlynkWrite(pin, val);
}

BLYNK_WRITE(SERVER_BASED_VPIN)
{
    int32_t val = param.asInt();
    if (!!val)
    {
        uint8_t paMapLen = sizeof(paMap) / sizeof(PinActuatorMap);

        for (uint8_t i = 0; i < paMapLen; i++)
        {
            Blynk.syncVirtual(paMap[i].virtualPin);
        }

        Serial.println("Synced with server value");
    }
}

BLYNK_CONNECTED()
{
    Blynk.syncVirtual(SERVER_BASED_VPIN);
}