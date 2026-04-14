#ifndef HCSR04_H
#define HCSR04_H

#include "stm32f4xx_hal.h"  // Change to match your STM32 series (f1xx, f3xx, etc.)
#include <stdint.h>

/* ── Configuration ─────────────────────────────────────────────────────────
 * Adjust these defines to match your wiring and CubeMX timer setup.
 *
 * TIMER  : Any general-purpose timer configured as a free-running counter
 *          at 1 MHz (i.e. CNT increments every 1 µs).
 *          In CubeMX: set Prescaler = (APB_timer_clock / 1_000_000) - 1
 *                     Counter Period = 0xFFFF (or 0xFFFFFFFF for 32-bit timers)
 *
 * TRIG   : Any GPIO output pin (push-pull, no pull)
 * ECHO   : Any GPIO input pin (no pull — use external 1 kΩ + 2 kΩ divider
 *          if your STM32 is 3.3 V, because the HC-SR04 outputs 5 V)
 * ─────────────────────────────────────────────────────────────────────── */
#define HCSR04_TIMER          htim2        /* HAL timer handle variable name */
#define HCSR04_TIMER_INSTANCE TIM2        /* Matching TIMx peripheral        */

#define HCSR04_TRIG_PORT      GPIOA
#define HCSR04_TRIG_PIN       GPIO_PIN_0

#define HCSR04_ECHO_PORT      GPIOA
#define HCSR04_ECHO_PIN       GPIO_PIN_1

/* Maximum time (µs) to wait for ECHO to go HIGH or LOW before timeout.
 * HC-SR04 max range ~4 m → round-trip ~23 ms.  38 000 µs = safe ceiling. */
#define HCSR04_TIMEOUT_US     42000U

/* Speed of sound (cm/µs).  Divide round-trip time by 2 for one-way. */
#define SOUND_SPEED_CM_PER_US 0.0343f

/* ── Return codes ─────────────────────────────────────────────────────── */
typedef enum {
    HCSR04_OK      =  0,
    HCSR04_TIMEOUT = -1,   /* Echo never arrived or never fell             */
    HCSR04_ERROR   = -2    /* Generic / out-of-range                       */
} HCSR04_Status;

/* ── Public API ──────────────────────────────────────────────────────── */

/**
 * @brief  Initialise the driver (starts the µs timer).
 *         Call once after HAL_Init() and MX_TIMx_Init().
 */
void     HCSR04_Init(void);

/**
 * @brief  Trigger one measurement.
 * @param  distance_cm  Output: distance in centimetres (float).
 * @return HCSR04_OK on success, HCSR04_TIMEOUT if sensor did not respond.
 */
HCSR04_Status HCSR04_Read(float *distance_cm);

#endif /* HCSR04_H */
