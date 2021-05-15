#include <Arduino.h>
// #include <ESP32Encoder.h>
// #include "ESP32MotorControl.h"
#include "dc_motor_warpper.h"

ESP32MotorControl DwonMotorControl = ESP32MotorControl();
//Down left
dc_motor DL(0,nullptr,nullptr,25,26,16,17,DwonMotorControl);

// ESP32Encoder encoder;
// ESP32Encoder encoder2;

// timer and flag for example, not needed for encoders
unsigned long encoder2lastToggled;
bool encoder2Paused = false;

void setup(){
	
	Serial.begin(115200);
	ESP32Encoder::useInternalWeakPullResistors=UP;

	// encoder.attachHalfQuad(25, 26);
	// encoder2.attachHalfQuad(4, 5);
  // MotorControl.attachMotor(16, 17);
  // MotorControl.attachMotor(14, 15);
		
	// set starting count value after attaching
	// encoder.setCount(37);

	// clear the encoder's raw count and set the tracked count to zero
	// encoder2.clearCount();
	// Serial.println("Encoder Start = " + String((int32_t)encoder.getCount()));
	// set the lastToggle
	// encoder2lastToggled = millis();
}

void loop(){
  static int speed = 100;
  // speed += 2;
  // if (speed > 100){
  //   speed = 80;
  // }
  DL.runSpeed(speed);
  DL.handle();
  // int32_t var = (int32_t)((int32_t)(-83108 - -82598) * 100) / (82840 - 82639);
  // // int32_t var = (int32_t)((int32_t)(-83108 - (-82598)) * 100);
  // Serial.println(var);
  // Serial.println("speedRaw = " + String(var));
  // MotorControl.motorForward(0, speed);
  // MotorControl.motorForward(1, speed);
	// Loop and read the count
  delay(200);
	// Serial.println("speed = " + String(DL.getSpeed()) + "\n Pos = " + String(DL.getRealPosition()));
	

	// every 5 seconds toggle encoder 2
	// if (millis() - encoder2lastToggled >= 5000) {
	// 	if(encoder2Paused) {
	// 		Serial.println("Resuming Encoder 2");
	// 		encoder2.resumeCount();
	// 	} else {
	// 		Serial.println("Paused Encoder 2");
	// 		encoder2.pauseCount();
	// 	}

	// 	encoder2Paused = !encoder2Paused;
	// 	encoder2lastToggled = millis();
	// }
}
