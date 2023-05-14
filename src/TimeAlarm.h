#ifndef TIME_ALARM_H
#define TIME_ALARM_H

#include <NTPClient.h>
#include <TimeLib.h>
#include <AlarmSound.h>
#include <EEPROM.h>

typedef struct
{
    int hour;
    int minute;
    int second;
} Time;

typedef struct
{
    Time alarmTime;
    uint8_t active;
    bool playing;
    unsigned int duration;
    unsigned int playStartTime;
    unsigned int pauseUntil;
    bool paused;
    void (*handler)();
} Alarm;

// currently this code play for 5 mins.
// value less than a minute is useless, since the code
// will play again because the minute hasn't change yet.
const unsigned int playDuration = 5 * 60 * 1000;

// check if two times are equal
bool timeEquals(Time t1, Time t2)
{
    return (t1.hour == t2.hour && t1.minute == t2.minute);
}

// check if it's time for the alarm to go off
void run(Alarm *alarm)
{
    if (!alarm->active)
        return;

    Time t = {hour(), minute(), second()};

    if (alarm->playing)
    {
        if (millis() - alarm->playStartTime >= alarm->duration)
        {
            alarm->playing = false;
            alarm->paused = false;
            Serial.println("Alarm stopping...");
            return;
        }

        alarm->handler();

        return;
    } else {
        digitalWrite(ALARM_TRIGGER_GPIO, ALARM_TRIGGER_GPIO_DEFAULT);
    }

    if (timeEquals(alarm->alarmTime, t))
    {
        long dev = alarm->pauseUntil - millis();
        if (alarm->paused && dev > 0) {
            return;
        }
        alarm->playing = true;
        alarm->paused = false;
        alarm->playStartTime = millis();
        resetBeforePlay();
        Serial.printf("Alarm match time! %d:%d\n", hour(), minute());
    }
}

// example alarm handler function
void alarmHandler()
{
    play();
    digitalWrite(ALARM_TRIGGER_GPIO, ALARM_TRIGGER_GPIO_ACTIVE);
}

Alarm DefaultAlarm;

const unsigned int runInterval = 1000;
unsigned int lastRun = 0UL;
bool firstRun = true;

void runAlarms(NTPClient time)
{
    run(&DefaultAlarm);
    if (millis() - lastRun < runInterval)
    {
        return;
    }

    setTime(time.getHours(), time.getMinutes(), time.getSeconds(), time.getDay(), 0, 0);
    lastRun = millis();

    // get saved alarm
    EEPROM.begin(3096);
    uint8_t active = EEPROM.read(3089);
    uint8_t hour = EEPROM.read(3090);
    uint8_t minute = EEPROM.read(3091);
    EEPROM.end();

    DefaultAlarm.alarmTime = {hour, minute};
    DefaultAlarm.active = active == 1;

    if (firstRun)
    {
        DefaultAlarm = {.alarmTime = {12, 36, 00}, .active = true, .duration = playDuration, .handler = alarmHandler};
        pinMode(ALARM_TRIGGER_GPIO, OUTPUT);
        digitalWrite(ALARM_TRIGGER_GPIO, ALARM_TRIGGER_GPIO_DEFAULT);
        Serial.println("Alarm registered!");
        firstRun = false;
    }
}

#endif
