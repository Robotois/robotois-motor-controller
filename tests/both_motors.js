var _motors = require('../index.js'),
  motors = new _motors();

var pwm = -50,sum = 5;

setInterval(()=> {
  motors.motorsPWM(pwm,pwm);

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
