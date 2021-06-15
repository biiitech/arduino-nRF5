///
/// @file BLPCOMP.cpp
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-09
///
/// @copyright Copyright (c) 2021
///
///
#include "BLPCOMP.h"

BLPCOMPClass BLPCOMP;

#ifdef __cplusplus
extern "C"
{
#endif
    void LPCOMP_IRQHandler()
    {
        if (NRF_LPCOMP->EVENTS_UP)
        {
            NRF_LPCOMP->EVENTS_UP = 0;
            BLPCOMP._crossCallback(1);
        }
        if (NRF_LPCOMP->EVENTS_DOWN)
        {
            NRF_LPCOMP->EVENTS_DOWN = 0;
            BLPCOMP._crossCallback(0);
        }
    }
#ifdef __cplusplus
}
#endif

bool BLPCOMPClass::startComparing(blpcomp_psel_pin_ids psel, blpcomp_nsel_ref_ids nsel, f_CrossCallback crossCallback)
{
    _crossCallback = crossCallback;
    //
    NRF_LPCOMP->PSEL = psel << LPCOMP_PSEL_PSEL_Pos;
    NRF_LPCOMP->REFSEL = (nsel & 0x0F) << LPCOMP_REFSEL_REFSEL_Pos;
    NRF_LPCOMP->EXTREFSEL = ((nsel >> 4) & 0x01) << LPCOMP_EXTREFSEL_EXTREFSEL_Pos;
    NRF_LPCOMP->HYST = LPCOMP_HYST_HYST_Hyst50mV << LPCOMP_HYST_HYST_Pos;
    NRF_LPCOMP->ANADETECT = LPCOMP_ANADETECT_ANADETECT_Cross << LPCOMP_ANADETECT_ANADETECT_Pos;
    //
    NRF_LPCOMP->INTENSET = ((LPCOMP_INTENSET_UP_Enabled << LPCOMP_INTENSET_UP_Pos) |
                            (LPCOMP_INTENSET_DOWN_Enabled << LPCOMP_INTENSET_DOWN_Pos));
    NRF_LPCOMP->ENABLE = LPCOMP_ENABLE_ENABLE_Enabled << LPCOMP_ENABLE_ENABLE_Pos;
    NRF_LPCOMP->SHORTS = LPCOMP_SHORTS_READY_SAMPLE_Enabled << LPCOMP_SHORTS_READY_SAMPLE_Pos;
    //
    NVIC_ClearPendingIRQ(LPCOMP_IRQn);
    NVIC_SetPriority(LPCOMP_IRQn, 3);
    NVIC_EnableIRQ(LPCOMP_IRQn);
    //
    NRF_LPCOMP->TASKS_START = 1;
}