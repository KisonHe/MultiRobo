#include <Arduino.h>
#include "chassis_warpper.h"
#include "dc_motor_warpper.h"
#include "mqtt.h"


ESP32MotorControl DownMotorControl = ESP32MotorControl(MCPWM_UNIT_0);
ESP32MotorControl UpMotorControl = ESP32MotorControl(MCPWM_UNIT_1);
//Down left
//(mcpwm_unit_t unitID_, mcpwm_timer_t pairID_, pid* SpeedPID_, pid* PositionPID_, uint8_t pin_encoder_a_,uint8_t pin_encoder_b_,uint8_t pin_pwm_a_,uint8_t pin_pwm_b_, ESP32MotorControl& MotorControl_);
// dc_motor DL(MCPWM_UNIT_0, MCPWM_TIMER_0,nullptr,nullptr,25,26,16,17,&DownMotorControl);
// dc_motor DR(MCPWM_UNIT_0, MCPWM_TIMER_1,nullptr,nullptr,39,36,19,15,&DownMotorControl);
dc_motor UL(MCPWM_UNIT_1, MCPWM_TIMER_0,nullptr,nullptr,4,2,21,23,&UpMotorControl);
dc_motor UR(MCPWM_UNIT_1, MCPWM_TIMER_1,nullptr,nullptr,16,17,22,18,&UpMotorControl);

// ESP32Encoder encoder;
// ESP32Encoder encoder2;
// chassis_config config={
//     .UpMotorControl = DownMotorControl,
//     .DownMotorControl = UpMotorControl,
//     .SpeedPID_ = nullptr,
//     .PositionPID_ = nullptr,
//     .PWMAPins = {
//       16,21,25,18
//     },
//     .PWMBPins = {
//       17,22,23,24
//     },
//     .EncoderAPins = {
//       12,13,14,4
//     },
//     .EncoderBPins = {
//       2,4,19,26
//     }

//   };

// chassis main_chassis(&config);


void setup(){
	Serial.begin(115200);
	ESP32Encoder::useInternalWeakPullResistors=UP;
  

  // mqtt_init();
}

void loop(){
  static int speed = 100;
  // DL.runSpeed(speed);
  // DL.handle();
  // DR.runSpeed(speed);
  // DR.handle();

  UL.runSpeed(speed);
  UL.handle();
  UR.runSpeed(speed);
  UR.handle();
  // Serial.println("SpeedX: " + String(speedx));
  // Serial.println("SpeedY: " + String(speedy));
  // main_chassis.runSpeed(100,100,0);
  delay(200);
}
