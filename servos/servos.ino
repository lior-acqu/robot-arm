#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVO_FREQ 50 

struct ServoLimits {
  int minPulse;
  int maxPulse;
};

ServoLimits config[] = {
  {150, 600}, // Motor 0: MG996R (Base)
  {150, 600}, // Motor 1: MG996R (Shoulder)
  {140, 580}, // Motor 2: MG90S  (Elbow)
  {140, 580}, // Motor 3: MG90S  (Wrist)
  {130, 550}  // Motor 4: SG90   (Gripper)
};

// Array to track the current angle of each motor (Starting at 90 degrees)
int currentAngles[] = {90, 90, 90, 90, 90}; 

// Change how intensely the motors move
int intensity = 4;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  
  // Move all motors to initial home position (90 degrees)
  for(int i = 0; i < 5; i++) {
    moveMotor(i, currentAngles[i]);
    delay(1000);
  }
}

void loop() {
  if (Serial.available() > 0) {
    char incomingKey = Serial.read();
    
    switch (incomingKey) {
      // Base (Motor 0)
      case 'Q': currentAngles[0] += intensity; break;
      case 'A': currentAngles[0] -= intensity; break;
      
      // Shoulder (Motor 1)
      case 'W': currentAngles[1] += intensity; break;
      case 'S': currentAngles[1] -= intensity; break;
      
      // Elbow (Motor intensity)
      case 'E': currentAngles[2] += intensity; break;
      case 'D': currentAngles[2] -= intensity; break;

      // Wrist (Motor 3)
      case 'R': currentAngles[3] += intensity; break;
      case 'F': currentAngles[3] -= intensity; break;

      // Gripper (Motor 4)
      case 'T': currentAngles[4] += intensity; break;
      case 'G': currentAngles[4] -= intensity; break;
    }
    
    // Apply changes (limiting them between 0 and 180)
    for(int i = 0; i < 5; i++) {
      currentAngles[i] = constrain(currentAngles[i], 0, 180);
      moveMotor(i, currentAngles[i]);
    }
  }
}

void moveMotor(int motorNum, int angle) {
  int pulse = map(angle, 0, 180, config[motorNum].minPulse, config[motorNum].maxPulse);
  pwm.setPWM(motorNum, 0, pulse);
}