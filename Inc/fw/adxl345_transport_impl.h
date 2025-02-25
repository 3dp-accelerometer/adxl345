/**
 * \file adxl345_transport_impl.h
 *
 * Hardware specific ADXL345 implementation API.
 */

#pragma once

#include <adxl345_spi_types.h>
#include <adxl345_transport_types.h>
#include <inttypes.h>

struct Adxl345_Handle;

#define ADXL345_HANDLE_INITIALIZER                                             \
  {                                                                            \
    .doTransmitFrameImpl = Adxl345TransportImpl_doTransmitFrameImpl,           \
    .doTransmitReceiveFrameImpl =                                              \
        Adxl345TransportImpl_doTransmitReceiveFrameImpl                        \
  }

/**
 * Sends one single frame to ADXL345 via SPI interface.
 *
 * @param frame the payload to send via SPI
 * @param numBytes size of frame
 * @param applyCs whether or not to set nCS before and clear nCS after
 * transmission
 * @param rwFlag flag to indicate whether this transaction is write or read
 *   \see Adxl345TransportImpl_transmitReceiveFrame(union Adxl345TP_TxFrame *,
 * union Adxl345TP_RxFrame *, uint8_t)
 *
 * @return -EINVAL if invalid args, -EIO on TX error, 0 otherwise
 */
int Adxl345TransportImpl_doTransmitFrameImpl(
    const union Adxl345Transport_TxFrame *frame, uint8_t numBytes,
    enum Adxl345Spi_Cs applyCs, enum Adxl345Spi_RwFlags rwFlag);

/**
 * Transmits and receives frames (read transaction).
 *
 * Note: This automatically sets nCS before TX and clears nCS after completed
 * RX.
 *
 * @param txFrame data to send
 * @param rxFrame receive data
 * @param numBytesReceive expected size of received data
 *
 * @return -EINVAL if invalid args, -EIO on TX/RX error, 0 otherwise
 */
int Adxl345TransportImpl_doTransmitReceiveFrameImpl(
    const union Adxl345Transport_TxFrame *txFrame,
    union Adxl345Transport_RxFrame *rxFrame, uint8_t numBytesReceive);
