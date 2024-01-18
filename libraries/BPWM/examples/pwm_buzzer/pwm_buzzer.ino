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
#include <BPWM.h>

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("PWM START");
    BPWM.begin();
    BPWM.setConfig(eBPWM_0, eBPWM_CH_0, 11, 1000ul, false);
}

void loop()
{
    BPWM.start(eBPWM_0, eBPWM_CH_0, 500ul);
    delay(1000);
    BPWM.stop(eBPWM_0, eBPWM_CH_0);
    delay(1000);
}