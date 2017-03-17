const Motors = require('../index.js');

const motors = new Motors();

let pwm = -50;
let sum = 5;

setInterval(() => {
  motors.motorsPWM(pwm, pwm);

  if (pwm === 50) {
    sum = -5;
  }

  if (pwm === -50) {
    sum = 5;
  }

  pwm += sum;
}, 1000);

process.on('SIGTERM', () => {
  process.exit();
});

process.on('SIGINT', () => {
  process.exit();
});
