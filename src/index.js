const MotorModule = require('bindings')('MotorsModule');
const Motor = require('./single-motor');

/**
 * Creates an instance of TemperatureSensor.
 * @param {int} add The second argument.
 * @returns {MotorController} MotorController object.
 */
function MotorController(add = 0) {
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

MotorController.prototype.motor1PWM = function motor1PWM(m1PWM) {
  this.motors.motor1PWM(m1PWM);
};

MotorController.prototype.motor2PWM = function motor2PWM(m2PWM) {
  this.motors.motor2PWM(m2PWM);
};

MotorController.prototype.motor1Stop = function motor1Stop() {
  this.motors.motor1Control(this.stop);
};

MotorController.prototype.motor2Stop = function motor2Stop() {
  this.motors.motor2Control(this.stop);
};

MotorController.prototype.createMotor = function createMotor(motorNumber) {
  return new Motor({
    motorPWM: this.motorPWM(motorNumber),
    motorStop: this.motorStop(motorNumber),
  });
};

MotorController.prototype.motorPWM = function motorPWM(motorNumber) {
  const setPWM = motorNumber === 1 ? this.motor1PWM.bind(this) : this.motor2PWM.bind(this);
  return (speed) => {
    setPWM(speed);
  };
};

MotorController.prototype.motorStop = function motorStop(motorNumber) {
  const stop = motorNumber === 1 ? this.motor1Stop.bind(this) : this.motor2Stop.bind(this);
  return () => {
    stop();
  };
};

/*
Multiple motors functions
 */
MotorController.prototype.motorsPWM = function motorsPWM(m1PWM, m2PWM) {
  this.motors.motorsPWM(m1PWM, m2PWM);
};

MotorController.prototype.motorsStop = function motorsStop() {
  this.motors.motorsControl(this.stop, this.stop);
};

MotorController.prototype.drivePWM = function drivePWM(leftPWM, rightPWM) {
  this.motors.drivePWM(leftPWM, rightPWM);
};

MotorController.prototype.forward = function forward(motorNumber) {
  this.motors[motorNumber === 1 ? 'motor1PWM' : 'motor2PWM'](50);
};

MotorController.prototype.backward = function backward(motorNumber) {
  this.motors[motorNumber === 1 ? 'motor1PWM' : 'motor2PWM'](-50);
};

MotorController.prototype.release = function release() {
  this.motors.release();
};

module.exports = MotorController;
