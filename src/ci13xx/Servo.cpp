// SPDX-License-Identifier: LGPL-2.1-or-later

#if defined(ARDUINO_ARCH_CI13XX)

#include <Servo.h>
#include <PeripheralManager.h>

extern "C" {
#include <ci130x_dpmu.h>
#include <ci130x_pwm.h>
#include <ci130x_scu.h>
#include <platform_config.h>
}

namespace {
Servo *channelOwners[MAX_SERVOS] = {};
uint8_t servoCount = 0;

pwm_base_t pwmBase(uint8_t channel) {
  static const pwm_base_t bases[MAX_SERVOS] = {
      PWM0, PWM1, PWM2, PWM3, PWM4, PWM5};
  return bases[channel < MAX_SERVOS ? channel : 0];
}

uint16_t clampPulse(int value, uint16_t minimum, uint16_t maximum) {
  if (value < static_cast<int>(minimum)) {
    return minimum;
  }
  if (value > static_cast<int>(maximum)) {
    return maximum;
  }
  return static_cast<uint16_t>(value);
}

bool calculateDuty(uint16_t pulse, uint32_t &duty, uint32_t &dutyMax) {
  const uint32_t sourceClock = get_src_clk();
  const uint32_t period = sourceClock / (1000000U / REFRESH_INTERVAL);
  if (period == 0U) {
    return false;
  }

  dutyMax = REFRESH_INTERVAL;
  const uint32_t safeMaximum = UINT32_MAX / period;
  if (dutyMax > safeMaximum) {
    dutyMax = safeMaximum;
  }
  if (dutyMax == 0U) {
    return false;
  }

  duty = static_cast<uint32_t>(
      (static_cast<uint64_t>(pulse) * dutyMax + REFRESH_INTERVAL / 2U) /
      REFRESH_INTERVAL);
  if (duty > dutyMax) {
    duty = dutyMax;
  }
  return true;
}
}  // namespace

Servo::Servo()
    : _servoIndex(servoCount < MAX_SERVOS ? servoCount++ : INVALID_SERVO),
      _pin(INVALID_SERVO),
      _channel(-1),
      _minPulse(MIN_PULSE_WIDTH),
      _maxPulse(MAX_PULSE_WIDTH),
      _pulse(DEFAULT_PULSE_WIDTH),
      _attached(false) {}

uint8_t Servo::attach(int pin) {
  return attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int minPulse, int maxPulse) {
  if (_servoIndex == INVALID_SERVO || pin < 0 ||
      pin >= static_cast<int>(NUM_DIGITAL_PINS) || minPulse < 1 ||
      maxPulse >= REFRESH_INTERVAL || minPulse >= maxPulse) {
    return INVALID_SERVO;
  }

  const PinDescription &description = g_APinDescription[pin];
  if ((description.capabilities & PIN_CAP_PWM) == 0 ||
      description.pwmChannel < 0 || description.pwmChannel >= MAX_SERVOS) {
    return INVALID_SERVO;
  }

  const uint8_t channel = static_cast<uint8_t>(description.pwmChannel);
  if (channelOwners[channel] != nullptr && channelOwners[channel] != this) {
    return INVALID_SERVO;
  }

  if (_attached && _pin != static_cast<uint8_t>(pin)) {
    detach();
  }

  const uint8_t claimedPin = static_cast<uint8_t>(pin);
  const PeripheralResource resource = static_cast<PeripheralResource>(
      static_cast<uint8_t>(PeripheralResource::Pwm0) + channel);
  if (!PeripheralManager.claim(PeripheralOwner::Servo, &claimedPin, 1,
                               &resource, 1)) {
    return INVALID_SERVO;
  }
  detachInterrupt(claimedPin);

  _pin = claimedPin;
  _channel = static_cast<int8_t>(channel);
  _minPulse = static_cast<uint16_t>(minPulse);
  _maxPulse = static_cast<uint16_t>(maxPulse);
  _pulse = clampPulse(_pulse, _minPulse, _maxPulse);
  channelOwners[channel] = this;

  if (!startPwm()) {
    channelOwners[channel] = nullptr;
    PeripheralManager.release(PeripheralOwner::Servo, &claimedPin, 1,
                              &resource, 1);
    _pin = INVALID_SERVO;
    _channel = -1;
    _attached = false;
    return INVALID_SERVO;
  }

  _attached = true;
  return _servoIndex;
}

void Servo::detach() {
  if (!_attached || _channel < 0 || _channel >= MAX_SERVOS) {
    return;
  }

  const uint8_t channel = static_cast<uint8_t>(_channel);
  pwm_stop(pwmBase(channel));
  (void)pinModeOwned(_pin, OUTPUT, PeripheralOwner::Servo);
  digitalWrite(_pin, LOW);
  if (channelOwners[channel] == this) {
    channelOwners[channel] = nullptr;
  }

  const uint8_t releasedPin = _pin;
  const PeripheralResource resource = static_cast<PeripheralResource>(
      static_cast<uint8_t>(PeripheralResource::Pwm0) + channel);
  PeripheralManager.release(PeripheralOwner::Servo, &releasedPin, 1,
                            &resource, 1);

  _pin = INVALID_SERVO;
  _channel = -1;
  _attached = false;
}

void Servo::write(int value) {
  if (value < MIN_PULSE_WIDTH) {
    if (value < 0) {
      value = 0;
    } else if (value > 180) {
      value = 180;
    }
    value = static_cast<int>(
        (static_cast<int64_t>(value) * (_maxPulse - _minPulse) + 90) / 180 +
        _minPulse);
  }
  writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value) {
  if (_servoIndex == INVALID_SERVO) {
    return;
  }
  _pulse = clampPulse(value, _minPulse, _maxPulse);
  if (_attached) {
    updatePwm();
  }
}

int Servo::read() {
  if (_maxPulse <= _minPulse) {
    return 0;
  }
  return static_cast<int>(
      (static_cast<uint32_t>(_pulse - _minPulse) * 180U +
       (_maxPulse - _minPulse) / 2U) /
      (_maxPulse - _minPulse));
}

int Servo::readMicroseconds() {
  return _servoIndex == INVALID_SERVO ? 0 : _pulse;
}

bool Servo::attached() {
  return _attached;
}

bool Servo::startPwm() {
  uint32_t duty = 0;
  uint32_t dutyMax = 0;
  if (_channel < 0 || !calculateDuty(_pulse, duty, dutyMax)) {
    return false;
  }

  const PinDescription &description = g_APinDescription[_pin];
  const pwm_base_t base = pwmBase(static_cast<uint8_t>(_channel));
  scu_set_device_gate(static_cast<uint32_t>(base), ENABLE);
  dpmu_set_adio_reuse(static_cast<PinPad_Name>(description.pad), DIGITAL_MODE);
  dpmu_set_io_reuse(static_cast<PinPad_Name>(description.pad),
                    static_cast<IOResue_FUNCTION>(description.pwmMux));

  pwm_init_t config = {
      1U, 1000000U / REFRESH_INTERVAL, duty, dutyMax};
  pwm_init(base, config);
  pwm_set_restart_md(base, 0);
  pwm_start(base);
  return true;
}

void Servo::updatePwm() {
  uint32_t duty = 0;
  uint32_t dutyMax = 0;
  if (_channel < 0 || !calculateDuty(_pulse, duty, dutyMax)) {
    return;
  }
  pwm_set_duty(pwmBase(static_cast<uint8_t>(_channel)), duty, dutyMax);
}

#endif  // defined(ARDUINO_ARCH_CI13XX)
