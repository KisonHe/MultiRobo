#ifndef CHASSIS_WARPPER_H
#define CHASSIS_WARPPER_H
#include "dc_motor_warpper.h"

typedef struct {
    ESP32MotorControl *UpMotorControl;
    ESP32MotorControl *DownMotorControl;
    pid* SpeedPID_;
    pid* PositionPID_;
    uint8_t PWMAPins[4];
    uint8_t PWMBPins[4];
    uint8_t EncoderAPins[4];
    uint8_t EncoderBPins[4];
}chassis_config;


class chassis
{
private:
    dc_motor* motors[4];

    /* data */
public:
    chassis(const chassis_config* config);
    void runSpeed(int16_t Vx, int16_t Vy, int16_t VOmega);
    // void runPosition(int32_t x, int32_t y, int16_t omega);
    ~chassis();
};



#endif