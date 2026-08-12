/*
  Control a servo by entering an angle in the Serial Monitor.

  Power the servo from a suitable external supply and connect its ground to
  the board ground. Do not power a servo motor from a GPIO pin.
*/

#include <Servo.h>

#if defined(ARDUINO_ARCH_CI13XX)
#if defined(CI_CHIP_CI1302) || defined(CI_CHIP_CI1303)
const int servoPin = PA5;  // PWM3; avoids PC4 amplifier control
#else
const int servoPin = PB3;  // PWM4 on CI1306
#endif
#else
const int servoPin = 9;
#endif

Servo myservo;
unsigned int pendingAngle = 0;
bool inputReceived = false;
bool digitsReceived = false;
bool inputValid = true;

void finishInput() {
  if (!inputReceived) {
    return;
  }

  if (inputValid && digitsReceived) {
    myservo.write(static_cast<int>(pendingAngle));
    Serial.print("Servo angle: ");
    Serial.println(pendingAngle);
  } else {
    Serial.println("Enter an angle from 0 to 180.");
  }

  pendingAngle = 0;
  inputReceived = false;
  digitsReceived = false;
  inputValid = true;
}

void setup() {
  Serial.begin(115200);
  myservo.attach(servoPin);

  if (!myservo.attached()) {
    Serial.println("Servo attach failed. Check that the pin is PWM-capable and free.");
    return;
  }

  myservo.write(90);
  Serial.print("Servo attached to pin ");
  Serial.println(servoPin);
  Serial.println("Enter an angle from 0 to 180, then press Enter.");
}

void loop() {
  while (Serial.available() > 0) {
    const int incoming = Serial.read();

    if (incoming >= '0' && incoming <= '9') {
      inputReceived = true;
      digitsReceived = true;
      if (pendingAngle <= 18U) {
        pendingAngle = pendingAngle * 10U +
                       static_cast<unsigned int>(incoming - '0');
        if (pendingAngle > 180U) {
          inputValid = false;
        }
      } else {
        inputValid = false;
      }
    } else if (incoming == '\r' || incoming == '\n') {
      finishInput();
    } else if (incoming != ' ' && incoming != '\t') {
      inputReceived = true;
      inputValid = false;
    }
  }
}
