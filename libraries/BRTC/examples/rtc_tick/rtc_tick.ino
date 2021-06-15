///
/// @file rtc_tick.ino
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-09
///
/// @copyright Copyright (c) 2021
///
///
#include <BRTC.h>

bool tick = false;
uint8_t tickCount = 0;
bool timer1expired = false;

void tickHandle()
{
    tick = true;
    tickCount++;
}

void timer1Expired()
{
    timer1expired = true;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("RTC START");
    delay(1000);
    BRTC.setPeriodicTimer(10000, tickHandle);
}

void loop()
{
    if (tick)
    {
        tick = false;
        Serial.println("tick");
        if (tickCount >= 2)
        {
            tickCount = 0;
            BRTC.setOneshotTimer(RTC_TIMER_CHANNEL_ONE_SHOT_1, 2000, timer1Expired);
        }
    }
    if (timer1expired)
    {
        timer1expired = false;
        Serial.println("timer1expired");
    }
    __WFE();
    __WFI();
}