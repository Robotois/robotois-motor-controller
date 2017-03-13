var motorsModule = require('bindings')('MotorsModule');

function MotorsModule(_add = 0) {
  var self = this;
  this.motors = new motorsModule(_add);
  this.stop = 0x00;

  process.on('SIGINT', function () {
    self.motors.release();
  });

  process.on('SIGTERM', function () {
    self.motors.release();
  });
}

MotorsModule.prototype.motor1PWM = function(m1PWM){
  this.motors.motor1PWM(m1PWM);
};

MotorsModule.prototype.motor2PWM = function(m2PWM){
  this.motors.motor2PWM(m2PWM);
};

MotorsModule.prototype.motor1Stop = function(){
  this.motors.motor1Control(this.stop);
};

MotorsModule.prototype.motor2Stop = function(){
  this.motors.motor2Control(this.stop);
};

MotorsModule.prototype.motorsPWM = function(m1PWM,m2PWM){
  this.motors.motorsPWM(m1PWM,m2PWM);
};

MotorsModule.prototype.motorsStop = function(){
  this.motors.motorsControl(this.stop,this.stop);
};

MotorsModule.prototype.drivePWM = function(leftPWM,rightPWM){
  this.motors.drivePWM(leftPWM,rightPWM);
};

MotorsModule.prototype.release = function(){
  this.motors.release();
};

module.exports = MotorsModule;
