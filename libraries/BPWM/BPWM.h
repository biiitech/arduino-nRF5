///
/// @file BPWM.h
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-09
///
/// @copyright Copyright (c) 2021
///
///
#ifndef BPWM_h
#define BPWM_h

#include "Arduino.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

enum bpwm_hw_id {
    eBPWM_0,
    eBPWM_1,
    eBPWM_2,
    eBPWM_MAX,
};

enum bpwm_ch_id {
    eBPWM_CH_0,
    eBPWM_CH_1,
    eBPWM_CH_2,
    eBPWM_CH_3,
    eBPWM_CH_MAX,
};

class BPWMClass {
public:
    int begin();
    int setConfig(enum bpwm_hw_id hw_id, enum bpwm_ch_id ch_id, uint8_t pwm_pin, uint16_t period_us, bool inverted);
    int start(enum bpwm_hw_id hw_id, enum bpwm_ch_id ch_id, uint16_t pulse_us);
    int stop(enum bpwm_hw_id hw_id, enum bpwm_ch_id ch_id);

private:
    struct {
        NRF_PWM_Type *hw;
        struct {
            bool is_active;
            bool is_inverted;
        } ch_cfg[eBPWM_CH_MAX];
        uint16_t ch_us_pulse[eBPWM_CH_MAX];
        uint16_t us_period;
    } _stm[eBPWM_MAX] = {
        {.hw = NRF_PWM0},
        {.hw = NRF_PWM1},
        {.hw = NRF_PWM2},
    };
};

extern BPWMClass BPWM;

#endif