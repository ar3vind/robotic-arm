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
  int minAngle;
  int maxAngle;
  int centerTrim;
  int current;
};

Joint base     = {PIN_SERVO_BASE,     10, 170,  0, 90};
Joint shoulder = {PIN_SERVO_SHOULDER, 15, 165,  0, 90};
Joint elbow    = {PIN_SERVO_ELBOW,    15, 165,  0, 90};
Joint wrist    = {PIN_SERVO_WRIST,    20, 160,  0, 90};
Joint gripper  = {PIN_SERVO_GRIPPER,  30, 110,  0, 90};

Servo sBase, sShoulder, sElbow, sWrist, sGripper;

// ======= Control behaviour =======
const int   DEAD_BAND = 8;    // degrees
const float ALPHA     = 0.25; // 0..1 low-pass filter factor
bool        USE_GRIP_POT = true;

int mapAnalogToAngle(int analogValue, const Joint &j) {
  long a = map(analogValue, 0, 1023, j.minAngle, j.maxAngle);
  a += j.centerTrim;
  if (a < j.minAngle) a = j.minAngle;
  if (a > j.maxAngle) a = j.maxAngle;
  return (int)a;
}

int applyDeadbandAndFilter(int target, int current) {
  if (abs(target - current) < DEAD_BAND) return current;
  return (int)(ALPHA * target + (1.0 - ALPHA) * current);
}

void attachAll() {
  sBase.attach(base.pin);
  sShoulder.attach(shoulder.pin);
  sElbow.attach(elbow.pin);
  sWrist.attach(wrist.pin);
  sGripper.attach(gripper.pin);
}

void writeAll() {
  sBase.write(base.current);
  sShoulder.write(shoulder.current);
  sElbow.write(elbow.current);
  sWrist.write(wrist.current);
  sGripper.write(gripper.current);
}

void centerAll() {
  base.current     = constrain(90 + base.centerTrim,     base.minAngle,     base.maxAngle);
  shoulder.current = constrain(90 + shoulder.centerTrim, shoulder.minAngle, shoulder.maxAngle);
  elbow.current    = constrain(90 + elbow.centerTrim,    elbow.minAngle,    elbow.maxAngle);
  wrist.current    = constrain(90 + wrist.centerTrim,    wrist.minAngle,    wrist.maxAngle);
  gripper.current  = constrain(90 + gripper.centerTrim,  gripper.minAngle,  gripper.maxAngle);
  writeAll();
}

void setup() {
  pinMode(PIN_GRIP_BTN, INPUT_PULLUP);
  attachAll();
  centerAll();
  delay(500);
}


void loop() {
  // Read base joystick (X1)
  int ax0 = analogRead(PIN_JOY_X1);

  // Map to angle and smooth
  int tBase = mapAnalogToAngle(ax0, base);
  base.current = applyDeadbandAndFilter(tBase, base.current);

  // Write outputs
  writeAll();

  delay(15);
}