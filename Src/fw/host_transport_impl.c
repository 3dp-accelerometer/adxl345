/**
 * \file host_transport_impl.c
 *
 * Implements hardware specific transport to the IN USB endpoint of the host.
 */

#include "fw/host_transport_impl.h"
#include "usbd_cdc_if.h"
#include <controller.h>
#include <errno.h>
#include <host_transport.h>
#include <host_transport_types.h>

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
extern struct Controller_Handle controllerHandle;

enum HostTransport_Status HostTransportImpl_doTransmitImpl(uint8_t *buffer,
                                                           uint16_t len) {
  switch (CDC_Transmit_FS(buffer, len)) {
  case USBD_OK:
    return HostTransport_Status_Ok;
  case USBD_BUSY:
    return HostTransport_Status_Busy;
  case USBD_EMEM:
  case USBD_FAIL:
    return HostTransport_Status_Fail;
  default:
    return HostTransport_Status_Undefined;
  }
}

volatile bool HostTransportImpl_isTransmitBusyImpl() {
  return HostTransport_Status_Busy == HostTransportImpl_doTransmitImpl(NULL, 0);
}

int HostTransportImpl_onTakeReceivedImpl(const uint8_t *buffer) {
  if (NULL == buffer) {
    return -EINVAL;
  }

  const struct TransportFrame *request = (struct TransportFrame *)buffer;

  switch (request->header.id) {
  case Transport_HeaderId_Rx_GetFirmwareVersion:
    controllerHandle.host.onRequestGetFirmwareVersion();
    return 0;
  case Transport_HeaderId_Rx_GetOutputDataRate:
    controllerHandle.host.onRequestGetOutputDataRate();
    return 0;
  case Transport_HeaderId_Rx_SetOutputDataRate:
    controllerHandle.host.onRequestSetOutputDatatRate(
        request->asRxFrame.asSetOutputDataRate.rate);
    return 0;
  case Transport_HeaderId_Rx_GetRange:
    controllerHandle.host.onRequestGetRange();
    return 0;
  case Transport_HeaderId_Rx_SetRange:
    controllerHandle.host.onRequestSetRange(
        request->asRxFrame.asSetRange.range);
    return 0;
  case Transport_HeaderId_Rx_GetScale:
    controllerHandle.host.onRequestGetScale();
    return 0;
  case Transport_HeaderId_Rx_SetScale:
    return controllerHandle.host.onRequestSetScale(
        request->asRxFrame.asSetScale.scale);
  case Transport_HeaderId_Rx_GetDeviceSetup:
    controllerHandle.host.onRequestGetDeviceSetup();
    return 0;
  case Transport_HeaderId_Rx_DeviceReboot:
    controllerHandle.requestReboot();
    return 0;
  case Transport_HeaderId_Rx_SamplingStart:
    controllerHandle.host.onRequestSamplingStart(
        request->asRxFrame.asSamplingStart.max_samples_count);
    return 0;
  case Transport_HeaderId_Rx_SamplingStop:
    controllerHandle.host.onRequestSamplingStop();
    return 0;
  case Transport_HeaderId_Rx_GetUptime:
    controllerHandle.host.onRequestUptime();
    return 0;
  case Transport_HeaderId_Rx_GetBufferStatus:
    controllerHandle.host.onRequestBufferStatus();
    return 0;

  default:
    return -EINVAL;
  }
}
