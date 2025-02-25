/**
 * \file to_host_transport.h
 *
 * API for transporting data from controller to host.
 */

#pragma once
#include <inttypes.h>

// NOLINTNEXTLINE(modernize-macro-to-enum)
#define TRANSPORTTX_TRANSMIT_ACCELERATION_BUFFER_BYTES 24U

struct HostTransport_Handle;
struct Transport_Acceleration;

enum HostTransport_Status;
enum TransportTx_FaultCode;

/**
 * Transmits sensor configuration TransportTx_DeviceSetup to the IN endpoint of
 * host.
 * Transmission will block this function from returning until completion.
 *
 * @param handle host transport pimpl
 * @param sensorOdr sensor output data rate
 * @param sensorRange sensor scale
 * @param sensorScale sensor range
 */
void TransportTx_TxSamplingSetup(struct HostTransport_Handle *handle,
                                 uint8_t sensorOdr, uint8_t sensorScale,
                                 uint8_t sensorRange);

/**
 * Transmits sensor scale TransportTx_Scale to the IN endpoint of host.
 *
 * Transmission will block this function from returning until completion.
 *
 * @param handle
 * @param sensorScale
 */
void TransportTx_TxScale(struct HostTransport_Handle *handle,
                         uint8_t sensorScale);

/**
 * Transmits sensor range TransportTx_Range to the IN endpoint of host.
 *
 * Transmission will block this function from returning until completion.
 *
 * @param handle
 * @param sensorRange
 */
void TransportTx_TxRange(struct HostTransport_Handle *handle,
                         uint8_t sensorRange);

/**
 * Transmits sensor output data rate TransportTx_OutputDataRate to the IN
 * endpoint of host.
 *
 * Transmission will block this function from returning until completion.
 * @param handle
 * @param sensorOdr
 */
void TransportTx_TxOutputDataRate(struct HostTransport_Handle *handle,
                                  uint8_t sensorOdr);

/**
 * Transmits firmware version TransportTx_FirmwareVersion to the IN endpoint of
 * host.
 *
 * Transmission will block this function from returning until completion.
 *
 * @param handle host transport pimpl
 * @param major firmware version
 * @param minor firmware version
 * @param patch firmware version
 */
void TransportTx_TxFirmwareVersion(struct HostTransport_Handle *handle,
                                   uint8_t major, uint8_t minor, uint8_t patch);

/**
 * Transmits sampling started package TransportTx_SamplingStarted to the IN
 * endpoint of host.
 *
 * Transmission will block this function from returning until completion.
 *
 * @param handle host transport pimpl
 */
void TransportTx_TxSamplingStarted(struct HostTransport_Handle *handle,
                                   uint16_t max_samples);

/**
 * Transmits sampling finished package TransportTx_SamplingFinished to the IN
 * endpoint of host.
 *
 * Transmission will block this function from returning until completion.
 *
 * @param handle host transport pimpl
 */
void TransportTx_TxSamplingFinished(struct HostTransport_Handle *handle);

/**
 * Transmits sampling stopped package TransportTx_SamplingStopped to the IN
 * endpoint of host.
 *
 * Transmission will block this function from returning until completion.
 *
 * @param handle host transport pimpl
 * @param sensorOdr
 * @param sensorScale
 * @param sensorRange
 */
void TransportTx_TxSamplingStopped(struct HostTransport_Handle *handle);

/**
 * Transmits sampling aborted package TransportTx_SamplingAborted to the IN
 * endpoint of host.
 *
 * Transmission will block this function from returning until completion.
 *
 * @param handle host transport pimpl
 */
void TransportTx_TxSamplingAborted(struct HostTransport_Handle *handle);

/**
 * Transmits FiFo overflow package TransportTx_FifoOverflow to the IN endpoint
 * of host.
 *
 * Transmission will block this function from returning until completion.
 *
 * @param handle host transport pimpl
 */
void TransportTx_TxFifoOverflow(struct HostTransport_Handle *handle);

void TransportTx_TxBufferOverflow(struct HostTransport_Handle *handle);

void TransportTx_TxTransmissionError(struct HostTransport_Handle *handle);

/**
 * Transmits device uptime TransportTx_Uptime to the IN endpoint of host.
 *
 * Transmission will block this function from returning until completion.
 * @param handle
 * @param uptimeMs
 */
void TransportTx_TxUptime(struct HostTransport_Handle *handle,
                          uint32_t uptimeMs);

/**
 * Transmits device error state TransportTx_TxFault to the IN endpoint of host.
 *
 * Transmission will block this function from returning until completion.
 * @param handle
 * @param code
 */
void TransportTx_TxFault(struct HostTransport_Handle *handle,
                         enum TransportTx_FaultCode code);

/**
 * Transmits device buffer status TransportTx_BufferStatus to the IN endpoint of
 * host.
 *
 * Transmission will block this function from returning until completion.
 *
 * @param handle
 * @param sizeBytes total buffer size in bytes
 * @param capacityTotal maximum capacity (items/slots)
 * @param capacityUsedMax greatest items utilization since sampling start
 * @param largestTxChunkBytes largest chunk sent at once since sampling start
 */
void TransportTx_TxBufferStatus(struct HostTransport_Handle *handle,
                                uint16_t sizeBytes, uint16_t capacityTotal,
                                uint16_t capacityUsedMax, uint16_t putCount,
                                uint16_t takeCount,
                                uint16_t largestTxChunkBytes);

/**
 * Forwards acceleration data block to the IN endpoint of host.
 *
 * Triggers sending data to the the IN endpoint.
 * If USB is busy the data is buffered for a later transmission.
 * To consume all buffered data this function shall be called until -ENODATA is
 * returned (with data and/or count being NULL and/or 0).
 *
 * @param handle host transport pimpl
 * @param data tx buffer or NULL to consume remaining buffered data
 * @param count buffer size or 0 to consume remaining buffered data
 * @param firstIndex the tracked index number of the first acceleration in data
 * buffer
 * @return
 *   - 0 on success (data send in first run),
 *   - EBUSY if data was buffered,
 *   - ENODATA if no buffered data available (all data sent),
 *   - -EINVAL otherwise
 */
int TransportTx_TxAccelerationBuffer(struct HostTransport_Handle *handle,
                                     const struct Transport_Acceleration *data,
                                     uint8_t count, uint16_t firstIndex);
