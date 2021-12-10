#pragma once

typedef unsigned char PIN;

class compressorControl
{
public:
unsigned int readspeed();
bool setSpeed(const float& inputSpeed);
bool setPins(const PIN& controlPin, const PIN& pulsePin);
private:
PIN MOTOR_CONTROL_PIN = 0;
PIN MOTOR_PULSE_PIN = 0;
bool pinSet = false;
};