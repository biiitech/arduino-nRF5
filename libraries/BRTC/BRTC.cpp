///
/// @file BRTC.cpp
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-09
///
/// @copyright Copyright (c) 2021
///
///
#include "BRTC.h"

BRTCClass BRTC;

#define RTC_MAX_TICK_COUNT ((1ul << 24ul) - 128ul)
#define RTC_MIN_TICK_COUNT (8ul)

#ifdef __cplusplus
extern "C"
{
#endif
    void RTC2_IRQHandler()
    {
        if (NRF_RTC2->EVENTS_COMPARE[RTC_TIMER_CHANNEL_PERIODIC])
        {
            NRF_RTC2->EVENTS_COMPARE[RTC_TIMER_CHANNEL_PERIODIC] = 0;
            NRF_RTC2->CC[RTC_TIMER_CHANNEL_PERIODIC] += BRTC.rtc_stm.ch[RTC_TIMER_CHANNEL_PERIODIC].ms_tick;
            if (BRTC.rtc_stm._tickCallback[RTC_TIMER_CHANNEL_PERIODIC])
            {
                BRTC.rtc_stm._tickCallback[RTC_TIMER_CHANNEL_PERIODIC]();
            }
        }
        else
        {
            for (int chid = RTC_TIMER_CHANNEL_ONE_SHOT_1; chid < RTC_TIMER_CHANNEL_ONE_SHOT_MAX; chid++)
            {
                if (NRF_RTC2->EVENTS_COMPARE[chid])
                {
                    NRF_RTC2->EVENTS_COMPARE[chid] = 0;
                    if (BRTC.rtc_stm._tickCallback[chid])
                    {
                        BRTC.rtc_stm._tickCallback[chid]();
                    }
                    NRF_RTC2->INTENCLR = ((RTC_INTENSET_COMPARE0_Enabled << (RTC_INTENSET_COMPARE0_Pos + chid)));
                }
            }
        }
    }
#ifdef __cplusplus
}
#endif

uint64_t BRTCClass::getTickCount(uint32_t msPeriod)
{
    uint64_t ticks = msPeriod;
    ticks *= 32768;
    ticks /= 1000;
    return ticks;
}

bool BRTCClass::updateTickCounts(rtc_timer_ch_ids ch_id, uint32_t msPeriod, bool debug)
{
    bool error = false;
    this->rtc_stm.ch[ch_id].ms_delay = msPeriod;
    uint32_t min_ms = 0xFFFFFFFFul;
    uint32_t max_ms = 0;
    // update min and max delay
    for (int chid = 0; chid < RTC_TIMER_CHANNEL_ONE_SHOT_MAX; chid++)
    {
        if (this->rtc_stm.ch[chid].ms_delay > 0)
        {
            if (this->rtc_stm.ch[chid].ms_delay < min_ms)
            {
                min_ms = this->rtc_stm.ch[chid].ms_delay;
            }
            if (this->rtc_stm.ch[chid].ms_delay > max_ms)
            {
                max_ms = this->rtc_stm.ch[chid].ms_delay;
            }
        }
    }
    error = (min_ms > max_ms);
    if (!error)
    {
        // setup nrf rtc to tick at ms provided
        uint64_t tick_in_prescaller = 0;
        uint64_t ticks = getTickCount(max_ms);
        uint32_t prescaller = 0;
        do
        {
            prescaller++;
            tick_in_prescaller = ticks / prescaller;
        } while (tick_in_prescaller > RTC_MAX_TICK_COUNT);
        //
        if (debug)
        {
            Serial.print("PSR ");
            Serial.println(prescaller);
        }
        error = (prescaller & 0xF000);
        if (!error)
        {
            for (int chid = 0; chid < RTC_TIMER_CHANNEL_ONE_SHOT_MAX; chid++)
            {
                if (this->rtc_stm.ch[chid].ms_delay > 0)
                {
                    this->rtc_stm.ch[chid].ms_tick = getTickCount(this->rtc_stm.ch[chid].ms_delay);
                    //
                    if (debug)
                    {
                        Serial.print(chid);
                        Serial.print(": MS>");
                        Serial.print(this->rtc_stm.ch[chid].ms_delay);
                        Serial.print(" TK>");
                        Serial.println(this->rtc_stm.ch[chid].ms_tick, HEX);
                    }
                    //
                    if (this->rtc_stm.ch[chid].ms_tick < RTC_MIN_TICK_COUNT)
                    {
                        this->rtc_stm.ch[chid].ms_delay = 0;
                        error = true;
                    }
                }
            }
            // everything is okay till now update all channels
            if (!error)
            {
                NRF_RTC2->PRESCALER = prescaller - 1;
                for (int chid = 0; chid < RTC_TIMER_CHANNEL_ONE_SHOT_MAX; chid++)
                {
                    if (this->rtc_stm.ch[chid].ms_delay > 0)
                    {
                        NRF_RTC2->CC[chid] = NRF_RTC2->COUNTER + this->rtc_stm.ch[chid].ms_tick;
                        NRF_RTC2->INTENSET = ((RTC_INTENSET_COMPARE0_Enabled << (RTC_INTENSET_COMPARE0_Pos + chid)));
                    }
                    else
                    {
                        NRF_RTC2->INTENCLR = ((RTC_INTENSET_COMPARE0_Enabled << (RTC_INTENSET_COMPARE0_Pos + chid)));
                    }
                }
            }
            else
            {
                if (debug)
                {
                    Serial.println("RTC: RNG Over");
                }
            }
        }
        else
        {
            if (debug)
            {
                Serial.println("RTC: PSR Over");
            }
        }
    }
    else
    {
        if (debug)
        {
            Serial.print("RTC: mM ERR: ");
            Serial.print(min_ms);
            Serial.print(" ");
            Serial.print(max_ms);
            Serial.println();
        }
    }
    return !error;
}

