//Load in the required main file
Servo = require('../../src/JerryScript/main')

//Define the servo pin here
const pin = 11

//sweep the servo 
setInterval(() => {
    for (let pos = 0; pos <= 1; pos += 0.1) {
        setTimeout(Servo.setServo(pin, pos), 50);
    }
    for (let pos = 1; pos >= 0; pos -= 0.1) {
        setTimeout(Servo.setServo(pin, pos), 50);
    }
}, 1000); // hault for 1 second before returning

