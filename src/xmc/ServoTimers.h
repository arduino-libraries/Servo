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
 * @copyright Copyright (c) 2019-2020 Infineon Technologies AG
 */
#ifndef _SERVO_H_
#define _SERVO_H_

#include <Arduino.h>
#include "wiring_analog.h"

/*
 * Note on Arduino compatibility:
 *
 * In the Arduino implementation, PWM is done "by hand" in the sense
 * that timer channels are hijacked in groups and an ISR is set which
 * toggles Servo::attach()ed pins using digitalWrite().
 *
 * While this scheme allows any pin to drive a servo, it chews up
 * cycles and complicates the programmer's notion of when a particular
 * timer channel will be in use.
 *
 * This implementation only allows Servo instances to attach() to pins
 * that already have PWM unit associated with them, which drives the wave.
 *
 * While the Arduino implementation of attach() returns the affected channel,
 * this one returns the index number of the servo or an INVALID_SERVO = 255 in
 * case of an error.
 * The attach will check if a pin is already in use and if a pin has a PWM unit on
 * the selected XMC board, otherwise it returns an INVALID_SERVO.
 * This error handling is different than the original one from Arduino.
 *
 * Depending on the XMC type the number of possible PWM channels vary from 4 to 23
 * and may change with future version of the XMC series.
 */

// Define the MAX_PWM_SERVOS number per XMC type and the allowed PWM pins on the selected XMC board
#if defined(XMC1100_XMC2GO)
#define MAX_PWM_SERVOS             4
#define ALLOWED_PINS               {1, 2, 3, 8,}
#elif defined(XMC1100_Boot_Kit)
#define MAX_PWM_SERVOS             6
#define ALLOWED_PINS               { 3,4,6,9,10,11 }
#elif defined(XMC1300_Boot_Kit)
#define MAX_PWM_SERVOS             4
#define ALLOWED_PINS               { 26,31,32,33 }
#elif defined(XMC1400_Arduino_Kit)
#define MAX_PWM_SERVOS             6
#define ALLOWED_PINS               { 3,4,6,9,10,11 }
#elif defined(XMC4200_Platform2GO)
#define MAX_PWM_SERVOS             7
#define ALLOWED_PINS               { 3,5,6,9,22,23,24 }
#elif defined(XMC4400_Platform2GO)
#define MAX_PWM_SERVOS             15
#define ALLOWED_PINS               { 3,5,6,9,10,14,25,26,27,28,29,30,45,48,67 }
#elif defined(XMC4700_Relax_Kit)
#define MAX_PWM_SERVOS             23
#define ALLOWED_PINS               { 3,5,6,9,10,11,34,36,37,51,61,62,66,70,76,77,79,80,81,88,89,93,94 }
#else
#error "Not a supported XMC Board"
#endif

#define MIN_ANGLE               0                               // the minimal angle in degree
#define MAX_ANGLE               180                             // the maximal angle in degree
#define MIN_PULSE_WIDTH         544                             // the shortest pulse sent to a servo in microseconds
#define MAX_PULSE_WIDTH         2400                            // the longest pulse sent to a servo in microseconds

#define MIN_PULSE_CHECK         500                             // border with below = angle and above = pulse width
#define REFRESH_FREQUENCY       50u                             // the refresh frequency on analog pins
#define REFRESH_TIME            20.0                            // the PWM refresh frequency for the servo motor
#define DUTYCYCLE_STEPS         65536.0 / REFRESH_TIME          // the number of duty cycle steps during one refresh period
#define ADC_RESOLUTION          16                              // the resolution of the adc during analog write

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
     * PWM channels on the XMC board is reached it will return
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
     *        for XMC PWM settings.
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
     *        XMC PWM settings. It also calculates the angle from the pwm value.
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

#endif /* _SERVO_H_ */
