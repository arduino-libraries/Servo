/* The MIT License (MIT)
 *
 * Copyright (c) 2022 Arduino SA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#if defined(ARDUINO_ARCH_RENESAS)

#include "Arduino.h"
#include "Servo.h"
#include "ServoTimers.h"
#include "math.h"
#include "FspTimer.h"

#define SERVO_MAX_SERVOS            (_Nbr_16timers  * SERVOS_PER_TIMER)
#define SERVO_INVALID_INDEX         (255)
// Lower the timer ticks for finer resolution.
#define SERVO_TIMER_TICK_US         (100)
#define SERVO_US_PER_CYCLE          (20000)
#define SERVO_IO_PORT_ADDR(pn)      &((R_PORT0 + ((uint32_t) (R_PORT1 - R_PORT0) * (pn)))->PCNTR3)

// Internal Servo sturct to keep track of RA configuration.
typedef struct {
    // Servo period in microseconds.
    uint32_t period_us;
    // Store min/max pulse width here, because min/max in
    // Servo class are not wide enough for the pulse width.
    uint32_t period_min;
    uint32_t period_max;
    // Period period_count in microseconds.
    uint32_t period_count;
    // Internal FSP GPIO port/pin control bits.
    volatile uint32_t *io_port;
    uint32_t io_mask;
} ra_servo_t;

// Keep track of the total number of servos attached.
static size_t n_servos=0;
static ra_servo_t ra_servos[SERVO_MAX_SERVOS];

static FspTimer servo_timer;
static bool servo_timer_started = false;
void servo_timer_callback(timer_callback_args_t *args);

static int servo_timer_config(uint32_t period_us)
{
    static bool configured = false;
    if (configured == false) {
        // Configure and enable the servo timer.
        uint8_t type = 0;
        int8_t channel = FspTimer::get_available_timer(type);
        if (channel != -1) {
            servo_timer.begin(TIMER_MODE_PERIODIC, type, channel,
                    1000000.0f/period_us, 50.0f, servo_timer_callback, nullptr);
            servo_timer.setup_overflow_irq();
            servo_timer.open();
            servo_timer.stop();
            configured = true;
        }
    }
    return configured ? 0 : -1;
}

static int servo_timer_start()
{
    // Start the timer if it's not started
    if (servo_timer_started == false &&
            servo_timer.start() == false) {
        return -1;
    }
    servo_timer_started = true;
    return 0;
}

static int servo_timer_stop()
{
    // Start the timer if it's not started
    if (servo_timer_started == true &&
            servo_timer.stop() == false) {
        return -1;
    }
    servo_timer_started = false;
    return 0;
}

void servo_timer_callback(timer_callback_args_t *args)
{
    for (size_t i=0; i<SERVO_MAX_SERVOS; i++) {
        ra_servo_t *servo = &ra_servos[i];
        if (servo->period_us) {
            servo->period_count += SERVO_TIMER_TICK_US;
            if (servo->period_count <= servo->period_us) {
                *servo->io_port = (uint32_t) servo->io_mask;
            } else {
                *servo->io_port = (uint32_t) (servo->io_mask << 16);
            }
            if (servo->period_count == SERVO_US_PER_CYCLE) {
                servo->period_count = 0;
            }
        }
    }
}

Servo::Servo()
{
    servoIndex = SERVO_INVALID_INDEX;
}

uint8_t Servo::attach(int pin)
{
    return attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
    //assert(pin < NUM_DIGITAL_PINS); ?
    if (n_servos == SERVO_MAX_SERVOS) {
        return 0;
    }

    // Try to find a free servo slot.
    ra_servo_t *servo = NULL;
    bsp_io_port_pin_t io_pin = g_pin_cfg[pin].pin;
    for (size_t i=0; i<SERVO_MAX_SERVOS; i++) {
        servo = &ra_servos[i];
        if (servo->period_us == 0) {
            n_servos++;
            servoIndex = i;
            servo->period_min = min;
            servo->period_max = max;
            servo->io_mask = (1U << (io_pin & 0xFF));
            servo->io_port = SERVO_IO_PORT_ADDR(((io_pin >> 8U) & 0xFF));
            writeMicroseconds(DEFAULT_PULSE_WIDTH);
            break;
        }
    }

    if (servoIndex == SERVO_INVALID_INDEX) {
        return 0;
    }

    // Configure GPIO pin for the servo.
    R_IOPORT_PinCfg(&g_ioport_ctrl, io_pin,
            IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_HIGH);

    // Configure and start the timer if it's not started.
    if (servo_timer_config(SERVO_TIMER_TICK_US) != 0 ||
            servo_timer_start() != 0) {
        return 0;
    }
    return 1;
}

void Servo::detach()
{
    if (servoIndex != SERVO_INVALID_INDEX) {
        ra_servo_t *servo = &ra_servos[servoIndex];
        servo_timer_stop();
        servo->period_us = 0;
        if (--n_servos) {
            servo_timer_start();
        }
        servoIndex = SERVO_INVALID_INDEX;
    }
}

void Servo::write(int angle)
{
    if (servoIndex != SERVO_INVALID_INDEX) {
        ra_servo_t *servo = &ra_servos[servoIndex];
        angle = constrain(angle, 0, 180);
        writeMicroseconds(map(angle, 0, 180, servo->period_min, servo->period_max));
    }
}

int Servo::read()
{
    if (servoIndex != SERVO_INVALID_INDEX) {
        ra_servo_t *servo = &ra_servos[servoIndex];
        return map(servo->period_us, servo->period_min, servo->period_max, 0, 180);
    }
    return 0;
}

void Servo::writeMicroseconds(int us)
{
    if (servoIndex != SERVO_INVALID_INDEX) {
        ra_servo_t *servo = &ra_servos[servoIndex];
        servo->period_count = 0;
        servo->period_us = constrain(us, servo->period_min, servo->period_max);
    }
}

int Servo::readMicroseconds()
{
    if (servoIndex != SERVO_INVALID_INDEX) {
        ra_servo_t *servo = &ra_servos[servoIndex];
        return servo->period_us;
    }
    return 0;
}
#endif  // defined(ARDUINO_ARCH_RENESAS)
