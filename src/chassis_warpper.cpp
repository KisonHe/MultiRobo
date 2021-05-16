#include "chassis_warpper.h"

// extern ESP32MotorControl DwonMotorControl;
chassis::chassis(const chassis_config* config) {
    for (int i = 0; i < 4; i++) {
        //Up
        //Left  0         1
        //
        //
        //      2         3
        motors[i] = new dc_motor(i>1?MCPWM_UNIT_1:MCPWM_UNIT_0,i % 2 ? MCPWM_TIMER_1 : MCPWM_TIMER_0,config->SpeedPID_,config->PositionPID_,config->EncoderAPins[i],\
        config->EncoderBPins[i],config->PWMAPins[i],config->PWMBPins[i],i>1?config->UpMotorControl:config->DownMotorControl);
    }
}

chassis::~chassis() {
    for (int i = 0; i < 4; i++) {
        delete motors[i];
    }
    
}

void chassis::runSpeed(int16_t Vx, int16_t Vy, int16_t VOmega){
    int32_t rawSpeed[4];
    int32_t Speed[4];
    int32_t MaxSpeed = 0;
    float proportion = 0;
    rawSpeed[0] = (Vx + Vy + VOmega);
	rawSpeed[1] = (Vx - Vy - VOmega);
	rawSpeed[2] = (Vx - Vy + VOmega);
	rawSpeed[3] = (Vx + Vy - VOmega);

    //求设定值中的最大值
	for(int8_t index=0, MaxSpeed=0; index<4; index++)
	{
		if((rawSpeed[index]>0 ? rawSpeed[index] : -rawSpeed[index]) > MaxSpeed)
		{
			MaxSpeed = (rawSpeed[index]>0 ? rawSpeed[index] : -rawSpeed[index]);
		}
	}
	//若速度设定值超过底盘允许最大速度，则等比减小速度设定值
	if(100 < MaxSpeed)
	{
		proportion = 100 / MaxSpeed;
		Speed[0] = rawSpeed[0] * proportion;
		Speed[1] = -rawSpeed[1] * proportion;
		Speed[2] = rawSpeed[2] * proportion;
		Speed[3] = -rawSpeed[3] * proportion; 
	}
	else
	{
		Speed[0] =  rawSpeed[0];
		Speed[1] =  -rawSpeed[1];
		Speed[2] =  rawSpeed[2];
		Speed[3] =  -rawSpeed[3];
	}
	for(uint8_t i=0; i<4; i++)
	{
		motors[i]->runSpeed(Speed[i]);
	}

}
