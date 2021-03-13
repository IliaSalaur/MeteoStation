#pragma once
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHT.h>
#include <MHZ19PWM.h>
#include <MillisTimer.h>
#include <AverageStructs.h>

class Sensor
{
protected:
    bool timerStarted = 0;
    AverageStr _data;
    float _lastData;
    String _DEBUG;
    uint32_t dataTimer = 0;
    //MillisTimer *dataTimer = new MillisTimer;

public:
    Sensor();
    virtual bool begin() = 0;
    virtual float getData() = 0;
    void handleAverageData(int hour, AverageStr * avg);
    String getDebug();
    ~Sensor();

};

class CO2_Sensor : protected Sensor
{
private:
    MHZ19PWM * _co2_sensor;

public:
    CO2_Sensor(MHZ19PWM * co2);
    bool begin() override;
    float getData() override;
    ~CO2_Sensor();
};

class Temp_Sensor : public Sensor
{
private:
    DHT * _temp_sensor;

public:
    Temp_Sensor(DHT * dht);
    bool begin() override;
    float getData() override;
    ~Temp_Sensor();
};

class Hudm_Sensor : public Sensor
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