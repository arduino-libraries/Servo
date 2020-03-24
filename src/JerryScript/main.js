//The setServo function takes in 2 arguments
//      1st Argument (pin) is the pin to which the servo is attached
//      2nd Argumnet (pos) is the pos between 0 & 1 which maps to 0 & 180 

function setServo(pin, pos) {
    if (pos < 0) pos = 0;
    if (pos > 1) pos = 1;
    analogWrite(pin, (1 + pos) / 50.0, { freq: 20 });
}

module.exports = {
    setServo()
}