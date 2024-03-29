#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define SQUAREWAVE // uncomment if speed impulse is too large
#include "motorcontrol.h"

#define DATA4 D0
#define DATA5 D1
#define DATA6 D2
#define DATA7 D3
#define REGISTER_SELECT D4
#define ENABLE D5

#define ENCODER_CLK A0
#define ENCODER_DATA D7

#define MOTOR_CONTROL A0
#define MOTOR_SPEED D9 /* TODO: find available pin to read motor encoder pulses */

#define BUTTON D8

LiquidCrystal display(REGISTER_SELECT, ENABLE, DATA4, DATA5, DATA6, DATA7);

int bigCount = 0;
int smallCount = 0;
int currentStateClock;
int previousStateClock;
int currentButtonState;
int previousButtonState;

compressorControl motor;

String encoderDirection = "";

void clearLine(int line)
{
  display.setCursor(0, line);
  display.print("                ");
}

void setup()
{
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DATA, INPUT);
  pinMode(BUTTON, INPUT);
  // attachInterrupt(0, checkEncoder, CHANGE);

  previousStateClock = digitalRead(ENCODER_CLK);
  previousButtonState = digitalRead(BUTTON);

  Serial.begin(115200);
  Serial.print("\nSerial initiated.\n");

  display.begin(16, 2);
  display.setCursor(0, 0);
  display.print("Use encoder.");
  display.setCursor(0, 1);
  display.print("Set speed.");

  // motor control setup
  motor.setPins(MOTOR_CONTROL, MOTOR_SPEED);
}

void loop()
{

  currentButtonState = digitalRead(BUTTON);
  if (currentButtonState == LOW && previousButtonState == HIGH)
  {
    Serial.print("HERE");
    smallCount = 0;
    bigCount = 0;
    clearLine(1);
    display.setCursor(0, 1);
    display.print("Val: ");
    display.setCursor(5, 1);

    String valueString(bigCount);
    valueString += '.';
    valueString += smallCount;
    valueString += " %";
    display.print(valueString);
  }

  currentStateClock = digitalRead(ENCODER_CLK);
  if (currentStateClock != previousStateClock)
  {
    if (digitalRead(ENCODER_DATA) != currentStateClock)
    {
      smallCount--;
      encoderDirection = "Counter Clockwise";
    }
    else
    {
      smallCount++;
      encoderDirection = "Clockwise";
    }
    clearLine(1);
    display.setCursor(0, 1);
    display.print("Val: ");
    display.setCursor(5, 1);
    String valueString(bigCount);
    valueString += '.';
    valueString += smallCount;
    valueString += " %";
    display.print(valueString);
  }

  if (smallCount < 0)
  {
    if (bigCount == 0)
    {
      smallCount = 0;
    }
    else
    {
      bigCount--;
      smallCount = 9;
    }
  }
  else if (smallCount > 9)
  {
    smallCount = 0;
    bigCount++;
  }

  if (bigCount == 100 && smallCount > 0)
  {
    smallCount = 0;
  }

  motor.setSpeed((float)bigCount + ((float)smallCount / 10) / 100);

  previousStateClock = currentStateClock;
  previousButtonState = currentButtonState;

  delay(1);
}
