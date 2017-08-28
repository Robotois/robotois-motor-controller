const MotorController = require('../');

const motorController = new MotorController(0);
const motor1 = motorController.createMotor(1);
const motor2 = motorController.createMotor(2);
let pwm1 = -100;
let sum1 = 10;

let pwm2 = -100;
let sum2 = 10;

setInterval(() => {
  motor1.motorPWM(pwm1);

  if (pwm1 === 100) {
    sum1 = -10;
  }

  if (pwm1 === -100) {
    sum1 = 10;
  }

  pwm1 += sum1;
}, 1000);

// setInterval(() => {
//   motor2.motorPWM(pwm2);
//
//   if (pwm2 === 100) {
//     sum2 = -10;
//   }
//
//   if (pwm2 === -100) {
//     sum2 = 10;
//   }
//
//   pwm2 += sum2;
// }, 1000);
