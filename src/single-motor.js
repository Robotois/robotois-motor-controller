function motor(motorFunctions) {
  this.motorPWM = motorFunctions.motorPWM;
  this.motorStop = motorFunctions.motorStop;
}

motor.prototype.motorSpeed = function motorSpeed(speed) {
  this.motorPWM(speed);
};

motor.prototype.motorStop = function motorStop() {
  this.motorStop();
};

module.exports = motor;
