#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>

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
int currentAngles[] = {90, 90, 90, 45, 90}; 

// Change how intensely the motors move
int intensity = 4;





// Moves multiple motors simultaneously from their current angles to their targets over `duration` ms
void moveMotorsSmooth(int motorNums[], int numMotors, int targetAngle, int duration) {

  int startAngles[numMotors];
  for (int i = 0; i < numMotors; i++) {
    startAngles[i] = currentAngles[motorNums[i]];
  }

  const int stepDelay = 20; // ms per step - lower = smoother, but more PWM writes
  int steps = duration / stepDelay;
  if (steps < 1) steps = 1;

  for (int step = 1; step <= steps; step++) {
    for (int i = 0; i < numMotors; i++) {
      int angle = startAngles[i] + (long)(targetAngle - startAngles[i]) * step / steps;
      moveMotor(motorNums[i], angle);
    }
    delay(stepDelay);
  }

  for (int i = 0; i < numMotors; i++) {
    currentAngles[motorNums[i]] = targetAngle;
  }
}




void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  int m0[] = {0};
  moveMotorsSmooth(m0, 1, 90, 500);
  int m1[] = {1};
  moveMotorsSmooth(m1, 1, 90, 500);
  int m2[] = {2};
  moveMotorsSmooth(m2, 1, 90, 500);
  int m3[] = {3};
  moveMotorsSmooth(m3, 1, 45, 500);
  int m4[] = {4};
  moveMotorsSmooth(m4, 1, 90, 500);
}





void loop() { 
  // decide between rock (0), paper (1), and scissors (2)
  int choice = random(3);
  // do the characteristic rock-paper-scissors motion
  int m2[] = {2};
  moveMotorsSmooth(m2, 1, 70, 100);
  delay(500);
  moveMotorsSmooth(m2, 1, 90, 100);
  delay(500);
  moveMotorsSmooth(m2, 1, 70, 100);
  delay(500);
  moveMotorsSmooth(m2, 1, 90, 100);
  delay(500);
  moveMotorsSmooth(m2, 1, 80, 50);
  int m4[] = {4};
  // make move based on randomly selected number
  if (choice == 0) {
    moveMotorsSmooth(m4, 1, 170, 50);
  } else if (choice == 1) {
    moveMotorsSmooth(m4, 1, 30, 50);
  } else {
    moveMotorsSmooth(m4, 1, 40, 100);
    delay(200);
    moveMotorsSmooth(m4, 1, 120, 300);
    delay(200);
    moveMotorsSmooth(m4, 1, 40, 300);
    delay(200);
    moveMotorsSmooth(m4, 1, 120, 300);
    delay(200);
    moveMotorsSmooth(m4, 1, 40, 300);
    delay(200);
    moveMotorsSmooth(m4, 1, 120, 300);
  }
  delay(1000);
  // get ready for another round
  moveMotorsSmooth(m2, 1, 150, 1000);
  moveMotorsSmooth(m4, 1, 120, 100);
  delay(500);
}


void moveMotor(int motorNum, int angle) {
  int pulse = map(angle, 0, 180, config[motorNum].minPulse, config[motorNum].maxPulse);
  pwm.setPWM(motorNum, 0, pulse);
}