/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010, LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/*
 * @copyright Copyright (c) 2025-2026 Infineon Technologies AG
 */

#ifndef __SERVO_TIMERS_H__
#define __SERVO_TIMERS_H__

#include <Arduino.h>

#define MAX_PWM_SERVOS 28
#define ALLOWED_PINS {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30}


#define MIN_ANGLE               0                               // the minimal angle in degree
#define MAX_ANGLE               180                             // the maximal angle in degree
#define MIN_PULSE_WIDTH         544                             // the shortest pulse sent to a servo in microseconds
#define MAX_PULSE_WIDTH         2400                            // the longest pulse sent to a servo in microseconds

#define MIN_PULSE_CHECK         500                             // border with below = angle and above = pulse width
#define REFRESH_FREQUENCY       50u                             // the refresh frequency on analog pins
#define REFRESH_TIME            20.0                            // the PWM refresh frequency for the servo motor
#define DUTYCYCLE_STEPS         65536.0 / REFRESH_TIME          // the number of duty cycle steps during one refresh period
#define RESOLUTION              16                              // the resolution of the adc during analog write

#define INVALID_SERVO           255                             // flag indicating an invalid servo index

/** Class for interfacing with RC servomotors. */
class Servo
{
public:
    /**
     * @brief Construct a new Servo instance.
     *
     * The new instance will not be attached to any pin, but only PWM capable pins will run.
     * see pin list above.
     */
    Servo();

    /**
     * @brief Associate this instance with a servomotor whose input is
     *        connected to pin.
     *
     * If this instance is already attached to a pin, it will be
     * detached before being attached to the new pin.
     * If the pin is not allowed for running PWM or the max number of
     * PWM channels on the PSOC6 board is reached it will return
     * with an INVALID_SERVO, otherwise with the servoIndex number.
     *
     * @param pin Pin connected to the servo pulse wave input. This
     *            pin must be capable of PWM output.
     *
     * @param min If this value is below MIN_PULSE_CHECK it will be associated
     *            with an angle in degree. Otherwise it will be the minimum
     *            pulse width.
     *            min as an angle must be between MIN_ANGLE < angle < MAX_ANGLE
     *            with default as MIN_ANGLE
     *            min as a pulse width must be between MIN_PULSE_WIDTH < pwm < MAX_PULSE_WIDTH
     *            with a default as MIN_PULSE_WIDTH
     *
     * @param max If this value is below MIN_PULSE_CHECK it will be associated
     *            with an angle in degree. Otherwise it will be the maximum
     *            pulse width.
     *            max as an angle must be between MIN_ANGLE < angle < MAX_ANGLE
     *            with default as MAX_ANGLE
     *            max as a pulse width must be between MIN_PULSE_WIDTH < pwm < MAX_PULSE_WIDTH
     *            with a default as MAX_PULSE_WIDTH
     *
     * @return servoIndex number or INVALID_SERVO = 255 in case of an error
     */
    uint8_t attach(uint8_t pin, uint16_t min = MIN_ANGLE, uint16_t max = MAX_ANGLE);


    /**
     * @brief Stop driving the servo pulse train.
     *
     * If not currently attached to a motor, this function has no effect.
     *
     * @return true if this call did anything, false otherwise.
     */
    void detach();

    /**
     * @brief Set the servomotor target angle by recalculating the duty cycle
     *        for PSOC6 PWM settings.
     *
     * @param value Target angle, in degrees. If the target angle is
     *              outside the range specified at attach(), it
     *              will be clamped to lie in that range.
     *
     * @see Servo::attach()
     */
    void write(int value);

    /**
     * @brief Set the pulse width, in microseconds by recalculating it for the
     *        PSOC6 PWM settings. It also calculates the angle from the pwm value.
     *
     * @param value Pulse width to send to the servomotor, in
     *              microseconds. If outside of the range
     *              specified at attach() time, it is clamped to
     *              lie in that range.
     *
     * @see Servo::attach()
     */
    void writeMicroseconds(int value);

    /**
     * returns the current value in degree as an angle between 0 and 189 degrees
     *
     * @see Servo::attach()
     */
    int read() const { return uint16_t(this->_deg); }

    /**
     * returns the current pwm value in microseconds.
     *
     * @see Servo::attach()
     */
    int readMicroseconds() const { return uint16_t(this->_pwm); }

    /**
     * @brief Check if this instance is attached to a servo.
     * @return true if this instance is attached to a servo, false otherwise.
     * @see Servo::attachedPin()
     */
    bool attached() const { return this->_isActive; }

private:
    uint16_t _minPW;                        // the initial minPulseWidth, if not set than MIN_PULSE_WIDTH
    uint16_t _maxPW;                        // the initial maxPulseWidth, if not set than MAX_PULSE_WIDTH
    int16_t  _minAngle;                     // the initial minAngle, if not set than MIN_ANGLE
    int16_t  _maxAngle;                     // the initial maxAngle, if not set than MAX_ANGLE
    int16_t  _pin;                          // attached arduino pin number
    double   _deg;                          // actual angle in degree
    double   _pwm;                          // actual pwm signal in microseconds
    uint8_t  _isActive;                     // true if this pin is active, otherwise false

    uint8_t  servoIndex;                    // the actual number of Servos attached to this library


};

#endif