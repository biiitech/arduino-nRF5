///
/// @file BPWM.cpp
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-09
///
/// @copyright Copyright (c) 2021
///
///
#include "BPWM.h"

BPWMClass BPWM;

int BPWMClass::begin()
{
    for (int i = 0; i < eBPWM_MAX; i++) {
        this->_stm[i].hw->PRESCALER = PWM_PRESCALER_PRESCALER_DIV_16; // 1 us
        this->_stm[i].hw->MODE = (PWM_MODE_UPDOWN_Up << PWM_MODE_UPDOWN_Pos);
        this->_stm[i].hw->DECODER = (PWM_DECODER_LOAD_Individual << PWM_DECODER_LOAD_Pos) | (PWM_DECODER_MODE_RefreshCount << PWM_DECODER_MODE_Pos);
        this->_stm[i].hw->LOOP = (PWM_LOOP_CNT_Disabled << PWM_LOOP_CNT_Pos);
        this->_stm[i].hw->SEQ[0].CNT = (eBPWM_CH_MAX << PWM_SEQ_CNT_CNT_Pos);
        this->_stm[i].hw->SEQ[0].ENDDELAY = 0;
        this->_stm[i].hw->SEQ[0].PTR = (uint32_t) & this->_stm[i].ch_us_pulse[0];
        this->_stm[i].hw->SEQ[0].REFRESH = 0;
        this->_stm[i].hw->SHORTS = 0;
    }
}

int BPWMClass::setConfig(enum bpwm_hw_id hw_id, enum bpwm_ch_id ch_id, uint8_t pwm_pin, uint16_t period_us, bool inverted)
{
    if ((hw_id >= eBPWM_MAX) || (ch_id >= eBPWM_CH_MAX)) {
        return -1;
    }
    if (this->_stm[hw_id].ch_cfg[ch_id].is_active) {
        return -2;
    }
    this->_stm[hw_id].hw->COUNTERTOP = period_us;
    this->_stm[hw_id].hw->PSEL.OUT[ch_id] = pwm_pin;
    this->_stm[hw_id].ch_cfg[ch_id].is_inverted = inverted;
    return 0;
}

int BPWMClass::start(enum bpwm_hw_id hw_id, enum bpwm_ch_id ch_id, uint16_t pulse_us)
{
    if ((hw_id >= eBPWM_MAX) || (ch_id >= eBPWM_CH_MAX)) {
        return -1;
    }
    if (this->_stm[hw_id].ch_cfg[ch_id].is_inverted) {
        this->_stm[hw_id].ch_us_pulse[ch_id] = pulse_us | (1 << 15);
    } else {
        this->_stm[hw_id].ch_us_pulse[ch_id] = pulse_us;
    }
    // if any one is running then we need not to restart it
    for (int i = 0; i < eBPWM_CH_MAX; i++) {
        if (this->_stm[hw_id].ch_cfg[i].is_active) {
            this->_stm[hw_id].ch_cfg[ch_id].is_active = true;
            return 0;
        }
    }
    // start hw
    this->_stm[hw_id].hw->ENABLE = 1;
    this->_stm[hw_id].hw->TASKS_SEQSTART[0] = 1;
    this->_stm[hw_id].ch_cfg[ch_id].is_active = true;
    return 0;
}

int BPWMClass::stop(enum bpwm_hw_id hw_id, enum bpwm_ch_id ch_id)
{
    if ((hw_id >= eBPWM_MAX) || (ch_id >= eBPWM_CH_MAX)) {
        return -1;
    }
    this->_stm[hw_id].ch_us_pulse[ch_id] = 0;
    this->_stm[hw_id].ch_cfg[ch_id].is_active = false;
    // if any one is running then we need not to restart it
    for (int i = 0; i < eBPWM_CH_MAX; i++) {
        if (this->_stm[hw_id].ch_cfg[i].is_active) {
            return 0;
        }
    }
    this->_stm[hw_id].hw->TASKS_STOP = 1;
    this->_stm[hw_id].hw->ENABLE = 0;
    return 0;
}
