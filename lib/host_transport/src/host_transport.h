/**
 * \file host_transport.h
 */

#pragma once

#include <inttypes.h>
#include <ringbuffer.h>

// NOLINTNEXTLINE(modernize-macro-to-enum)
#define TRANSPORTTX_TRANSMIT_TX_DATA_CHUNK_BUFFER_BYTES 2048U

/**
 * Status returned by HostTransport_Handle.transmit(uint8_t *, uint16_t);
 */
enum HostTransport_Status {
  HostTransport_Status_Ok = 0,
  HostTransport_Status_Busy,
  HostTransport_Status_Fail,
  HostTransport_Status_Undefined
};

struct HostTransport_FromHostApi {
  int (*const doTakeReceivedPacketImpl)(
      const uint8_t *); ///< Context: CDC_Receive_FS(uint8_t* , uint32_t *)
};

struct HostTransport_ToHostApi {
  /**
   * Circular buffer for buffering outgoing acceleration packets.
   *
   * This buffer is used to pile up acceleration chunks when USB is busy and
   * transmission has to be postponed.
   * The buffer must only be utilized by TransportTx_TxAccelerationBuffer(
   * struct HostTransport_Handle *, const struct Transport_Acceleration *,
   * uint8_t, uint16_t).
   *
   * \see HostTransport_ToHostApi.txBuffer
   *
   * Context: main()
   */
  struct Ringbuffer ringbuffer;

  /**
   * Keeps track of the largest chunk size transmitted at once since sampling
   * stream started.
   *
   * Maximum possible chunk size is 2kB.
   */
  uint16_t largestTxChunkBytes;

  /**
   * Copies over buffer and goes into transmit mode.
   *
   * Context: main() and interrupts
   *
   * @return HostTransport_Status_Busy until transmission is finished
   */
  enum HostTransport_Status (*const doTransmitImpl)(uint8_t *, uint16_t);

  /**
   * Context: main() and interrupts
   *
   * @return true until transmission is finished.
   */
  volatile bool (*const isTransmitBusyImpl)();
};

/**
 * Host communication handle.
 */
struct HostTransport_Handle {
  struct HostTransport_FromHostApi fromHost;
  struct HostTransport_ToHostApi toHost;
};

/**
 * Resets internal ringbuffer state.
 *
 * Usually called when new sampling stream is started upon user request.
 *
 * @param handle
 */
void Transport_resetBuffer(struct HostTransport_Handle *handle);
