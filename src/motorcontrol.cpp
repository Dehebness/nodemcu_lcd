#include "motorcontrol.h"
#include <Arduino.h>

#ifdef PULSELOW
const auto pulseRead = LOW;
#else
const auto pulseRead = HIGH;
#endif

unsigned long cycleTimer(const unsigned int pulseTime)
{
#ifdef SQUAREWAVE
    unsigned int cycleTime = 0;
    cycleTime = pulseTime * 2;
    return cycleTime;
#else
    return pulseTime;
#endif
}

unsigned int compressorControl::readspeed()
{
    if (this->pinSet)
    {
        return 0;
    }
    unsigned long pulseTime = pulseIn(this->MOTOR_PULSE_PIN, pulseRead, 100);
    if (pulseTime == 0) // if pulseIn returns 0 it means the MCU gave up on finding a pulse
    {
        return 0;
    }
    unsigned long cycleTime = cycleTimer(pulseTime);
    unsigned int speed = (1.0f / ((double)cycleTime / 1000)) * 60;
    return speed;
}

bool compressorControl::setSpeed(const unsigned int &inputSpeed)
{
    if (this->pinSet)
    {
        return false;
    }
    float dutyCycle = (float)inputSpeed / 35000; // Assumed 35,000 rpm at 100% duty cycle
    unsigned char output = 255 * dutyCycle;
    analogWrite(this->MOTOR_CONTROL_PIN, output);
    return true;
}

bool compressorControl::setPins(const PIN &controlPin, const PIN &pulsePin)
{
    this->MOTOR_CONTROL_PIN = controlPin;
    this->MOTOR_PULSE_PIN = pulsePin;
    this->pinSet = true;
    return this->pinSet;
}
