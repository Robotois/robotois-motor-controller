var _motors = require('../index.js'),
  motors = new _motors();

var pwm1 = -50,sum1 = 5;
var pwm2 = -50,sum2 = 5;

setInterval(()=> {
  motors.motor1PWM(pwm1);

  if(pwm1 == 50){
    sum1 = -5;
  }

  if(pwm1 == -50){
    sum1 = 5;
  }

  pwm1 += sum1;
},1000);

setInterval(()=> {
  motors.motor2PWM(pwm2);

  if(pwm2 == 50){
    sum2 = -5;
  }

  if(pwm2 == -50){
    sum2 = 5;
  }

  pwm2 += sum2;
},1000);

process.on('SIGTERM', function () {
  process.exit();
});

process.on('SIGINT', function () {
  process.exit();
});
