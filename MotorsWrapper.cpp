#include <node.h>
#include "MotorsWrapper.h"

using namespace v8;

Persistent<Function> MotorsWrapper::constructor;

MotorsWrapper::MotorsWrapper(uint8_t _add){
  motors = new Motors(_add);
}

MotorsWrapper::~MotorsWrapper(){
  // delete motors;
}

void MotorsWrapper::release(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  MotorsWrapper* temp_obj = ObjectWrap::Unwrap<MotorsWrapper>(args.Holder());
  temp_obj->motors->release();
  delete temp_obj->motors;
}

void MotorsWrapper::Init(){
  Isolate* isolate = Isolate::GetCurrent();
  // Prepare consructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate,New);
  tpl->SetClassName(String::NewFromUtf8(isolate,"MotorsWrapper"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl,"motor1PWM",motor1PWM);
  NODE_SET_PROTOTYPE_METHOD(tpl,"motor1Control",motor1Control);
  NODE_SET_PROTOTYPE_METHOD(tpl,"motor2PWM",motor2PWM);
  NODE_SET_PROTOTYPE_METHOD(tpl,"motor2Control",motor2Control);
  NODE_SET_PROTOTYPE_METHOD(tpl,"motorsPWM",motorsPWM);
  NODE_SET_PROTOTYPE_METHOD(tpl,"motorsControl",motorsControl);
  NODE_SET_PROTOTYPE_METHOD(tpl,"drivePWM",drivePWM);
  NODE_SET_PROTOTYPE_METHOD(tpl,"release",release);

  constructor.Reset(isolate,tpl->GetFunction());
}

void MotorsWrapper::New(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _add = 0x01;
  // If there are two params: First Param => i2c address, second => Port number
  // - Only one Param, this means that the given param is the Port Number,
  MotorsWrapper* obj;
  uint8_t _argc = args.Length();
  if(args.IsConstructCall()){
    // Invoked as constructor: `new MyObject(...)`
    switch(_argc){
      case 0:
        obj = new MotorsWrapper();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
        break;
      case 1:
        _add = (uint8_t) args[0]->NumberValue();
        obj = new MotorsWrapper(_add);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
        break;
      default:
        isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments for Motor Module...")));
    }
  }else{
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    if(_argc > 1){
      isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments for Motor Module...")));
    }
    Local<Value>* argv = new Local<Value>[_argc];
    for(uint8_t i = 0; i < _argc; i++){
      argv[i] = args[i];
    }
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(_argc, argv));
  }
}

void MotorsWrapper::NewInstance(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _argc = args.Length();
  // printf("Args Count: %d\n",_argc);
  if(_argc > 1){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong arguments for Motor Module...")));
  }
  Handle<Value>* argv = new Handle<Value>[_argc];
  for(uint8_t i = 0; i < _argc; i++){
    argv[i] = args[i];
  }
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(_argc, argv);

  args.GetReturnValue().Set(instance);
}

void MotorsWrapper::motor1PWM(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _argc = args.Length();
  if(_argc != 1){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong arguments for Motor Module...")));
  }

  float pwm = args[0]->NumberValue();
  if(pwm > 100 or pwm < -100){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong PWM value for motor1...")));
  }

  MotorsWrapper* temp_obj = ObjectWrap::Unwrap<MotorsWrapper>(args.Holder());
  temp_obj->motors->motor1PWM((int16_t)(pwm*10)); // The PWM range is (-1000)-1000 in the Motor Module
}

void MotorsWrapper::motor1Control(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _argc = args.Length();
  if(_argc != 1){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong arguments...")));
  }

  uint8_t control = (uint8_t) args[0]->NumberValue();

  MotorsWrapper* temp_obj = ObjectWrap::Unwrap<MotorsWrapper>(args.Holder());
  temp_obj->motors->motor1Control(control);
}

void MotorsWrapper::motor2PWM(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _argc = args.Length();
  if(_argc != 1){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong arguments for Motor Module...")));
  }

  float pwm = args[0]->NumberValue();
  if(pwm > 100 or pwm < -100){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong PWM value for motor1...")));
  }

  MotorsWrapper* temp_obj = ObjectWrap::Unwrap<MotorsWrapper>(args.Holder());
  temp_obj->motors->motor2PWM((int16_t)(pwm*10)); // The PWM range is (-1000)-1000 in the Motor Module
}

void MotorsWrapper::motor2Control(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _argc = args.Length();
  if(_argc != 1){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong arguments...")));
  }

  uint8_t control = (uint8_t) args[0]->NumberValue();

  MotorsWrapper* temp_obj = ObjectWrap::Unwrap<MotorsWrapper>(args.Holder());
  temp_obj->motors->motor2Control(control);
}

void MotorsWrapper::motorsControl(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _argc = args.Length();
  if(_argc != 2){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong arguments...")));
  }

  uint8_t control1 = (uint8_t) args[0]->NumberValue();
  uint8_t control2 = (uint8_t) args[1]->NumberValue();

  MotorsWrapper* temp_obj = ObjectWrap::Unwrap<MotorsWrapper>(args.Holder());
  temp_obj->motors->motorsControl(control1,control2);
}

void MotorsWrapper::motorsPWM(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _argc = args.Length();
  if(_argc != 2){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong arguments for Motor Module...")));
  }

  float pwm1 = args[0]->NumberValue();
  float pwm2 = args[1]->NumberValue();

  if(pwm1 > 100 or pwm1 < -100 or pwm2 > 100 or pwm2 < -100){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong PWM value for motors...")));
  }

  MotorsWrapper* temp_obj = ObjectWrap::Unwrap<MotorsWrapper>(args.Holder());
  temp_obj->motors->motorsPWM((int16_t)(pwm1*10),(int16_t)(pwm2*10));
}
void MotorsWrapper::drivePWM(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uint8_t _argc = args.Length();
  if(_argc != 2){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong arguments for Motor Module...")));
  }

  float left_pwm = args[0]->NumberValue(),
    right_pwm = args[1]->NumberValue();

  if(left_pwm > 100 or left_pwm < -100 or right_pwm > 100 or right_pwm < -100){
    isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong PWM value for motors...")));
  }

  MotorsWrapper* temp_obj = ObjectWrap::Unwrap<MotorsWrapper>(args.Holder());
  temp_obj->motors->drivePWM((int16_t)(left_pwm*10),(int16_t)(right_pwm*10));
}
