/**
 * \file host_transport_types.h
 */

#pragma once
#include <inttypes.h>

/**
 * Status returned by HostTransport_Handle.transmit(uint8_t *, uint16_t);
 */
enum HostTransport_Status {
  HostTransport_Status_Ok = 0,
  HostTransport_Status_Busy,
  HostTransport_Status_Again,
  HostTransport_Status_BufferOverflow,
  HostTransport_Status_Fail,
  HostTransport_Status_Undefined
};

struct HostTransport_FromHostApi {
  int (*const doTakeReceivedPacketImpl)(
      uint8_t *); ///< Context: CDC_Receive_FS(uint8_t* , uint32_t *)
};

struct HostTransport_ToHostApi {
  uint8_t *transmissionBuffer; ///< Context: main() (and usb tx interrupt?)
  enum HostTransport_Status (*const doTransmitImpl)(
      uint8_t *, uint16_t); ///< Context: main() and interrupts

  const uint8_t controllerVersionMajor; ///< Context: main()
  const uint8_t controllerVersionMinor; ///< Context: main()
  const uint8_t controllerVersionPatch; ///< Context: main()

  int (*const doGetSensorOutputDataRateImpl)(uint8_t *); ///< Context: main()
  int (*const doGetSensorScaleImpl)(uint8_t *);          ///< Context: main()
  int (*const doGetSensorRangeImpl)(uint8_t *);          ///< Context: main()
  uint32_t (*const doGetUptimeMsImpl)(); ///< Context: CDC_Receive_FS(uint8_t* ,
                                         ///< uint32_t *)
};

/**
 * Host communication handle.
 */
struct HostTransport_Handle {
  struct HostTransport_FromHostApi fromHost;
  struct HostTransport_ToHostApi toHost;
};
