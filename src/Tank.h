#ifndef TANK_H
#define TANK_H

const long tank_check_interval = 1000;
const long tank_notify_interval = 1000 * 3600 * 3;

void setupTank()
{
    pinMode(TANK_HIGH_PIN, INPUT_PULLUP);
    pinMode(TANK_LOW_PIN, INPUT_PULLUP);
    Serial.println("tank set!");
}

void runTank()
{
    static long lastRun;
    if (millis() - lastRun < tank_check_interval)
    {
        return;
    }
    lastRun = millis();

    // This is when the tank full. Stop the pump!
    if (digitalRead(TANK_HIGH_PIN) != TANK_PIN_PULL)
    {
        SerialCommand sc;
        sc.cmd = 0x53;
        sc.arg1 = 2;
        sc.arg2 = TANK_PUMP_VPIN_LOW;
        sendToActuator(sc);

        // We're offline sometimes. We do virtual write later
        Blynk.virtualWrite(TANK_PUMP_VPIN, TANK_PUMP_VPIN_LOW);
        return;
    }

    static long lastNotify;
    static bool firstRun = true;
    // Notify on low
    if (digitalRead(TANK_LOW_PIN) != TANK_PIN_PULL && digitalRead(TANK_HIGH_PIN) == TANK_PIN_PULL && (firstRun || millis() - lastNotify > tank_notify_interval))
    {
        Serial.println("notified: low tank");
        Blynk.logEvent("message", String("Tank capacity passes minimum low treshold"));
        lastNotify = millis();
        firstRun = false;
    }
}

#endif