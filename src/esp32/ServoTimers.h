#define MAX_PWM_SERVOS             16

#define LEDC_MAX_BIT_WIDTH      SOC_LEDC_TIMER_BIT_WIDE_NUM

constexpr uint32_t BIT_RESOLUTION = (1 << LEDC_MAX_BIT_WIDTH) - 1;

#define LEDC_US_TO_TICKS(us)    static_cast<uint32_t>((us * BIT_RESOLUTION) / REFRESH_INTERVAL)
#define LEDC_TICKS_TO_US(ticks) static_cast<uint32_t>((ticks * REFRESH_INTERVAL) / BIT_RESOLUTION)