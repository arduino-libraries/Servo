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

#if defined(ARDUINO_ARCH_PSOC6)

#include "ServoTimers.h"

uint8_t _ServoCount = 1;                                   // internal counter to check if max numbers of servos is reached
static uint8_t _allowed[MAX_PWM_SERVOS] = ALLOWED_PINS;    // internal array to check allowed pwm pins
static uint8_t _servos[MAX_PWM_SERVOS];                    // static array of used servo pins for checking


/**
 * @brief None blocking wait loop.
 *
 * @param uS microseconds to wait
 */
static void _delayUs(unsigned long uS)
{
    unsigned long time_now = micros();
    while (micros() < time_now + uS)
        ;
}

Servo::Servo()
{
    if(_ServoCount <= MAX_PWM_SERVOS)
    {
        this->servoIndex = _ServoCount++;

        // Initialize the servo with default values
        this->_minAngle = MIN_ANGLE;
        this->_maxAngle = MAX_ANGLE;
        this->_minPW = MIN_PULSE_WIDTH;
        this->_maxPW = MAX_PULSE_WIDTH;
        this->_pin = 0;
        this->_isActive = false;
        this->_pwm = 0;
        this->_deg = 0.0;

        for(int i = 0; i < MAX_PWM_SERVOS; i++)
        {
            _servos[i] = 0xFF; // Initialize all servo pins to unused state
        }
    }
    else
    {
        this->servoIndex = INVALID_SERVO; // No more servos can be attached
    }
}

uint8_t Servo::attach(uint8_t pin, uint16_t min, uint16_t max)
{
   if(this->servoIndex <= MAX_PWM_SERVOS)
    {
        // Validate the pin
        bool pin_allowed = false;
        for(int i = 0; i < MAX_PWM_SERVOS; i++)
        {
            // Check if pin already in use
            if(_servos[i] == pin)
            {
                return INVALID_SERVO;
            }
            // Check if selected pin has a PWM unit on the used PSOC6 board
            if(_allowed[i] == pin)
                pin_allowed = true;
        }

        // Return if pin is not found in allowed pin list
        if(!pin_allowed)
            return INVALID_SERVO;

        // Set min/max values according to input and check for absolute limits
        if(min < MIN_PULSE_CHECK)
        {
            this->_minAngle = constrain(min, MIN_ANGLE, MAX_ANGLE);
            this->_minPW = MIN_PULSE_WIDTH;
        }
        else
        {
            this->_minAngle = MIN_ANGLE;  // TODO: has to be calculated
            this->_minPW = constrain(min, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
        }

        if(max < MIN_PULSE_CHECK)
        {
            this->_maxAngle = constrain(max, MIN_ANGLE, MAX_ANGLE);
            this->_maxPW = 2 * MAX_PULSE_WIDTH;
        }
        else
        {
            this->_maxAngle = MAX_ANGLE;  // TODO: has to be calculated
            this->_maxPW = constrain(max, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
        }

        this->_pin = pin;
        this->_isActive = true;
        _servos[this->servoIndex - 1] = pin; // Mark the pin as used
        analogWriteResolution(RESOLUTION);
        setAnalogWriteFrequency(this->_pin, REFRESH_FREQUENCY); // Set the default frequency for the pin

    }
    return this->servoIndex;
}

void Servo::detach()
{
    this->servoIndex = _ServoCount--;

    this->_minAngle = MIN_ANGLE;
    this->_maxAngle = MAX_ANGLE;
    this->_minPW = MIN_PULSE_WIDTH;
    this->_maxPW = MAX_PULSE_WIDTH;

    this->_pin = 0;
    this->_isActive = false;
    this->_pwm = 0;
    this->_deg = 0.0;
    _servos[this->servoIndex - 1] = 0xFF; // Mark the pin as unused
}

void Servo::write(int value)
{
    if (value < MIN_PULSE_CHECK)
    {
        // angle must be inside the boundaries
        double angle = constrain(value, this->_minAngle, this->_maxAngle);
        double dutyCycle = ( 0.5 + ( angle / MAX_ANGLE ) * 2.0 ) * DUTYCYCLE_STEPS;

        this->_deg = angle;
        this->_pwm = uint16_t(dutyCycle);

        analogWrite(this->_pin, uint16_t(dutyCycle));
        _delayUs(50);
    } else {
        writeMicroseconds(value);
    }
}

void Servo::writeMicroseconds(int value)
{
    // value must be inside the boundaries
    double pw = constrain(value,this->_minPW, this->_maxPW);
    double dutyCycle = map(pw, MIN_PULSE_WIDTH,MAX_PULSE_WIDTH, 0.5 * DUTYCYCLE_STEPS, 2.5 * DUTYCYCLE_STEPS);

    this->_deg = ( dutyCycle - DUTYCYCLE_STEPS * 0.5 ) * MAX_ANGLE / ( 2 * DUTYCYCLE_STEPS );
    this->_pwm = uint16_t(dutyCycle);
    analogWrite(this->_pin, uint16_t(dutyCycle));

    _delayUs(50);
}
#endif




