const Motors = require('../index.js');

const motors = new Motors();

let pwm = -50;
let sum = 10;

setInterval(() => {
  // The drive PWM considers that the motor 1 is moving the right side wheel of a robot,
  // therefore the motor 2 is moving the left side wheel of the robot.
  motors.drivePWM(pwm, pwm);

  if (pwm === 50) {
    sum = -10;
  }

  if (pwm === -50) {
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
