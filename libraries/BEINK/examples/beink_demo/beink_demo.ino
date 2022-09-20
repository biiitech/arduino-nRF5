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
#include <BEINK.h>

void setup()
{
    Serial.begin(115200);
    delay(5000);
    BEINK.turn_on();
    Serial.println("BEINK START");
    Serial.println("setting white");
    BEINK.clear(color_white);
    Serial.println("white ready");
    //
    BEINK.print_line(84, 104, "BIII Tech");
    BEINK.print_line(64, 104, "Kaizen");
    BEINK.print_line(44, 104, "for");
    BEINK.print_line(24, 104, "perfection");
    //
    BEINK.print_qr(0, 0, "https://biii.in");
    BEINK.update();
    Serial.println("done");
}

void loop()
{
}
