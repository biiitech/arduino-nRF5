///
/// @file vddMonitor.ino
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-05
///
/// @copyright Copyright (c) 2021
///
///
#include <BADC.h>

uint8_t adc_ch_seq[] = {BADC_ADC_PIN_VDD};
float vdd;

void adc_seq_Callback(int16_t *results, uint8_t result_cnt)
{
    vdd = BADC.getVoltsFromAdcVal(results[0]);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("BADC Scan Start");
    BADC.sartScanMode(adc_ch_seq, sizeof(adc_ch_seq), adc_seq_Callback);
}

void loop()
{
    BADC.startSampling();
    delay(500);
    if (BADC.convDone)
    {
        Serial.print("VDD = ");
        Serial.println(vdd);
    }
}