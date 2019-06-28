const MController = require('../');

const motorController = MController();
// console.log(motorController.motorCreator);
const motor1 = motorController.motorCreator(1);
// console.log(motor1);
const motor2 = motorController.motorCreator(2);
let pwm1 = -100;
let sum1 = 10;

let pwm2 = -100;
let sum2 = 10;

setInterval(() => {
  motor1.pwm(pwm1);

  if (pwm1 === 50) {
    sum1 = -10;
  }

  if (pwm1 === -50) {
    sum1 = 10;
  }

  pwm1 += sum1;
}, 500);

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
