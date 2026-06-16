#include <soc/soc_caps.h>

#if defined(SOC_LEDC_CHANNEL_NUM) && defined(SOC_LEDC_SUPPORT_HS_MODE)
#define MAX_PWM_SERVOS          (SOC_LEDC_CHANNEL_NUM * 2)
#elif defined(SOC_LEDC_CHANNEL_NUM)
#define MAX_PWM_SERVOS          SOC_LEDC_CHANNEL_NUM
#else
#define MAX_PWM_SERVOS             16
#endif

#if defined(SOC_LEDC_TIMER_BIT_WIDTH)
#define LEDC_MAX_BIT_WIDTH      SOC_LEDC_TIMER_BIT_WIDTH
#elif defined(SOC_LEDC_TIMER_BIT_WIDE_NUM)
#define LEDC_MAX_BIT_WIDTH      SOC_LEDC_TIMER_BIT_WIDE_NUM
#else
#error "Unsupported ESP32 LEDC timer bit width"
#endif

constexpr uint32_t BIT_RESOLUTION = (1 << LEDC_MAX_BIT_WIDTH) - 1;

#define LEDC_US_TO_TICKS(us)    static_cast<uint32_t>((us * BIT_RESOLUTION) / REFRESH_INTERVAL)
#define LEDC_TICKS_TO_US(ticks) static_cast<uint32_t>((ticks * REFRESH_INTERVAL) / BIT_RESOLUTION)
