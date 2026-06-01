#include <Servo.h>

Servo thumbServo;
Servo indexServo;
Servo middleServo;
Servo ringServo;
Servo littleServo;

const int thumbFlexPin  = A0;
const int indexFlexPin  = A1;
const int middleFlexPin = A2;
const int ringFlexPin   = A3;
const int littleFlexPin = A4;

void setup()
{
  thumbServo.attach(0);
  indexServo.attach(1);
  middleServo.attach(2);
  littleServo.attach(3);
  ringServo.attach(4);
}

void loop()
{
  int thumbFlexValue = analogRead(thumbFlexPin);
  int thumbAngle = map(thumbFlexValue, 800, 900, 0, 180);
  thumbAngle = constrain(thumbAngle, 0, 180);
  thumbServo.write(thumbAngle);

  int indexFlexValue = analogRead(indexFlexPin);
  int indexAngle = map(indexFlexValue, 800, 900, 0, 180);
  indexAngle = constrain(indexAngle, 0, 180);
  indexServo.write(indexAngle);

  int middleFlexValue = analogRead(middleFlexPin);
  int middleAngle = map(middleFlexValue, 800, 900, 0, 180);
  middleAngle = constrain(middleAngle, 0, 180);
  middleServo.write(middleAngle);

  int littleFlexValue = analogRead(littleFlexPin);
  int littleAngle = map(littleFlexValue, 800, 900, 0, 180);
  littleAngle = constrain(littleAngle, 0, 180);
  littleServo.write(littleAngle);

  int ringFlexValue = analogRead(ringFlexPin);
  int ringAngle = map(ringFlexValue, 800, 900, 0, 180);
  ringAngle = constrain(ringAngle, 0, 180);
  ringServo.write(ringAngle);

  delay(100);
}