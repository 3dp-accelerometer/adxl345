/**
 * \file adxl345_transport_types.h
 *
 * ADXL345 data types transported to/from the sensor.
 */

#pragma once

#include "adxl345_register.h"
#include <assert.h>
#include <inttypes.h>

/**
 * 2-Bytes struct for convenient byte access.
 *
 * Provides convenient and readable access to underlying bytes.
 */
struct TwoBytes {
  uint8_t byte1;
  uint8_t byte2;
} __attribute__((packed));

/* TX Frame ------------------------------------------------------------------*/

/**
 * Struct for convenient data access of padded register without bit-shift magic.
 */
struct Adxl345Transport_DataPaddedRegister {
  uint8_t _padding8;                ///< reserved
  union Adxl345Register asRegister; ///< cast to Adxl345Register
} __attribute__((packed));

/**
 * Union for convenient data access of words, bytes and padded registers
 * without bit-shift magic.
 */
union Adxl345Transport_TxFrame {
  uint16_t asWord;         ///< cast to uint16_t
  struct TwoBytes asBytes; ///< cast to TwoBytes
  uint8_t asAddress;       ///< cast to uint8_t
  struct Adxl345Transport_DataPaddedRegister
      asPaddedRegister; ///< cast to Adxl345DataPaddedRegister
} __attribute__((packed));

// NOLINTNEXTLINE(readability-redundant-declaration,clang-diagnostic-implicit-int)
static_assert(sizeof(union Adxl345Transport_TxFrame) == 2U,
              "ERROR: Adxl345Transport_TxFrame must be 2B large.");

/* RX Frame ------------------------------------------------------------------*/

/**
 * Data acceleration struct with same byte layout as reports by ADXL345.
 */
struct Adxl345Transport_Acceleration {
  int16_t x; ///< x-axis acceleration as seen from the sensor's perspective
  int16_t y; ///< y-axis acceleration as seen from the sensor's perspective
  int16_t z; ///< z-axis acceleration as seen from the sensor's perspective
} __attribute__((packed));

/**
 * Union for convenient type-casting in between raw data, bytes, word, register
 * and payload types without c-style cast.
 */
union Adxl345Transport_RxFrame {
  union Adxl345Register asRegister; ///< cast to Adxl345Register
  struct Adxl345Transport_Acceleration
      asAcceleration;      ///< cast to Adxl345_Acceleration
  struct TwoBytes asBytes; ///< cast to TwoBytes
  uint16_t asWord;         ///< cast to uint16_t
} __attribute__((packed));
