#include "hcsr04.h"

/* ── Forward declarations of internal helpers ────────────────────────── */
extern TIM_HandleTypeDef HCSR04_TIMER;   /* defined by CubeMX in tim.c   */

static void     delay_us(uint32_t us);
static uint32_t get_timer_count(void);

/* ═══════════════════════════════════════════════════════════════════════
 *  Public functions
 * ═════════════════════════════════════════════════════════════════════ */

void HCSR04_Init(void)
{
    /* Start the free-running µs counter.  The timer must already be
     * configured in CubeMX (1 MHz, free-running, no interrupts needed). */
    HAL_TIM_Base_Start(&HCSR04_TIMER);
}

HCSR04_Status HCSR04_Read(float *distance_cm)
{
    uint32_t t_start, t_end, elapsed_us;

    /* ── 1. Send 10 µs trigger pulse ──────────────────────────────── */
    HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_RESET);
    delay_us(2);                              /* ensure line is LOW first */

    HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_SET);
    delay_us(10);                             /* ≥10 µs HIGH pulse        */
    HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_RESET);

    /* ── 2. Wait for ECHO to go HIGH (start of pulse) ──────────────── */
    t_start = get_timer_count();
    while (HAL_GPIO_ReadPin(HCSR04_ECHO_PORT, HCSR04_ECHO_PIN) == GPIO_PIN_RESET)
    {
        if ((get_timer_count() - t_start) >= HCSR04_TIMEOUT_US)
        {
            return HCSR04_TIMEOUT;
        }
    }

    /* ── 3. Measure pulse width while ECHO is HIGH ──────────────────── */
    t_start = get_timer_count();
    while (HAL_GPIO_ReadPin(HCSR04_ECHO_PORT, HCSR04_ECHO_PIN) == GPIO_PIN_SET)
    {
        if ((get_timer_count() - t_start) >= HCSR04_TIMEOUT_US)
        {
            return HCSR04_TIMEOUT;            /* object too far / no echo */
        }
    }
    t_end = get_timer_count();

    /* ── 4. Calculate distance ─────────────────────────────────────── */
    elapsed_us   = t_end - t_start;          /* round-trip time in µs    */
    *distance_cm = (elapsed_us * SOUND_SPEED_CM_PER_US) / 2.0f;

    return HCSR04_OK;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  Internal helpers
 * ═════════════════════════════════════════════════════════════════════ */

/**
 * @brief  Busy-wait delay using the hardware µs counter.
 *         Handles 16-bit timer wrap-around correctly.
 */
static void delay_us(uint32_t us)
{
    uint32_t start = get_timer_count();//__HAL_TIM_GET_COUNTER(&HCSR04_TIMER);
    while ((get_timer_count() - start) < us);//((__HAL_TIM_GET_COUNTER(&HCSR04_TIMER) - start) < us);
}

/**
 * @brief  Read the current timer counter value.
 */
static uint32_t get_timer_count(void)
{
    return __HAL_TIM_GET_COUNTER(&HCSR04_TIMER);
}
