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

#define PINS_COUNT         (32u)
#define NUM_DIGITAL_PINS   (32u)
#define NUM_ANALOG_INPUTS  (8u)
#define NUM_ANALOG_OUTPUTS (0u)

// Serial
#define PIN_SERIAL_RX (24)
#define PIN_SERIAL_TX (22)

// Wire
#define WIRE_INTERFACES_COUNT 1
#define PIN_WIRE_SDA          (6u)
#define PIN_WIRE_SCL          (5u)

// SPI
#define SPI_INTERFACES_COUNT 1
#define PIN_SPI_MISO         (12)
#define PIN_SPI_MOSI         (14)
#define PIN_SPI_SCK          (15)

// USER NAMES
#define LP_BUZZ_PIN        (11u)
#define LPCOMP_PIN         (28u)
#define USR_BTN_PIN        (25u)
#define BLUE_LED_PIN       (23u)
#define MPR_VDD_CTRL_PIN   (4u)
#define MPR_WIRE_SDA_PIN   PIN_WIRE_SDA
#define MPR_WIRE_SCL_PIN   PIN_WIRE_SCL
#define E_INK_BUSY_PIN     (19u)
#define E_INK_RST_PIN      (18u)
#define E_INK_DC_PIN       (17u)
#define E_INK_CS_PIN       (16u)
#define E_INK_SCL_PIN      PIN_SPI_SCK
#define E_INK_SDA_PIN      PIN_SPI_MOSI
#define E_INK_VDD_CTRL_PIN (13u)

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#endif
