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
#include <BLPCOMP.h>

uint8_t _state = 0;
void crossHandle(uint8_t state)
{
    _state = state;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("LPCOMP START");
    delay(1000);
    BLPCOMP.startComparing(LPCOMP_PSEL_AIN0, LPCOMP_NSEL_REF_4_8Vdd, crossHandle);
}

void loop()
{
    Serial.print(_state ? "High ->" : "Low  ->");
    Serial.println("detected");
    __WFE();
    __WFI();
}