#ifndef MOTORSWRAPPER_H
#define MOTORSWRAPPER_H

#include <node.h>
#include <node_object_wrap.h>
#include "../Modules/Motors/Motors.h"

class MotorsWrapper : public node::ObjectWrap {
public:
  static void Init();
  static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
  explicit MotorsWrapper(uint8_t _add = 0x00);
  ~MotorsWrapper();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void motor1PWM(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void motor1Control(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void motor2PWM(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void motor2Control(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void motorsPWM(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void motorsControl(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void drivePWM(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void release(const v8::FunctionCallbackInfo<v8::Value>& args);

  static v8::Persistent<v8::Function> constructor;

  Motors *motors;
};

#endif
