#ifndef DRAWER_H
#define DRAWER_H

#include <Arduino.h>
#include <Bitmap.h>
#include <TimeManager.h>

#include <GxEPD.h>

#include <GxGDE0213B1/GxGDE0213B1.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBold18pt7b.h>

enum ChartMode
{
    TEMP,
    HUDM
};

enum Page
{
    CLEAR,
    FRAME,
    TEMPCHART,
    HUDMCHART
};


class Drawer
{
private:
    GxEPD_Class * _display;
    Bitmaps * _bitmaps;
    Page page;  

    

public:
    Drawer(GxEPD_Class * display, Bitmaps * bitmap);
    void drawAll(byte batteryP, bool buzzerState, String date, int co2, TimeStruct * time, float temp, int hudm, bool updt);
    void drawBattery(byte batteryP, bool updt);
    void drawBuzzer(bool buzzerState, bool updt);
    void drawChart(ChartMode chartMode, float tempAvg[24]);
    void drawUpperText(String date, bool updt);
    void drawMiddleText(int co2,  TimeStruct * time, bool updt);
    void drawDownerText(float temp, int hudm, bool updt);
    void _drawChartData(byte initial_x, byte initial_y, byte k_x, byte k_y, float data[24]);
    void _update(int x, int y, int width, int height);
    void _update();
    ~Drawer();
};

#endif