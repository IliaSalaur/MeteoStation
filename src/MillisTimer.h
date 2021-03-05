#ifndef MILLIS_TIMER_H
#define MILLIST_IMER_H
#include <Arduino.h>

class MillisTimer
{
private:
    uint32_t _timer = 0;
    uint32_t _period = 0;
    bool _stopIfReady = 0;
public:
    MillisTimer();
    void start(uint32_t period, bool stopIfReady);
    void start(int period, bool stopIfReady);
    bool isReady();
    ~MillisTimer(); 
};



#endif