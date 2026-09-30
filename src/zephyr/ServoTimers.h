#define MAX_ZEPHYR_SERVOS 16

#define TIMER DT_NODELABEL(counter_servo)

#define SERVO_TIMER_BASE_US_TICK_POW 5
const uint32_t servo_timer_base_us_tick = 1 << SERVO_TIMER_BASE_US_TICK_POW;

const uint32_t servo_timer_end_tick = 20000/servo_timer_base_us_tick;
