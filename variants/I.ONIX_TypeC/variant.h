/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_NRF52_DK_
#define _VARIANT_NRF52_DK_

/** Master clock frequency */
#define VARIANT_MCK (64000000ul)

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Number of pins defined in PinDescription array
#define PINS_COUNT         (32u)
#define NUM_DIGITAL_PINS   (32u)
#define NUM_ANALOG_INPUTS  (8u)
#define NUM_ANALOG_OUTPUTS (0u)

// LEDs
#define PIN_LED_EN    (10u)
#define PIN_LED_RED   (9u)
#define PIN_LED_GREEN (3u)
#define PIN_LED_BLUE  (4u)
#define LED_BUILTIN   PIN_LED_RED

/*
 * Analog pins
 */
#define PIN_A1 (3)
#define PIN_A2 (4)
#define PIN_A4 (28)
#define PIN_A5 (29)
#define PIN_A6 (30)

static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
#define ADC_RESOLUTION 10

#define PIN_LPCOMP PIN_A5
/*
 * Serial interfaces
 */
// Serial
#define PIN_SERIAL_RX (28)
#define PIN_SERIAL_TX (27)

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_CTRL (11u)
#define PIN_WIRE_SDA  (12u)
#define PIN_WIRE_SCL  (17u)

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#endif
