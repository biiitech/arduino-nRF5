///
/// @file BRTC.h
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-09
///
/// @copyright Copyright (c) 2021
///
///
#ifndef BRTC_h
#define BRTC_h

#include "Arduino.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include "nrf_nvic.h"
#ifdef __cplusplus
}
#endif

typedef enum
{
    RTC_TIMER_CHANNEL_PERIODIC = 0,
    RTC_TIMER_CHANNEL_ONE_SHOT_1,
    RTC_TIMER_CHANNEL_ONE_SHOT_2,
    RTC_TIMER_CHANNEL_ONE_SHOT_3,
    RTC_TIMER_CHANNEL_ONE_SHOT_MAX,
} rtc_timer_ch_ids;

typedef void (*f_TickCallback)(void);

class BRTCClass
{
public:
    struct
    {
        struct
        {
            uint32_t ms_delay;
            uint32_t ms_tick;
        } ch[RTC_TIMER_CHANNEL_ONE_SHOT_MAX];
        f_TickCallback _tickCallback[RTC_TIMER_CHANNEL_ONE_SHOT_MAX];
    } rtc_stm;

    ///
    /// @brief Set the Periodic Tick object
    ///
    /// @param msPeriod
    /// @param tickCallback
    /// @return true
    /// @return false
    ///
    bool setPeriodicTimer(uint32_t msPeriod, f_TickCallback periodicCallback, bool debug = false);

    ///
    /// @brief Set the Oneshot Timer tick handler
    ///
    /// @param ch_id channel id to be used to set this timer interrupt
    /// @param msDelay ms delay from current time after which tickCallback should get called
    /// @param tickCallback function to be called after expiry of
    /// @return true
    /// @return false
    ///
    bool setOneshotTimer(rtc_timer_ch_ids ch_id, uint32_t msDelay, f_TickCallback oneshotCallback, bool debug = false);

    ///
    /// @brief
    ///
    /// @param ch_id
    /// @return true
    /// @return false
    ///
    bool cancelOneshotTimer(rtc_timer_ch_ids ch_id);

private:
    ///
    /// @brief
    ///
    /// @return true
    /// @return false
    ///
    uint64_t getTickCount(uint32_t ms, uint32_t prescaller);
    bool updateTickCounts(rtc_timer_ch_ids ch_id, uint32_t ms, bool debug = false);
};

extern BRTCClass BRTC;

#endif