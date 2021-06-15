///
/// @file BADC.h
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-04
///
/// @copyright Copyright (c) 2021
///
///

#ifndef BADC_h
#define BADC_h

#include "Arduino.h"

#define BADC_ADC_PIN_2 (1UL)   /*!< AIN0 */
#define BADC_ADC_PIN_3 (2UL)   /*!< AIN1 */
#define BADC_ADC_PIN_4 (3UL)   /*!< AIN2 */
#define BADC_ADC_PIN_5 (4UL)   /*!< AIN3 */
#define BADC_ADC_PIN_28 (5UL)  /*!< AIN4 */
#define BADC_ADC_PIN_29 (6UL)  /*!< AIN5 */
#define BADC_ADC_PIN_30 (7UL)  /*!< AIN6 */
#define BADC_ADC_PIN_31 (8UL)  /*!< AIN7 */
#define BADC_ADC_PIN_VDD (9UL) /*!< VDD */

#ifdef __cplusplus
extern "C"
{
#endif
#include "nrf_nvic.h"
#ifdef __cplusplus
}
#endif

typedef void (*BADC_Callback)(int16_t *result, uint8_t result_cnt);

class BADCClass
{
public:
	BADC_Callback _callback;
	uint8_t _ch_count;
	int16_t _results[8];
	bool convDone;

	void sartScanMode(uint8_t *ch_ids, uint8_t ch_count, BADC_Callback callback);

	void startSampling();

	float getVoltsFromAdcVal(int16_t val);

	float readTemperature();

private:
};

extern BADCClass BADC;

#endif //NFC_h