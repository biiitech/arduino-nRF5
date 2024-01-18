///
/// @file AdcScan.ino
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-05
///
/// @copyright Copyright (c) 2021
///
///

#include <BADC.h>

uint8_t adc_ch_seq[] = {BADC_ADC_PIN_3, BADC_ADC_PIN_4};
int16_t adc_results[sizeof(adc_ch_seq)];
uint8_t adc_result_cnt = 0;

void adc_seq_Callback(int16_t *results, uint8_t result_cnt)
{
  memcpy(adc_results, results, result_cnt * sizeof(results[0]));
  adc_result_cnt = result_cnt;
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
    Serial.print("ADC Val\t");
    for (int i = 0; i < adc_result_cnt; i++)
    {
      Serial.print(BADC.getVoltsFromAdcVal(adc_results[i]));
      Serial.print(",\t");
    }
    Serial.println();
  }
}