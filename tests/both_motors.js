const Motors = require('../');

const motors = new Motors();

let pwm = -50;
let sum = 10;

setInterval(() => {
  motors.motorsPWM(pwm, pwm);

  if (pwm === 100) {
    sum = -10;
  }

  if (pwm === -100) {
    sum = 10;
  }

  pwm += sum;
}, 1000);

process.on('SIGTERM', () => {
  process.exit();
});

process.on('SIGINT', () => {
  process.exit();
});
