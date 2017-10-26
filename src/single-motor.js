function Motor(motorSettings, mqttConfig) {
  this.pwm = motorSettings.motorPWM;
  this.stop = motorSettings.motorStop;
  this.prevSpeed = 1000;

  if (mqttConfig) {
    this.mqttClient = mqttConfig.mqttClient;
    this.myTopic = `motors/motor${mqttConfig.instance}`;
  }
}

Motor.prototype.motorPWM = function motorSpeed(speed) {
  if (this.prevSpeed !== speed) {
    this.pwm(speed);
    if (this.mqttClient) {
      this.mqttClient.publish(this.myTopic, speed.toString());
      this.prevSpeed = speed;
    }
  }
};

Motor.prototype.motorStop = function motorStop() {
  this.stop();
  if (this.mqttClient) {
    this.mqttClient.publish(this.myTopic, '0');
    this.prevSpeed = 0;
  }
};

module.exports = Motor;
