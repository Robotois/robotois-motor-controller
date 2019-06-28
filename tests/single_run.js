const MController = require('../');

const motorController = MController();
// console.log(motorController.motorCreator);
const motor1 = motorController.motorCreator(1);
// console.log(motor1);
const motor2 = motorController.motorCreator(2);
let pwm1 = 75;
let sum1 = -10;

let pwm2 = -100;
let sum2 = 10;

setInterval(() => {
  motorController.motor1PWM(pwm1);

  if (pwm1 === 75) {
    sum1 = -10;
  }

  if (pwm1 === -75) {
    sum1 = 10;
  }

  pwm1 += sum1;
}, 2000);

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
