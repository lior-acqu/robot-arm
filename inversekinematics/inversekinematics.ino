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
int currentAngles[] = {90, 90, 90, 90, 90}; 

// Change how intensely the motors move
int intensity = 4;

// measurements of arm
float l_under = 5;
float l_upper = 8;
float l_hand = 10;
float height = 10;

// Over time, the angles of the specific parts might slightly shift. To make sure that the inverse kinematics is still working, you can enter the approx. corrections of the angles below (in deg). 
// e.g. if shoulder_corr is 10, all calculated angles will be increased by 10 degrees.
float shoulder_corr = 10;
float elbow_corr = 0;
float wrist_corr = 0;

void inverseKin(int rad, int angle) {

  // move base according to angle
  int base[] = {0};
  moveMotorsSmooth(base, 1, angle, 1000);

  // how much difference there is between the wrist and the base in height
  float height_diff = l_hand - height;

  // calculating the distance betweeen wrist and shoulder to find the angle of the shoulder
  float dist = sqrt(rad*rad + height_diff*height_diff);

  //calculating elbow angle
  float elbow_value = (dist - l_upper)/l_under;
  float result_rad = asin(elbow_value); // Returns radians
  float elbow_deg = result_rad * 180.0 / 3.14159265; // Convert to degrees
  elbow_deg += (90 + elbow_corr);

  //setting elbow angle
  int elbow[] = {2};
  moveMotorsSmooth(elbow, 1, elbow_deg, 1000);

  //calculating shoulder angle
  result_rad = atan((l_under*sin(result_rad) + height_diff)/rad); // Returns radians
  float shoulder_deg = result_rad * 180.0 / 3.14159265; // Convert to degrees
  if (shoulder_deg < 0) shoulder_deg = 0;
  shoulder_deg += shoulder_corr;

  // calculating wrist angle
  float wrist_deg = elbow_deg - (90 - shoulder_deg);
  if (wrist_deg > 180) wrist_deg = 179;
  wrist_deg += wrist_corr;

   //setting wrist angle
  int wrist[] = {3};
  moveMotorsSmooth(wrist, 1, wrist_deg, 1000);

  //setting shoulder angle
  int shoulder[] = {1};
  moveMotorsSmooth(shoulder, 1, shoulder_deg, 1000);

}




// Moves multiple motors simultaneously from their current angles to their targets over duration in ms
void moveMotorsSmooth(int motorNums[], int numMotors, int targetAngle, int duration) {

  int startAngles[numMotors];
  for (int i = 0; i < numMotors; i++) {
    startAngles[i] = currentAngles[motorNums[i]];
  }

  const int stepDelay = 20; // ms per step
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
  moveMotorsSmooth(m3, 1, 90, 500);
  int m4[] = {4};
  moveMotorsSmooth(m4, 1, 90, 500);
}

void home() {
  int m1[] = {1};
  moveMotorsSmooth(m1, 1, 90, 1500);
  int m2[] = {2};
  moveMotorsSmooth(m2, 1, 90, 1000);
  int m3[] = {3};
  moveMotorsSmooth(m3, 1, 45, 1000);
  int m0[] = {0};
  moveMotorsSmooth(m0, 1, 90, 1500);
}


void grip() {
  int m4[] = {4};
  moveMotorsSmooth(m4, 1, 175, 500);
}

void release() {
  int m4[] = {4};
  moveMotorsSmooth(m4, 1, 30, 500);
}


void loop() { 
  // inverse kinematics of gripping motion
  // go to pos A
  inverseKin(10,45);
  delay(200);
  // grip object
  grip();
  delay(200);
  // lift it up
  home();
  delay(200);
  // put it down
  inverseKin(10,135);
  delay(200);
  // release it
  release();
  delay(200);
  // go home
  home();
  delay(200);
  // go to pos B
  inverseKin(10,135);
  delay(200);
  // grip object
  grip();
  delay(200);
  // lift it up
  home();
  delay(200);
  // put it down
  inverseKin(10,45);
  delay(200);
  // release it
  release();
  delay(200);
    // go home
  home();
  delay(200);
}


void moveMotor(int motorNum, int angle) {
  int pulse = map(angle, 0, 180, config[motorNum].minPulse, config[motorNum].maxPulse);
  pwm.setPWM(motorNum, 0, pulse);
}