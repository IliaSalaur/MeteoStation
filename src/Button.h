#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include <Arduino.h>

enum class ButtonStates
{
    CLICKED,
    HOLDED,
    RELEASED
};

class Button
{
private:
    void changePinMode()
    {
        if(alwaysChangePinMode) pinMode(pin, INPUT);
    }
    int pin = 0;
    bool alwaysChangePinMode = 0;

    int ReadBut()
    {
        static uint32_t Timer = 0;
        static int lastRead = 0;
        if(millis() - Timer > 10) 
        {
            Timer = millis();
            lastRead = digitalRead(pin);
        }
        else return lastRead;
    }
public:
    Button(int _pin, bool acpm = 0) 
    {
        pin = _pin;
        alwaysChangePinMode = acpm;
        pinMode(pin, INPUT);
    }

    ButtonStates getState()
    {
        static uint32_t butTimer = 0;
        static bool butTimerStart = 0;

        int s = this->ReadBut();

        if(s > 0 && butTimerStart == 0)
        {
            butTimer = millis();
            butTimerStart = 1;
            
            Serial.println("butTimer start");
            
        }
        else if(s < 1 && butTimerStart == 1)
        {
            butTimerStart = 0;
            int milHolded = millis() - butTimer;

            DEBUG(String("butTimer stop , milHolded: ") + String(milHolded));


            if(milHolded > 900) return ButtonStates::HOLDED;
            else if(milHolded > 80) return ButtonStates::CLICKED;
        }
        return ButtonStates::RELEASED;
    }
};

#endif