#pragma once
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHT.h>
#include <MHZ19PWM.h>
#include <MillisTimer.h>
#include <Average.h>
#include <Config.h>

class ISensor
{
protected:
    bool timerStarted = 0;
    Average _data;
    float _freshData;
    String _DEBUG;
    uint32_t dataTimer = 0;
    //MillisTimer *dataTimer = new MillisTimer;

public:
    ISensor();
    virtual bool begin() = 0;
    virtual float getData() = 0;
    void handleAverageData(int hour, Average* avg);
    String getDebug();
    ~ISensor();

};

class CO2_Sensor : public ISensor
{
private:
    MHZ19PWM * _co2_sensor;
    bool _em = 0;
    void(*_cb)();
    float _maxLimit = CO2_MAX_LIMIT;

public:
    CO2_Sensor(MHZ19PWM * co2, bool);
    bool begin() override;
    float getData() override;
    void onMaxLimit(void(*cb)(), float maxLimit);
    ~CO2_Sensor();
};

class Temp_Sensor : public ISensor
{
private:
    DHT * _temp_sensor;

public:
    Temp_Sensor(DHT * dht);
    bool begin() override;
    float getData() override;
    ~Temp_Sensor();
};

class Hudm_Sensor : public ISensor
{
private:
    DHT * _hudm_sensor;

public:
    Hudm_Sensor(DHT * dht);
    bool begin() override;
    float getData() override;
    ~Hudm_Sensor();
};

#endif