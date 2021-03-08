#include <MillisTimer.h>


void MillisTimer::start(uint32_t period, bool stopIfReady = 1)
{
    _timer = millis();
    _period = period;
    _stopIfReady = stopIfReady;
}

void MillisTimer::start(int period, bool stopIfReady = 1)
{
    _timer = millis();
    _period = period;
    _stopIfReady = stopIfReady;
    _workState = 1;
}

bool MillisTimer::isReady()
{
    if(_workState == 1 && millis() - _timer >= _period)
    {
        if(millis() - _timer >= _period+2)
        {
            _workState = 0;
            if(_stopIfReady == 0)
            {
                this->start(_period, 0);
            }   
        }
        
        return 1;
    }
    else return 0;
}


MillisTimer::MillisTimer() {}
MillisTimer::~MillisTimer() {}