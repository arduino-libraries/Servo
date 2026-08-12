// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CI13XX_SERVO_TIMERS_H
#define CI13XX_SERVO_TIMERS_H

#include <Arduino.h>

// The CI13XX family exposes six hardware PWM channels. Some package pins map
// to the same channel, so the implementation also tracks channel ownership.
#define CI13XX_MAX_SERVOS 6

class Servo {
public:
  Servo();

  uint8_t attach(int pin);
  uint8_t attach(int pin, int minPulse, int maxPulse);
  void detach();

  void write(int value);
  void writeMicroseconds(int value);

  int read();
  int readMicroseconds();
  bool attached();

private:
  bool startPwm();
  void updatePwm();

  uint8_t _servoIndex;
  uint8_t _pin;
  int8_t _channel;
  uint16_t _minPulse;
  uint16_t _maxPulse;
  uint16_t _pulse;
  bool _attached;
};

#endif
