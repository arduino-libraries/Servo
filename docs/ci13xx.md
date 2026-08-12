# CI13XX support

The CI13XX backend implements the standard Arduino `Servo` API with the six
hardware PWM channels in CI1302, CI1303 and CI1306 chips. It generates a 50 Hz
frame and supports both angle and pulse-width control.

Only pins marked as PWM-capable by the selected ChipIntelli board variant can
be attached. Pins that share the same underlying PWM channel cannot drive two
servos at the same time. The core resource manager also prevents conflicts
with `analogWrite()`, `tone()`, SPI, UART and Wire before changing a pin's mux.
Calling `detach()` stops the PWM channel and releases its pin and channel.

CI1306 exposes all six PWM channels. CI1302 and CI1303 can expose all six when
using the internal RC clock; when the external crystal is selected, its PA0
pad is unavailable and five PWM channels remain.

The `Sweep`, `Knob` and `SerialControl` examples use PA5/PWM3 on CI1302 and
CI1303, avoiding the PC4 amplifier-control connection found on some CI1303
modules. They use PB3/PWM4 on CI1306.

Power the servo motor from a suitable external supply and connect the supply
ground to the CI13XX board ground. Do not power a servo motor from a GPIO pin.
Check the board's current capability and the servo's signal voltage before
connecting it.
