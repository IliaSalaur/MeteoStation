#ifndef CONTEXT_H
#define CONTEXT_H

#include <DrawContext.h>
#include <Bitmap.h>
#include <Fonts/FreeSerifBold18pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Config.h>

namespace ContextUtils
{
    Bitmaps bm;
};

#ifdef DISPLAY213

class Context
{
private:
    
public:
    Context() {}

    static DrawContext getHome()
    {
        return(DrawContextBuilder("Home")
    .addBitmap("frame", 0, 0, 252, 122, ContextUtils::bm.frame)
    ->addText("date", 10, 16, 140, 9, "", &FreeSerifBold9pt7b)
    ->addText("temp", 20, 115, 27, 9, "", &FreeSerifBold9pt7b)
    ->addText("time", 155, 70, 90, 42, "", &FreeSerifBold18pt7b)
    ->addText("hudm", 210, 115, 27, 9, "", &FreeSerifBold9pt7b)
    ->addText("co2", 40, 70, 59, 42, "", &FreeSerifBold18pt7b)
    ->addBitmap("battery", 221, 6, 24, 12, ContextUtils::bm.battery.low)
    ->addRect("bwr", 224, 9, 15, 6, 1, GxEPD_WHITE)
    ->addRect("bper", 224, 9, 5, 6, 1)
    ->build());
    }

    static DrawContext getC02Chart(Average* avg)
    {
        return(DrawContextBuilder("Co2")
        .addChart("chart", 33, 91, 235, 15, avg, 0, 2000, ContextUtils::bm.chart.co2_chart)
        ->build());
    }

    static DrawContext getHumidityChart(Average* avg)
    {
        return(DrawContextBuilder("Hudm")
        .addChart("chart", 30, 107, 237, 5, avg, 0, 100, ContextUtils::bm.chart.hudm_chart)
        ->build());
    }

    static DrawContext getTempChart(Average* avg)
    {
        return(DrawContextBuilder("Temp")
        .addChart("chart", 30, 107, 237, 5, avg, 0, 50, ContextUtils::bm.chart.temp_chart)
        ->build());
    }
    
};

#elif defined(DISPLAY290)

    class Context
{
private:
    
public:
    Context() {}

    static DrawContext getHome()
    {
        return(DrawContextBuilder("Home")
    .addBitmap("frame", 0, 0, 296, 128, ContextUtils::bm.frame)//
    ->addText("date", 10, 16, 140, 9, "", &FreeSerifBold9pt7b)//
    ->addText("temp", 20, 115, 27, 9, "", &FreeSerifBold9pt7b)//
    ->addText("time", 169, 70, 90, 42, "", &FreeSerifBold18pt7b)//
    ->addText("hudm", 256, 115, 27, 9, "", &FreeSerifBold9pt7b)//
    ->addText("co2", 40, 70, 59, 42, "", &FreeSerifBold18pt7b)//
    ->addBitmap("battery", 267, 6, 24, 12, ContextUtils::bm.battery.low)//
    ->addRect("bwr", 270, 9, 15, 6, 1, GxEPD_WHITE)//
    ->addRect("bper", 270, 9, 5, 6, 1)//
    ->build());
    }

    static DrawContext getC02Chart(Average* avg)
    {
        return(DrawContextBuilder("Co2")
        .addChart("chart", 39, 96, 275, 16, avg, 0, 2000, ContextUtils::bm.chart.co2_chart)//
        ->build());
    }

    static DrawContext getHumidityChart(Average* avg)
    {
        return(DrawContextBuilder("Hudm")
        .addChart("chart", 39, 96, 275, 16, avg, 0, 100, ContextUtils::bm.chart.hudm_chart)//
        ->build());
    }

    static DrawContext getTempChart(Average* avg)
    {
        return(DrawContextBuilder("Temp")
        .addChart("chart", 39, 96, 275, 16, avg, 0, 50, ContextUtils::bm.chart.temp_chart)//
        ->build());
    }
    
};
#endif

#endif