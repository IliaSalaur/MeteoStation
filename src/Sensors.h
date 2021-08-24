#pragma once
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHT.h>
#include <MHZ19PWM.h>
#include <MillisTimer.h>
#include <Average.h>

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

class CO2_Sensor : protected ISensor
{
private:
    MHZ19PWM * _co2_sensor;

public:
    CO2_Sensor(MHZ19PWM * co2);
    bool begin() override;
    float getData() override;
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