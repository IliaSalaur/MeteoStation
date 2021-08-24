#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer
{
private:
    int _pin;
    bool _alwaysChangePinMode = 0;
    bool _state = 0;
public:
    Buzzer(int pin, bool acpm = 0)
    {
        _pin = pin;
        _alwaysChangePinMode = acpm;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    
    void setState(bool state)
    {
        if(_alwaysChangePinMode) pinMode(_pin, OUTPUT);
        digitalWrite(_pin, state);
        _state = state;
    }

    bool getState()
    {
        return _state;
    }
};

