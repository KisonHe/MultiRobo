#ifndef DC_MOTOR_WARPPER_H
#define DC_MOTOR_WARPPER_H

#include <Arduino.h>
#include <ESP32Encoder.h>
#include "ESP32MotorControl.h"
#include "pid.h"
class dc_motor
{
enum RunState_t{Speed_Ctl, Position_Ctl};
private:
mcpwm_timer_t pairID;
int32_t TargetPosition = 0;
int32_t RealPosition = 0;
RunState_t RunState = Speed_Ctl;
int16_t realSpeed = 0;
int16_t TargetSpeed = 0;
ESP32Encoder encoder;

pid* SpeedPID = nullptr;
pid* PositionPID = nullptr;
uint8_t pin_encoder_a;
uint8_t pin_encoder_b;
uint8_t pin_pwm_a;
uint8_t pin_pwm_b;
ESP32MotorControl* MotorControl;
void setSpeed(int8_t speed_);

void Position_Run();
void Speed_Run();
    /* data */
public:
    void handle();
    void runSpeed(int16_t speed_);
    void runPosition(int32_t position_);
    
    int32_t getRealPosition();
    int16_t getSpeed();
    dc_motor(mcpwm_unit_t unitID_, mcpwm_timer_t pairID_, pid* SpeedPID_, pid* PositionPID_, uint8_t pin_encoder_a_,uint8_t pin_encoder_b_,uint8_t pin_pwm_a_,uint8_t pin_pwm_b_, ESP32MotorControl* MotorControl_);
    ~dc_motor();
};



#endif