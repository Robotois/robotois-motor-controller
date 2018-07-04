const MotorModule = require('bindings')('MotorsModule');
const Motor = require('./single-motor');

/**
 * Creates an instance of TemperatureSensor.
 * @param {int} add The second argument.
 * @returns {MotorController} MotorController object.
 */
function MotorController({ client, topic }, add = 0, force = 0) {
  const self = this;
  this.motors = new MotorModule(add);
  this.stop = 0x00;
  this.client = client;
  this.topic = topic;
  this.force = force;

  process.on('SIGINT', () => {
    self.release();
  });

  process.on('SIGTERM', () => {
    self.release();
  });
}

MotorController.prototype.initClient = function initClient() {
  if (this.client) {
    this.client.subscribe(this.topic);
  }
};

MotorController.prototype.publishValue = function publishValue(motorNumber, value) {
  if (!this.client) {
    return;
  }
  const message = {
    reported: {
      motors: {
        [`motor${motorNumber}`]: value,
      },
    },
  };

  this.client.publish(this.topic, JSON.stringify(message));
};

MotorController.prototype.motor1PWM = function motor1PWM(m1PWM) {
  this.motors.motor1PWM(m1PWM, this.force);
  this.publishValue(1, m1PWM);
};

MotorController.prototype.motor2PWM = function motor2PWM(m2PWM) {
  this.motors.motor2PWM(m2PWM, this.force);
  this.publishValue(2, m2PWM);
};

MotorController.prototype.motor1Stop = function motor1Stop() {
  this.motors.motor1Control(this.stop, this.force);
};

MotorController.prototype.motor2Stop = function motor2Stop() {
  this.motors.motor2Control(this.stop, this.force);
};

MotorController.prototype.createMotor = function createMotor(motorNumber, mqttConfig) {
  return new Motor(
    {
      motorPWM: this.motorPWM(motorNumber),
      motorStop: this.motorStop(motorNumber),
    },
    mqttConfig,
  );
};

MotorController.prototype.motorCreator = function motorCreator(motorNumber) {
  const pwm = motorNumber === 1 ? this.motor1PWM.bind(this) : this.motor2PWM.bind(this);
  // const update = this.motorUpdate(motorNumber);
  const stop = motorNumber === 1 ? this.motor1Stop.bind(this) : this.motor2Stop.bind(this);
  const motor = {
    pwm,
    stop,
  };
  return motor;
};

MotorController.prototype.motorUpdate = function motorUpdate(motorNumber) {
  const setPWM = motorNumber === 1 ? this.motor1PWM.bind(this) : this.motor2PWM.bind(this);
  return (jsonState) => {
    const { desired } = jsonState;
    if (desired && desired.motors && desired.motors[`motor${motorNumber}`]) {
      setPWM(Number(desired.motors[`motor${motorNumber}`]));
    }
  };
};

MotorController.prototype.singleMotorPublish = function singleMotorPublish(motorNumber) {
  return (value) => {
    this.publishValue(motorNumber, value);
  };
};

MotorController.prototype.singleMotorPWM = function singleMotorPWM(num) {
  const setPWM = num === 1 ? this.motor1PWM.bind(this) : this.motor2PWM.bind(this);
  return setPWM;
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
  this.motors.motorsPWM(m1PWM, m2PWM, this.force);
};

MotorController.prototype.motorsStop = function motorsStop() {
  this.motors.motorsControl(this.stop, this.stop, this.force);
};

MotorController.prototype.drivePWM = function drivePWM(leftPWM, rightPWM) {
  this.motors.drivePWM(leftPWM, rightPWM, this.force);
};

MotorController.prototype.forward = function forward(motorNumber) {
  this.motors[motorNumber === 1 ? 'motor1PWM' : 'motor2PWM'](50, this.force);
};

MotorController.prototype.backward = function backward(motorNumber) {
  this.motors[motorNumber === 1 ? 'motor1PWM' : 'motor2PWM'](-50, this.force);
};

MotorController.prototype.release = function release() {
  // this.motorsStop();
  this.motors.release();
};

const init = (opt = {}, addr = 0, force = 1) => {
  // const { client, topic } = opt;
  const motors = new MotorController(opt, addr, force);
  // console.log(motors.motorCreator);
  return motors;
};

module.exports = init;
