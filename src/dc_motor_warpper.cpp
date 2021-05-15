#include "dc_motor_warpper.h"

dc_motor::dc_motor(uint8_t ID_, pid* SpeedPID_, pid* PositionPID_, uint8_t pin_encoder_a_,uint8_t pin_encoder_b_,uint8_t pin_pwm_a_,uint8_t pin_pwm_b_, ESP32MotorControl& MotorControl_):\
ID(ID_), SpeedPID(SpeedPID_), PositionPID(PositionPID_), pin_encoder_a(pin_encoder_a_), pin_encoder_b(pin_encoder_b_), pin_pwm_a(pin_pwm_a_), pin_pwm_b(pin_pwm_b_), MotorControl(MotorControl_)
{
    encoder.attachHalfQuad(pin_encoder_a_,pin_encoder_b_);
    MotorControl.attachMotor(pin_pwm_a_, pin_pwm_b_);
    RunState = Speed_Ctl;
}

dc_motor::~dc_motor()
{
}

void dc_motor::setSpeed(int8_t speed_){
    speed_ = speed_ > 100 ? 100 : speed_;
    speed_ = speed_ < -100 ? -100 : speed_;
    if (speed_ >= 0) 
        MotorControl.motorForward(ID, speed_);
    else if (speed_ < 0) 
        MotorControl.motorReverse(ID, -speed_);
}

void dc_motor::handle(){
    RealPosition = encoder.getCount();
    realSpeed = getSpeed();
    switch(RunState)
	{
    case Speed_Ctl://电机在以速度态运行
        Speed_Run();
        break;
    case Position_Ctl://路程
        Position_Run();
        break;
    default:
        break;
	}
}

int16_t dc_motor::getSpeed(){
    static int32_t lastPosition = 0;
    static uint32_t lastTime = 0;
    if ((millis() - lastTime) < 3)
        return realSpeed;   //too fast, data is useless
    
    int32_t speedRaw = (int32_t)(((int32_t)(RealPosition - lastPosition) * 100) / (int32_t)(millis() - lastTime));     //make the compiler use int32 division
    // Serial.println("(R - L) * 100= " + String((int32_t)((RealPosition - lastPosition) * 100)));
    // Serial.println("RealPosition = " + String(RealPosition) + "\nlastPosition = " + String(lastPosition));
    // Serial.println("Time = " + String(millis()) + "\nlastTime = " + String(lastTime));
    // Serial.println("Time - lastTime = " + String(millis() - lastTime));
    // Serial.println("speedRaw = " + String(speedRaw));
    lastPosition = RealPosition;
    lastTime = millis();
    //FIXME: map the raw speed to -100 -- 100 here
    return speedRaw;

}

void dc_motor::Position_Run()
{
	if(PositionPID == nullptr) while(1);//如果没有外环pid的话直接暴露错误
	int16_t err = TargetPosition - RealPosition;//此时得数误差为路程环误差
	err = PositionPID->pid_run(err);//计算后此时err为目标速度
	TargetSpeed = err;
	Speed_Run();//把目标速度放进速度控制里
}

void dc_motor::Speed_Run()
{
    if (SpeedPID == nullptr){
        setSpeed(TargetSpeed);
    }
    else{
        int16_t feedSpeed = TargetSpeed + SpeedPID->pid_run(TargetSpeed - realSpeed);
        setSpeed(feedSpeed);
    }
    
}
void dc_motor::runSpeed(int16_t speed_){
    RunState = Speed_Ctl;
    TargetSpeed = speed_;
}
void dc_motor::runPosition(int32_t position_){
    RunState = Position_Ctl;
    TargetPosition = position_;
}

int32_t dc_motor::getRealPosition(){
    return RealPosition;
}