///
/// @file BADC.cpp
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-04
///
/// @copyright Copyright (c) 2021
///
///

#include "BADC.h"

#if 1 //defined(NRF52_SERIES)
#ifdef __cplusplus
extern "C"
{
#endif
	void SAADC_IRQHandler()
	{
		if (NRF_SAADC->EVENTS_STARTED)
		{
			NRF_SAADC->EVENTS_STARTED = 0;
			NRF_SAADC->TASKS_SAMPLE = 0x01UL;
		}
		if (NRF_SAADC->EVENTS_END)
		{
			NRF_SAADC->EVENTS_END = 0;
			NRF_SAADC->TASKS_STOP = 0x01UL;
		}
		if (NRF_SAADC->EVENTS_STOPPED)
		{
			NRF_SAADC->EVENTS_STOPPED = 0;
			BADC._callback(BADC._results, BADC._ch_count);
			BADC.convDone = true;
		}
	}
#ifdef __cplusplus
}
#endif

void BADCClass::sartScanMode(uint8_t *ch_ids, uint8_t ch_count, BADC_Callback callback)
{
	this->_callback = callback;
	this->_ch_count = ch_count;
	// configure each channel
	for (int i = 0; i < 8; i++)
	{
		if (i < ch_count)
		{
			NRF_SAADC->CH[i].PSELN = ch_ids[i];
			NRF_SAADC->CH[i].PSELP = ch_ids[i];
			NRF_SAADC->CH[i].CONFIG = ((SAADC_CH_CONFIG_RESP_Bypass << SAADC_CH_CONFIG_RESP_Pos) & SAADC_CH_CONFIG_RESP_Msk) |		   // disable +res bypass
									  ((SAADC_CH_CONFIG_RESP_Bypass << SAADC_CH_CONFIG_RESN_Pos) & SAADC_CH_CONFIG_RESN_Msk) |		   // disable -res bypass
									  ((SAADC_CH_CONFIG_GAIN_Gain1_6 << SAADC_CH_CONFIG_GAIN_Pos) & SAADC_CH_CONFIG_GAIN_Msk) |		   // set gain to 1/6 => 3.3/6 => 0.55
									  ((SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) & SAADC_CH_CONFIG_REFSEL_Msk) | // internal ref 0.6
									  ((SAADC_CH_CONFIG_TACQ_40us << SAADC_CH_CONFIG_TACQ_Pos) & SAADC_CH_CONFIG_TACQ_Msk) |		   // taq 40us
									  ((SAADC_CH_CONFIG_MODE_SE << SAADC_CH_CONFIG_MODE_Pos) & SAADC_CH_CONFIG_MODE_Msk);			   // single ended
		}
		else
		{
			NRF_SAADC->CH[i].PSELN = 0;
			NRF_SAADC->CH[i].PSELP = 0;
		}
	}
	//
	NRF_SAADC->RESOLUTION = SAADC_RESOLUTION_VAL_12bit;
	NRF_SAADC->SAMPLERATE = ((SAADC_SAMPLERATE_MODE_Task << SAADC_SAMPLERATE_MODE_Pos) |
							 (SAADC_SAMPLERATE_CC_Msk << SAADC_SAMPLERATE_CC_Pos));
	//
	NRF_SAADC->INTEN = ((SAADC_INTEN_STARTED_Enabled << SAADC_INTEN_STARTED_Pos) |
						(SAADC_INTEN_STOPPED_Enabled << SAADC_INTEN_STOPPED_Pos) |
						(SAADC_INTEN_END_Enabled << SAADC_INTEN_END_Pos));
	NRF_SAADC->INTENSET = ((SAADC_INTENSET_STARTED_Enabled << SAADC_INTENSET_STARTED_Pos) |
						   (SAADC_INTENSET_STOPPED_Enabled << SAADC_INTENSET_STOPPED_Pos) |
						   (SAADC_INTENSET_END_Enabled << SAADC_INTENSET_END_Pos));
	//
	NRF_SAADC->ENABLE = (SAADC_ENABLE_ENABLE_Enabled << SAADC_ENABLE_ENABLE_Pos);
	//
	NVIC_ClearPendingIRQ(SAADC_IRQn);
	NVIC_SetPriority(SAADC_IRQn, 3);
	NVIC_EnableIRQ(SAADC_IRQn);
	//
	// while (!NRF_SAADC->EVENTS_STARTED)
	// 	;
	// NRF_SAADC->EVENTS_STARTED = 0x00UL;
	// NRF_SAADC->TASKS_SAMPLE = 0x01UL;
}

void BADCClass::startSampling()
{
	BADC.convDone = false;
	NRF_SAADC->RESULT.PTR = (uint32_t)_results; //
	NRF_SAADC->RESULT.MAXCNT = _ch_count;		//
	NRF_SAADC->TASKS_START = 0x01UL;
}

float BADCClass::getVoltsFromAdcVal(int16_t val)
{
	long volts = 0;
	if (val > 0)
	{
		volts = val;
		volts *= 3600;
		volts >>= 12;
	}
	return (float)(volts / 1000.0);
}

float BADCClass::readTemperature()
{
	NRF_TEMP->TASKS_START = 0x01UL;	  // start temp calculation
	while (!NRF_TEMP->EVENTS_DATARDY) // wait for data
	{
	}
	NRF_TEMP->EVENTS_DATARDY = 0x00UL; // clear event for future
	NRF_TEMP->TASKS_STOP = 0x01UL;	   // stop calculations

	return ((float)NRF_TEMP->TEMP) / 4.0;
}

BADCClass BADC;

#endif
