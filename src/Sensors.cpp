#include <Sensors.h>
#define PERIOD 7050 // measuring will be done only if passed the timeout (called "period")
#define MEASURING_TIMEOUT 600

CO2_Sensor::CO2_Sensor(MHZ19PWM * co2, bool em = 0)
{
    _co2_sensor = co2;
    _co2_sensor->useLimit(5000);
    _em = em;
}

bool CO2_Sensor::begin()
{
    if(_em == 0)
    {
        float data = this->getData();
        if(data > 0 && data < 5000)
        {
            _freshData = data;
            _DEBUG = "CO2 Sensor Start succes , co2 level is ";
            _DEBUG += String(_freshData);
            return 1;
        }
        else
        {
            _DEBUG = "CO2 Sensor Start failed , check the wiring";
            return 0;
        } 
    }
    return 0;
}

float CO2_Sensor::getData()
{
    static uint32_t request_tmr = 0;
    static uint32_t tmr = 0;
    static bool measuring = 0;

    if(!measuring && millis() - tmr >= PERIOD-500)
    {
        if(!_em)_co2_sensor->requestData(); 
        request_tmr = millis();
        measuring = 1;
    }

    else if(measuring && millis() - request_tmr >= MEASURING_TIMEOUT)
    {
        if(_em || _co2_sensor->isDataReady())
        {
            float data = (_em) ? float(random(0, 2000)) : _co2_sensor->getCO2();
            if(data > 0 && data <= 5000)
            {
                measuring = 0;
                _freshData = data;
                if(_freshData >= _maxLimit)
                {
                    if(_cb) _cb();
                }
            }
        }
    }
    return _freshData;
}

void CO2_Sensor::onMaxLimit(void(*cb)(), float maxLimit)
{
    _maxLimit = maxLimit;
    _cb = cb;
}

CO2_Sensor::~CO2_Sensor() {}

Temp_Sensor::Temp_Sensor(DHT * dht)
{
    _temp_sensor = dht;
}

bool Temp_Sensor::begin()
{
    float data = this->getData();
    if(isnan(data))
    {
        _DEBUG = "Temp Sensor Start failed , check the wiring";
        return 0;
    }
    else 
    {
        _freshData = data;
        _DEBUG = "Temp Sensor Start succes , temp level is ";
        _DEBUG += String(_freshData);
        return 1;
    }
}

float Temp_Sensor::getData()
{
    static uint32_t tmr = 0;
    if(millis() - tmr >= PERIOD)
    {
        tmr = millis();
        float data = _temp_sensor->readTemperature();
        if(!isnan(data))
        {
            _freshData = data;
        }
    }
    else return _freshData;
}

Temp_Sensor::~Temp_Sensor(){}

Hudm_Sensor::Hudm_Sensor(DHT * dht)
{
    _hudm_sensor = dht;
}

bool Hudm_Sensor::begin()
{
    float data = this->getData();
    if(isnan(data))
    {
        _DEBUG = "Hudm Sensor Start failed , check the wiring";
        return 0;
    }
    else 
    {
        _freshData = data;
        _DEBUG = "Hudm Sensor Start succes , hudm level is ";
        _DEBUG += String(_freshData);
        return 1;
    }
}

float Hudm_Sensor::getData()
{
    static uint32_t tmr = 0;
    if(millis() - tmr >= PERIOD)
    {
        tmr = millis();
        float data = _hudm_sensor->readHumidity();
        if(!isnan(data))
        {
            _freshData = data;
        }
    }
    else return _freshData;
}

Hudm_Sensor::~Hudm_Sensor(){}

void ISensor::handleAverageData(int hour, Average* avg)
{
    if(timerStarted == 0){   dataTimer = millis();  timerStarted = 1;}

    if(timerStarted == 1 && millis() - dataTimer > 10000)
    {DEBUG("Handle average data")
        float _oldData = _freshData; 
        this->getData();
        if(_freshData > 0)
        {
            if(avg->a[hour] == -1.0F || _data.a[hour] == -1.0F)
            {
                _data.a[hour] = _freshData;
                avg->a[hour] = _freshData;
            }
            avg->a[hour] = (_freshData + _oldData) / 2;
            _data.a[hour] = avg->a[hour];
        }       
        timerStarted = 0; 
    }
}

String ISensor::getDebug()
{
    return _DEBUG;
}

ISensor::ISensor() {}
ISensor::~ISensor() {}

