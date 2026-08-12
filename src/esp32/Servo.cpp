#if defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>
#include <Servo.h>

#if defined __has_include
#  if __has_include ("esp_arduino_version.h")
#    include "esp_arduino_version.h"
#  endif
#  if __has_include ("pinDefinitions.h")
#    include "pinDefinitions.h"
#  endif
#endif

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
#define SERVO_ESP32_LEDC_PIN_API 1
#else
#define SERVO_ESP32_LEDC_PIN_API 0
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

static const uint32_t SERVO_PWM_FREQUENCY_HZ = 1000000 / REFRESH_INTERVAL;

class ServoImpl {
  uint8_t pin;
  uint8_t channel;
  bool is_attached;

public:
    ServoImpl(const uint8_t _pin, const uint8_t _channel) :
      pin(_pin),
      channel(_channel),
      is_attached(false)
    {
#if SERVO_ESP32_LEDC_PIN_API
      is_attached = ledcAttachChannel(pin, SERVO_PWM_FREQUENCY_HZ, LEDC_MAX_BIT_WIDTH, channel);
#else
      ledcSetup(channel, SERVO_PWM_FREQUENCY_HZ, LEDC_MAX_BIT_WIDTH);

      // Attach timer to a LED pin
      ledcAttachPin(pin, channel);
      is_attached = true;
#endif
    }

    ~ServoImpl() {
#if SERVO_ESP32_LEDC_PIN_API
      if (is_attached) {
        ledcDetach(pin);
      }
#else
      ledcDetachPin(pin);
#endif
    }

    bool attached() const {
      return is_attached;
    }

    void set(const uint32_t duration_us) {
      if (!is_attached) {
        return;
      }
#if SERVO_ESP32_LEDC_PIN_API
      ledcWrite(pin, LEDC_US_TO_TICKS(duration_us));
#else
      ledcWrite(channel, LEDC_US_TO_TICKS(duration_us));
#endif
    }

    uint32_t get() const {
      if (!is_attached) {
        return 0;
      }
#if SERVO_ESP32_LEDC_PIN_API
      return LEDC_TICKS_TO_US(ledcRead(pin));
#else
      return LEDC_TICKS_TO_US(ledcRead(channel));
#endif
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
  if (this->servoIndex == INVALID_SERVO) {
    return INVALID_SERVO;
  }

  if (servos[this->servoIndex]) {
    detach();
  }

  ServoImpl* servo = new ServoImpl(pin, this->servoIndex);
  if (!servo) {
    return INVALID_SERVO;
  }

  if (!servo->attached()) {
    delete servo;
    return INVALID_SERVO;
  }

  servos[this->servoIndex] = servo;

  this->min  = (MIN_PULSE_WIDTH - min);
  this->max  = (MAX_PULSE_WIDTH - max);
  return this->servoIndex;
}

void Servo::detach()
{
  if (this->servoIndex == INVALID_SERVO) {
    return;
  }

  delete servos[this->servoIndex];
  servos[this->servoIndex] = NULL;
}

void Servo::write(int value)
{
  if (!attached()) {
    return;
  }

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
  if (this->servoIndex == INVALID_SERVO || !servos[this->servoIndex]) {
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

    servos[this->servoIndex]->set(value);
  }
}

int Servo::read() // return the value as degrees
{
  if (!attached()) {
    return 0;
  }

  return map(readMicroseconds(), SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  if (this->servoIndex == INVALID_SERVO || !servos[this->servoIndex]) {
    return 0;
  }
  return servos[this->servoIndex]->get();
}

bool Servo::attached()
{
  if (this->servoIndex == INVALID_SERVO) {
    return false;
  }

  return servos[this->servoIndex] != NULL;
}

#endif
