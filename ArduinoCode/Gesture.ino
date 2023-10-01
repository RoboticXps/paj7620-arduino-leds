#include <Wire.h>
#include "PAJ7620U2.h"

unsigned short Gesture_Data;
const int leftLedPin = 2;
const int rightLedPin = 3;
const int upLedPin = 4;
const int downLedPin = 5;
const int rotationTime = 150;
int times = 0;

void setup()
{
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  pinMode(upLedPin, OUTPUT);
  pinMode(downLedPin, OUTPUT);
  uint8_t i;
  Serial.begin(115200);
  delayMicroseconds(800);
  Wire.begin();
  delayMicroseconds(10);
  while (!PAJ7620U2_init())
  {
    Serial.print("\nError\n");
    delay(500);
  }
  Serial.print("\nOK\n");
  I2C_writeByte(PAJ_BANK_SELECT, 0); // Select Bank 0
  for (i = 0; i < Gesture_Array_SIZE; i++)
  {
    I2C_writeByte(Init_Gesture_Array[i][0], Init_Gesture_Array[i][1]); // Gesture register initializes
  }
}

void loop()
{
  Gesture_Data = I2C_readU16(PAJ_INT_FLAG1);
  if (Gesture_Data)
  {
    Serial.print(getGestureName(Gesture_Data) + "\n");
    setLedState(Gesture_Data);
    Gesture_Data = 0;
  }
}

String getGestureName(uint16_t gesture)
{
  switch (gesture)
  {
  case PAJ_UP:
    return "Up";
  case PAJ_DOWN:
    return "Down";
  case PAJ_LEFT:
    return "Left";
  case PAJ_RIGHT:
    return "Right";
  case PAJ_CLOCKWISE:
    return "CW";
  case PAJ_COUNT_CLOCKWISE:
    return "CCW";
  default:
    return "";
  }
}

void setLedState(uint16_t gesture)
{
  digitalWrite(upLedPin, gesture == PAJ_UP);
  digitalWrite(leftLedPin, gesture == PAJ_LEFT);
  digitalWrite(rightLedPin, gesture == PAJ_RIGHT);
  digitalWrite(downLedPin, gesture == PAJ_DOWN);
  if (gesture == PAJ_CLOCKWISE)
  {
    times = 0;
    while (times < 3)
    {
      Serial.print("CW");
      digitalWrite(leftLedPin, HIGH);
      delay(rotationTime);
      digitalWrite(leftLedPin, LOW);
      digitalWrite(upLedPin, HIGH);
      delay(rotationTime);
      digitalWrite(upLedPin, LOW);
      digitalWrite(rightLedPin, HIGH);
      delay(rotationTime);
      digitalWrite(rightLedPin, LOW);
      digitalWrite(downLedPin, HIGH);
      delay(rotationTime);
      digitalWrite(downLedPin, LOW);
      delay(rotationTime);
      times = times + 1;
    }
  }
  if (gesture == PAJ_COUNT_CLOCKWISE)
  {
    Serial.print("CCW");
    times = 0;
    while (times < 3)
    {
      Serial.print("CW");
      digitalWrite(leftLedPin, HIGH);
      delay(rotationTime);
      digitalWrite(leftLedPin, LOW);
      digitalWrite(downLedPin, HIGH);
      delay(rotationTime);
      digitalWrite(downLedPin, LOW);
      digitalWrite(rightLedPin, HIGH);
      delay(rotationTime);
      digitalWrite(rightLedPin, LOW);
      digitalWrite(upLedPin, HIGH);
      delay(rotationTime);
      digitalWrite(upLedPin, LOW);
      delay(rotationTime);
      times = times + 1;
    }
  }
}