bool BRTCClass::setPeriodicTimer(uint32_t msPeriod, f_TickCallback periodicCallback, bool debug)
{
    NRF_RTC2->TASKS_STOP = 1;
    if (updateTickCounts(RTC_TIMER_CHANNEL_PERIODIC, msPeriod, debug))
    {
        this->rtc_stm._tickCallback[RTC_TIMER_CHANNEL_PERIODIC] = periodicCallback;
        //
        NVIC_ClearPendingIRQ(RTC2_IRQn);
        NVIC_SetPriority(RTC2_IRQn, 3);
        NVIC_EnableIRQ(RTC2_IRQn);
        NRF_RTC2->TASKS_START = 1;
        return true;
    }
    NRF_RTC2->TASKS_START = 1;
    return false;
}

bool BRTCClass::setOneshotTimer(rtc_timer_ch_ids ch_id, uint32_t msDelay, f_TickCallback oneshotCallback, bool debug)
{
    if ((ch_id > RTC_TIMER_CHANNEL_PERIODIC) &&
        (ch_id < RTC_TIMER_CHANNEL_ONE_SHOT_MAX))
    {
        NRF_RTC2->TASKS_STOP = 1;
        if (updateTickCounts(ch_id, msDelay, debug))
        {
            this->rtc_stm._tickCallback[ch_id] = oneshotCallback;
            //
            NVIC_ClearPendingIRQ(RTC2_IRQn);
            NVIC_SetPriority(RTC2_IRQn, 3);
            NVIC_EnableIRQ(RTC2_IRQn);
            NRF_RTC2->TASKS_START = 1;
            return true;
        }
        NRF_RTC2->TASKS_START = 1;
    }
    return false;
}

bool BRTCClass::cancelOneshotTimer(rtc_timer_ch_ids ch_id)
{
    if ((ch_id > RTC_TIMER_CHANNEL_PERIODIC) &&
        (ch_id < RTC_TIMER_CHANNEL_ONE_SHOT_MAX))
    {
        NRF_RTC2->TASKS_STOP = 1;
        NRF_RTC2->INTENCLR = ((RTC_INTENSET_COMPARE0_Enabled << (RTC_INTENSET_COMPARE0_Pos + ch_id)));
        NRF_RTC2->TASKS_START = 1;
    }
    return false;
}