#include <Arduino.h>
#include <Servo.h>

// ======= Pin configuration =======
const uint8_t PIN_SERVO_BASE     = 3;
const uint8_t PIN_SERVO_SHOULDER = 5;
const uint8_t PIN_SERVO_ELBOW    = 6;
const uint8_t PIN_SERVO_WRIST    = 9;
const uint8_t PIN_SERVO_GRIPPER  = 10;

const uint8_t PIN_JOY_X1 = A0;
const uint8_t PIN_JOY_Y1 = A1;
const uint8_t PIN_JOY_X2 = A2;
const uint8_t PIN_JOY_Y2 = A3;
const uint8_t PIN_GRIP_POT = A4;
const uint8_t PIN_GRIP_BTN = 7;

// ======= Joint limits and trims =======
struct Joint {
  uint8_t pin;
  int minAngle;   // mechanical min
  int maxAngle;   // mechanical max
  int centerTrim; // offset applied after mapping
  int current;    // current written angle
};

// Default conservative limits; tweak later
Joint base     = {PIN_SERVO_BASE,     10, 170,  0, 90};
Joint shoulder = {PIN_SERVO_SHOULDER, 15, 165,  0, 90};
Joint elbow    = {PIN_SERVO_ELBOW,    15, 165,  0, 90};
Joint wrist    = {PIN_SERVO_WRIST,    20, 160,  0, 90};
Joint gripper  = {PIN_SERVO_GRIPPER,  30, 110,  0, 90};

Servo sBase, sShoulder, sElbow, sWrist, sGripper;

void setup() {}
void loop() {}
