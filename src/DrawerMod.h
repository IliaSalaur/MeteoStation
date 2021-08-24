#ifndef DRAWER_MOD_H
#define DRAWER_MOD_H

#include <Arduino.h>
#include <Config.h>
#include <DrawContext.h>

#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#ifdef DISPLAY213
#include <GxGDE0213B1/GxGDE0213B1.h>
#elif defined(DISPLAY290)
#include <GxGDEH029A1/GxGDEH029A1.h> //not right
#endif


#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBold18pt7b.h>

enum ChartMode
{
    TEMP,
    HUDM,
    CO2
};

enum Page
{
    CLEAR,
    FRAME,
    TEMPCHART,
    HUDMCHART,
    CO2CHART
};


class DrawerMod
{
private:
    GxEPD_Class * _display;
    String _lastName;

    vector<DrawContext> _pages;

    void _drawRect(int, int, int, int, bool, uint32_t);
    void _drawLine(int, int, int, int, uint32_t);
    void _drawText(int, int, int, int, String, GFXfont*, uint32_t);
    void _drawBitmap(int, int, int, int, const unsigned char*, uint32_t);
    void _drawChart(uint8_t initial_x, uint8_t initial_y, uint8_t k_x, uint8_t k_y, Average data);
    void _drawChart(ElChart*);
    void _drawEl(IElement*);
    void _update(int x, int y, int width, int height);
    void _update();
    
public:
    DrawerMod(GxEPD_Class * display);

    void begin();
    void drawContext(DrawContext context);    
    bool addPage(DrawContext page);
    void drawNextPage();
    void redrawLastPage();
    void drawPage(String name);
    
    ~DrawerMod();
};

#endif