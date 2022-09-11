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

volatile bool tick = false;
uint8_t tickCount = 0;
bool timer1expired = false;

void tickHandle()
{
    tick = true;
    tickCount++;
    Serial.print(tickCount);
    Serial.println(":tick");
}

void timer1Expired()
{
    timer1expired = true;
    Serial.println("timer1expired");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("RTC START");
    BRTC.setPeriodicTimer(20 * 60 * 1000, tickHandle, true);
}

void loop()
{
    if (tick)
    {
        tick = false;
        if ((tickCount & 0x03) == 0x00)
        {
            BRTC.setOneshotTimer(RTC_TIMER_CHANNEL_ONE_SHOT_1, 2000, timer1Expired, true);
        }
    }
    if (timer1expired)
    {
        timer1expired = false;
    }
    __WFE();
    __WFI();
}