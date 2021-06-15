///
/// @file BLPCOMP.h
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-09
///
/// @copyright Copyright (c) 2021
///
///
#ifndef BLPCOMP_h
#define BLPCOMP_h

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
    LPCOMP_PSEL_AIN0 = 0x00, /*!< AIN0 selected as analog input */
    LPCOMP_PSEL_AIN1 = 0x01, /*!< AIN1 selected as analog input */
    LPCOMP_PSEL_AIN2 = 0x02, /*!< AIN2 selected as analog input */
    LPCOMP_PSEL_AIN3 = 0x03, /*!< AIN3 selected as analog input */
    LPCOMP_PSEL_AIN4 = 0x04, /*!< AIN4 selected as analog input */
    LPCOMP_PSEL_AIN5 = 0x05, /*!< AIN5 selected as analog input */
    LPCOMP_PSEL_AIN6 = 0x06, /*!< AIN6 selected as analog input */
    LPCOMP_PSEL_AIN7 = 0x07  /*!< AIN7 selected as analog input */
} blpcomp_psel_pin_ids;

typedef enum
{
    LPCOMP_NSEL_REF_1_8Vdd = 0x00,   /*!< VDD * 1/8 selected as reference */
    LPCOMP_NSEL_REF_2_8Vdd = 0x01,   /*!< VDD * 2/8 selected as reference */
    LPCOMP_NSEL_REF_3_8Vdd = 0x02,   /*!< VDD * 3/8 selected as reference */
    LPCOMP_NSEL_REF_4_8Vdd = 0x03,   /*!< VDD * 4/8 selected as reference */
    LPCOMP_NSEL_REF_5_8Vdd = 0x04,   /*!< VDD * 5/8 selected as reference */
    LPCOMP_NSEL_REF_6_8Vdd = 0x05,   /*!< VDD * 6/8 selected as reference */
    LPCOMP_NSEL_REF_7_8Vdd = 0x06,   /*!< VDD * 7/8 selected as reference */
    LPCOMP_NSEL_REF_1_16Vdd = 0x08,  /*!< VDD * 1/16 selected as reference */
    LPCOMP_NSEL_REF_3_16Vdd = 0x09,  /*!< VDD * 3/16 selected as reference */
    LPCOMP_NSEL_REF_5_16Vdd = 0x0A,  /*!< VDD * 5/16 selected as reference */
    LPCOMP_NSEL_REF_7_16Vdd = 0x0B,  /*!< VDD * 7/16 selected as reference */
    LPCOMP_NSEL_REF_9_16Vdd = 0x0C,  /*!< VDD * 9/16 selected as reference */
    LPCOMP_NSEL_REF_11_16Vdd = 0x0D, /*!< VDD * 11/16 selected as reference */
    LPCOMP_NSEL_REF_13_16Vdd = 0x0E, /*!< VDD * 13/16 selected as reference */
    LPCOMP_NSEL_REF_15_16Vdd = 0x0F, /*!< VDD * 15/16 selected as reference */
    LPCOMP_NSEL_REF_AIN0 = 0x07,     /*!< External analog reference selected */
    LPCOMP_NSEL_REF_AIN1 = 0x17      /*!< External analog reference selected */
} blpcomp_nsel_ref_ids;

typedef void (*f_CrossCallback)(uint8_t);

class BLPCOMPClass
{
public:
    f_CrossCallback _crossCallback;
    bool startComparing(blpcomp_psel_pin_ids psel, blpcomp_nsel_ref_ids nsel, f_CrossCallback crossCallback);

private:
};

extern BLPCOMPClass BLPCOMP;

#endif