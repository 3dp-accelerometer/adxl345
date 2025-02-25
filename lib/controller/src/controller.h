/**
 * \file controller.h
 *
 * The controller module aims at abstracting the whole device API.
 *
 * The device API consists of the following APIs:
 *   - host transport API
 *   - sensor API
 *   - sampling API
 */

#pragma once
#include <inttypes.h>
#include <stdbool.h>

#include <adxl345.h>
#include <host_transport.h>
#include <sampling_types.h>

enum TransportRx_SetOutputDataRate_Rate;
enum TransportRx_SetScale_Scale;
enum TransportRx_SetRange_Range;

struct Controller_Sensor {
  struct Adxl345_Handle handle;

  void (*const init)(); ///< Context: main()
};

struct Controller_Sampling {
  struct Sampling_Handle handle; ///< device specific pimpl

  /**
   * Device API for sampling module.
   *
   * @{
   */
  void (*const doSetFifoWatermark)();   ///< Context: EXTI2_IRQHandler()
  void (*const doClearFifoWatermark)(); ///< Context: EXTI2_IRQHandler()
  void (*const doSetFifoOverflow)();    ///< Context: EXTI3_IRQHandler()
  void (*const doSet5usTimerExpired)(); ///< Context: TIM3_IRQHandler()
  /// @}
};

struct Controller_Host {
  struct HostTransport_Handle handle; ///< device specific pimpl

  /**
   * Device API for host-transport module.
   *
   * Context: CDC_Receive_FS(uint8_t*, uint32_t *)
   */
  void (*const doTakeBytes)(const uint8_t *, uint16_t);

  /**
   * Device API for Host-Transport callbacks upon doTakeBytes(uint8_t *,
   * uint16_t).
   *
   * Context: CDC_Receive_FS(uint8_t*, uint32_t *)
   * @{
   */
  void (*const onRequestGetFirmwareVersion)();
  void (*const onRequestGetOutputDataRate)();
  int (*const onRequestSetOutputDatatRate)(
      enum TransportRx_SetOutputDataRate_Rate);
  void (*const onRequestGetRange)();
  int (*const onRequestSetRange)(enum TransportRx_SetRange_Range);
  void (*const onRequestGetScale)();
  int (*const onRequestSetScale)(enum TransportRx_SetScale_Scale);
  void (*const onRequestGetDeviceSetup)();
  void (*const onRequestSamplingStart)(uint16_t);
  void (*const onRequestSamplingStop)();
  void (*const onRequestUptime)();
  void (*const onRequestBufferStatus)();
  /// @}
};

/**
 * Handle for several device pointer implementations.
 *
 * This handle will be shared among main() context and several interrupts'
 * context.Hence, all members shall be seen as const and never be modified at
 * runtime.Underlying fields may be changed though. These shall be marked as
 * volatile and properly synchronized.
 *
 */
struct Controller_Handle {
  /**
   * Device API for sensor and sensor pimpl.
   */
  struct Controller_Sensor sensor;

  /**
   * Device API for sampling and sampling pimpl.
   */
  struct Controller_Sampling sampling;

  /**
   * Device API for host transport and host transport pimpl.
   */
  struct Controller_Host host;

  /**
   * Public device API.
   * @{
   */
  void (*const init)(); ///< Context: main()
  void (*const loop)(); ///< Context: main()

  void (*const requestReboot)(); ///< Context: main()

  void (*const fault_onNmiFaultHandler)();   ///< \see stm32f4xx_it.h
  void (*const fault_onUsageFaultHandler)(); ///< \see stm32f4xx_it.h
  void (*const fault_onBusFaultHandler)();   ///< \see stm32f4xx_it.h
  void (*const fault_onHardFaultHandler)();  ///< \see stm32f4xx_it.h
  void (*const fault_onErrorHandler)();      ///< \see main.h

  /// @}
};
