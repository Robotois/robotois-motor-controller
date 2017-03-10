const MotorModule = require('bindings')('MotorsModule');
/**
 * Creates an instance of TemperatureSensor.
 * @param {int} add The second argument.
 * @returns {MotorsModule} MotorsModule object.
 */
function MotorsModule(add = 0) {
  const self = this;
  this.motors = new MotorModule(add);
  this.stop = 0x00;

  process.on('SIGINT', () => {
    self.motors.release();
  });

  process.on('SIGTERM', () => {
    self.motors.release();
  });
}

MotorsModule.prototype.motor1PWM = function motor1PWM(m1PWM) {
  this.motors.motor1PWM(m1PWM);
};

MotorsModule.prototype.motor2PWM = function motor2PWM(m2PWM) {
  this.motors.motor2PWM(m2PWM);
};

MotorsModule.prototype.motor1Stop = function motor1Stop() {
  this.motors.motor1Control(this.stop);
};

MotorsModule.prototype.motor2Stop = function motor2Stop() {
  this.motors.motor2Control(this.stop);
};

MotorsModule.prototype.motorsPWM = function motorsPWM(m1PWM, m2PWM) {
  this.motors.motorsPWM(m1PWM, m2PWM);
};

MotorsModule.prototype.motorsStop = function motorsStop() {
  this.motors.motorsControl(this.stop, this.stop);
};

MotorsModule.prototype.drivePWM = function drivePWM(leftPWM, rightPWM) {
  this.motors.drivePWM(leftPWM, rightPWM);
};

MotorsModule.prototype.forward = function drivePWM(motorNumber) {
  this.motors[motorNumber === 1 ? 'motor1PWM' : 'motor2PWM'](50);
};

MotorsModule.prototype.backward = function drivePWM(motorNumber) {
  this.motors[motorNumber === 1 ? 'motor1PWM' : 'motor2PWM'](-50);
};

module.exports = MotorsModule;
