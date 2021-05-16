
/*****************************************
 * Library   : ESP32MotorControl - Library for dual motor driver carrier for Arduino.
 * Programmer: Joao Lopes
 * Comments  : This library is to use with dual motors ICs, as DRV8833, DRV8825 and L298.
 * 			   And uses the MCPWM for control motors
 * Versions  :
 * ------ 	---------- 		-------------------------
 * 0.1.0  	2018-12-26		First beta
 *****************************************/

/*
 * Source for ESP32MotorControl
 *
 * Copyright (C) 2018  Joao Lopes https://github.com/JoaoLopesF/ESP32MotorControl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * This file contains the code for ESP32MotorControl library.
 *
 */



////// Includes

#include "Arduino.h"

#include "ESP32MotorControl.h"

#include <stdio.h>

#include "esp_system.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

///// Defines

// debug

// #define debug(fmt, ...)
#define debug(fmt, ...) Serial.printf("%s: " fmt "\r\n", __func__, ##__VA_ARGS__)

///// Methods

/**
 * @brief 
 * 
 * @param unitID 
 * @param pair 
 * @param gpioIn1 
 * @param gpioIn2 
 */
void ESP32MotorControl::attachMotor(mcpwm_timer_t pair, uint8_t gpioIn1, uint8_t gpioIn2)
{
	switch (pair)
	{
	case MCPWM_TIMER_0:
		mcpwm_gpio_init(unitID, MCPWM0A, gpioIn1);
		mcpwm_gpio_init(unitID, MCPWM0B, gpioIn2);
		this->mMotorAttached[0] = true;
		break;
	case MCPWM_TIMER_1:
		mcpwm_gpio_init(unitID, MCPWM1A, gpioIn1);
		mcpwm_gpio_init(unitID, MCPWM1B, gpioIn2);
		this->mMotorAttached[1] = true;
		break;
	case MCPWM_TIMER_2:
		mcpwm_gpio_init(unitID, MCPWM2A, gpioIn1);
		mcpwm_gpio_init(unitID, MCPWM2B, gpioIn2);
		this->mMotorAttached[2] = true;
		break;
	
	default:
		debug("Esp32MotorControl:This is no good");
		break;
	}

    debug ("Configuring Initial Parameters of MCPWM...");

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;    //frequency,
    pwm_config.cmpr_a = 0;    		//duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    		//duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    ESP_ERROR_CHECK(mcpwm_init(unitID, pair, &pwm_config));

	debug ("MCPWM initialized");
}

void ESP32MotorControl::motorMove(uint8_t motor, int8_t speed){
	if (!isMotorValid(motor)) {
		debug("calling motorMove on not attached motor");
		return;
	}
	debug("calling move with %u speed %d", motor, speed);
	speed = speed > 100?100:speed;
	speed = speed < -100?-100:speed;
	mMotorSpeed[motor] = speed;

	if (speed == 100){
		// Motor full forward
		mcpwm_set_signal_low(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_B);
		mcpwm_set_signal_high(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_A);
	}
	else if (speed > 0){
		mcpwm_set_signal_low(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_B);
		mcpwm_set_duty(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_A, speed);
		mcpwm_set_duty_type(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state

	}
	else if (speed > -100){
		mcpwm_set_signal_low(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_A);
		mcpwm_set_duty(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_B, speed);
		mcpwm_set_duty_type(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_B, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
	}
	else if (speed == -100){
		mcpwm_set_signal_low(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_A);
		mcpwm_set_signal_high(unitID, (mcpwm_timer_t)motor, MCPWM_OPR_B);
	}
	else {
		motorsStop();
	}
}



void ESP32MotorControl::motorStop(uint8_t motor)
{
	if (!isMotorValid(motor)) {
		return;
	}

	// Motor stop

	if (motor == 0) {

		mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
		mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);

	} else {

		mcpwm_set_signal_low(MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_A);
		mcpwm_set_signal_low(MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_B);

	}

	mMotorSpeed[motor] = 0; // Save it

	debug("Motor %u stop", motor);
}

// Motors stop

void ESP32MotorControl::motorsStop()
{
	motorStop(0);
	motorStop(1);
	motorStop(2);
	debug("Motors stop");
}

// Get motor speed

int8_t ESP32MotorControl::getMotorSpeed(uint8_t motor) {

	if (!isMotorValid(motor)) {
		return false;
	}
	return mMotorSpeed[motor];

}

// Is motor in forward ?

boolean ESP32MotorControl::isMotorForward(uint8_t motor) {
	
	if (!isMotorValid(motor)) {
		return false;
	}

	if (isMotorStopped(motor)) {
		return false;
	} else {
		return (mMotorSpeed[motor] > 0);
	}
}

// Is motor stopped ?

boolean ESP32MotorControl::isMotorStopped(uint8_t motor) {
	

	if (!isMotorValid(motor)) {
		return true;
	}
	return (mMotorSpeed[motor] == 0);
}

//// Privates

// Is motor valid ?

boolean ESP32MotorControl::isMotorValid(uint8_t motor) {


	if (motor > 1) {
		return false;
	}

	return mMotorAttached[motor];
}

ESP32MotorControl::ESP32MotorControl(mcpwm_unit_t unitID):unitID(unitID){

}
mcpwm_unit_t ESP32MotorControl::getUnitID(){
	return unitID;
}
///// End
