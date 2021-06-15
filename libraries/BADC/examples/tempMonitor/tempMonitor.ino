///
/// @file tempMnitor.ino
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-05
///
/// @copyright Copyright (c) 2021
///
///
#include <BADC.h>

void setup()
{
    Serial.begin(115200);
    Serial.println("Temp Start");
}

void loop()
{
    delay(1000);
    Serial.print("Temp = ");
    Serial.println(BADC.readTemperature());
}