#include <Servo.h> // Corrected: Include the library first

/* * --------------------------------------------------------------------------
 * writeMicroseconds_Example 
 * --------------------------------------------------------------------------
 * The writeMicroseconds() function is part of the Servo Library.
 * It sets the servo's angle by accepting a pulse width duration measured in microseconds (µs).
 * * CORE TIMING VALUES (Note: Values can vary slightly by servo model):
 * * 1. Minimum Position (0 degrees):
 * - Value: 544 microseconds (us)
 * - Result: Servo moves to its extreme counter-clockwise limit (0°).
 * * 2. Center Position (90 degrees):
 * - Value: 1500 microseconds (us)
 * - Result: Servo moves to its neutral, center position (90°).
 * * 3. Maximum Position (180 degrees):
 * - Value: 2400 microseconds (us)
 * - Result: Servo moves to its extreme clockwise limit (180°).
 *
 * * BASIC WORKING PRINCIPLE:
 * Servo motors use Pulse Width Modulation (PWM). The duration (width) that the
 * control wave is held HIGH directly determines the commanded angle. This function
 * allows for precise, non-standard movement by letting the user specify the exact time duration.
 */

// Create a servo Object. We'll use myservo for standardization.
Servo myservo; // Correction: Added standard name and semicolon

void setup() {
  // 1. Attach the servo object to a PWM pin (e.g., Digital Pin 9).
  myservo.attach(9); 
  
  // 2. Initialize Serial Monitor to explain the timing values.
  Serial.begin(9600);
  Serial.println("--- Servo Microseconds Example Running ---");
}

void loop() {
  // 1. Write CENTER position (Standard 90 degrees)
  myservo.writeMicroseconds(1500);
  Serial.println("Position: Center (1500 us)");
  delay(1500); // Wait 1.5 seconds

  // 2. Write MINIMUM position (Standard 0 degrees)
  myservo.writeMicroseconds(544);
  Serial.println("Position: Min (544 us)");
  delay(1500); 

  // 3. Write MAXIMUM position (Standard 180 degrees)
  myservo.writeMicroseconds(2400);
  Serial.println("Position: Max (2400 us)");
  delay(1500); 
}