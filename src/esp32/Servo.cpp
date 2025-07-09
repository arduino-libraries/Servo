#if defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>
#include <Servo.h>

#if defined __has_include
#  if __has_include ("pinDefinitions.h")
#    include "pinDefinitions.h"
#  endif
#endif

/*
 * This group/channel/timmer mapping is for information only;
 * the details are handled by lower-level code
 *
 * LEDC Chan to Group/Channel/Timer Mapping
 ** ledc: 0  => Group: 0, Channel: 0, Timer: 0
 ** ledc: 1  => Group: 0, Channel: 1, Timer: 0
 ** ledc: 2  => Group: 0, Channel: 2, Timer: 1
 ** ledc: 3  => Group: 0, Channel: 3, Timer: 1
 ** ledc: 4  => Group: 0, Channel: 4, Timer: 2
 ** ledc: 5  => Group: 0, Channel: 5, Timer: 2
 ** ledc: 6  => Group: 0, Channel: 6, Timer: 3
 ** ledc: 7  => Group: 0, Channel: 7, Timer: 3
 ** ledc: 8  => Group: 1, Channel: 0, Timer: 0
 ** ledc: 9  => Group: 1, Channel: 1, Timer: 0
 ** ledc: 10 => Group: 1, Channel: 2, Timer: 1
 ** ledc: 11 => Group: 1, Channel: 3, Timer: 1
 ** ledc: 12 => Group: 1, Channel: 4, Timer: 2
 ** ledc: 13 => Group: 1, Channel: 5, Timer: 2
 ** ledc: 14 => Group: 1, Channel: 6, Timer: 3
 ** ledc: 15 => Group: 1, Channel: 7, Timer: 3
 */

class ServoImpl {
  uint8_t pin;

public:
    ServoImpl(const uint8_t _pin, const uint8_t _channel) : pin(_pin) {
      // Setup timer
      ledcSetup(_channel, (1000000 / REFRESH_INTERVAL), LEDC_MAX_BIT_WIDTH);

      // Attach timer to a LED pin
      ledcAttachPin(pin, _channel);
    }

    ~ServoImpl() {
      ledcDetachPin(pin);
    }

    void set(const uint8_t _channel, const uint32_t duration_us) {
      ledcWrite(_channel, LEDC_US_TO_TICKS(duration_us));
    }

    uint32_t get(const uint8_t _channel) const {
      return LEDC_TICKS_TO_US(ledcRead(_channel));
    }
};

static ServoImpl* servos[MAX_PWM_SERVOS] = {nullptr};      // static array of servo structures
uint8_t ServoCount = 0;                                    // the total number of attached servos

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min)   // minimum value in us for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max)   // maximum value in us for this servo

Servo::Servo()
{
  if (ServoCount < MAX_PWM_SERVOS) {
    this->servoIndex = ServoCount++;
  } else {
    this->servoIndex = INVALID_SERVO;  // too many servos
  }
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  servos[this->servoIndex] = new ServoImpl(pin, this->servoIndex);

  this->min  = (MIN_PULSE_WIDTH - min);
  this->max  = (MAX_PULSE_WIDTH - max);
  return this->servoIndex;
}

void Servo::detach()
{
  delete servos[this->servoIndex];
  servos[this->servoIndex] = NULL;
}

void Servo::write(int value)
{
  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
  if (value < MIN_PULSE_WIDTH)
  {
    if (value < 0)
      value = 0;
    else if (value > 180)
      value = 180;

    value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
  }
  writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  if (!servos[this->servoIndex]) {
    return;
  }
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_PWM_SERVOS) )   // ensure channel is valid
  {
    if (value < SERVO_MIN())          // ensure pulse width is valid
      value = SERVO_MIN();
    else if (value > SERVO_MAX())
      value = SERVO_MAX();

    servos[this->servoIndex]->set(this->servoIndex, value);
  }
}

int Servo::read() // return the value as degrees
{
  return map(readMicroseconds(), SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  if (!servos[this->servoIndex]) {
    return 0;
  }
  return servos[this->servoIndex]->get(this->servoIndex);
}

bool Servo::attached()
{
  return servos[this->servoIndex] != NULL;
}

#endif
