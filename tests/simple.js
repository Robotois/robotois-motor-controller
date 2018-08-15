const Motors = require('../');

const motors = Motors();

motors.motor1PWM(50);

setTimeout(() => {
  motors.motor1Stop();
  motors.release();
}, 3000);
