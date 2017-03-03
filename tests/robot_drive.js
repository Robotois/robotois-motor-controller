var _motors = require('../index.js'),
  motors = new _motors();

var pwm = -50,sum = 5;

setInterval(()=> {
  // The drive PWM considers that the motor 1 is moving the right side wheel of a robot,
  // therefore the motor 2 is moving the left side wheel of the robot.
  motors.drivePWM(pwm,pwm);

  if(pwm == 50){
    sum = -5;
  }

  if(pwm == -50){
    sum = 5;
  }

  pwm += sum;
},1000);

  process.on('SIGTERM', function () {
    process.exit();
  });

  process.on('SIGINT', function () {
    process.exit();
  });
