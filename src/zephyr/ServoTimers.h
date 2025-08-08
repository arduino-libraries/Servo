#define MAX_ZEPHYR_SERVOS 16

#define TIMER DT_NODELABEL(counter_servo)

const uint32_t servo_timer_base_us_tick = 4; 
const uint32_t servo_timer_end_tick = 20000/servo_timer_base_us_tick;
