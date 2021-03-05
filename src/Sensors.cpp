#include <Sensors.h>

CO2_Sensor::CO2_Sensor(MHZ19PWM * co2)
{
    _co2_sensor = co2;
    _co2_sensor->useLimit(5000);
}

bool CO2_Sensor::begin()
{
    float data = this->getData();
    if(data > 0 && data < 5000)
    {
        _lastData = data;
        _DEBUG = "CO2 Sensor Start succes , co2 level is ";
        _DEBUG += String(_lastData);
        return 1;
    }
    else
    {
        _DEBUG = "CO2 Sensor Start failed , check the wiring";
        return 0;
    } 
}

float CO2_Sensor::getData()
{
    MillisTimer timer;
    _co2_sensor->requestData(); 
    timer.start(500, 1);
    while(_co2_sensor->isDataReady() == 0)   
    {
       if(timer.isReady())  break;
    }

    float data = _co2_sensor->getCO2();

    if(data > 0 && data <= 5000)
    {
        return data;
    }
    else return 0;
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
        _lastData = data;
        _DEBUG = "Temp Sensor Start succes , temp level is ";
        _DEBUG += String(_lastData);
        return 1;
    }
}

float Temp_Sensor::getData()
{
    float data;
    data = _temp_sensor->readTemperature();
    if(isnan(data))
    {
        return 0;
    }
    else return data;
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
        _lastData = data;
        _DEBUG = "Hudm Sensor Start succes , hudm level is ";
        _DEBUG += String(_lastData);
        return 1;
    }
}

float Hudm_Sensor::getData()
{
    float data;
    data = _hudm_sensor->readHumidity();
    if(isnan(data))
    {
        return 0;
    }
    else return data;
}

Hudm_Sensor::~Hudm_Sensor(){}

void Sensor::handleAverageData(int hour, AverageStr * avg)
{
    if(timerStarted == 0)   dataTimer->start(10 * 1000, 0);  timerStarted = 1;
    if(dataTimer->isReady())
    {
        float data = this->getData();
        if(data > 0)
        {
            if(avg->hour[hour] == 0 || _data.hour[hour] == 0)
            {
                _data.hour[hour] = _lastData;
                avg->hour[hour] = _lastData;
            }
            avg->hour[hour] = (_lastData + data) / 2;
            _data.hour[hour] = avg->hour[hour];
            _lastData = data;
        }        
    }
}

String Sensor::getDebug()
{
    return _DEBUG;
}

Sensor::Sensor() {}
Sensor::~Sensor() {}

