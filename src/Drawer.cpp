#include <Drawer.h>

Drawer::Drawer(GxEPD_Class * display, Bitmaps * bitmaps)
{
    _display = display;
    _bitmaps = bitmaps;
    _display->setRotation(1);
}

Page Drawer::getPage()
{
    return page;
}

void Drawer::drawAll(byte batteryP, bool buzzerState, String date, int co2, TimeStruct * time, float temp, float hudm, bool updt = 1)
{
    _display->drawBitmap(_bitmaps->frame, 0, 0, 252, 121, GxEPD_BLACK);
    drawBattery(batteryP, 0);
    drawBuzzer(buzzerState, 0);

    drawUpperText(date, 0);
    drawMiddleText(co2, time, 0);
    drawDownerText(temp, hudm, 0);
    if(updt == 1)
    {
        if(page == FRAME)   this->_update(0, 0, 252, 122);
        else this->_update();
    }   
    page = FRAME;
}

void Drawer::drawBattery(byte batteryP, bool updt = 1)
{
  _display->fillRect(221, 6, 26, 14, GxEPD_WHITE);
  switch (batteryP)
  {
    case 5:
      _display->drawBitmap(_bitmaps->battery.low, 222, 5, 24, 12, GxEPD_BLACK);
      break;

    case 25:
      _display->drawBitmap(_bitmaps->battery.quarter, 222, 5, 24, 12, GxEPD_BLACK);
      break;

    case 50:
      _display->drawBitmap(_bitmaps->battery.half, 222, 5, 24, 12, GxEPD_BLACK);
      break;

    case 75:
      _display->drawBitmap(_bitmaps->battery.three_quarters, 222, 5, 24, 12, GxEPD_BLACK);
      break;

    case 100:
      _display->drawBitmap(_bitmaps->battery.full, 222, 5, 24, 12, GxEPD_BLACK);
      break;

    case 255:
      _display->drawBitmap(_bitmaps->battery.charge, 222, 5, 24, 12, GxEPD_BLACK);
      break;
  }
    if(updt == 1)   this->_update(221, 6, 26, 14);
}

void Drawer::drawBuzzer(bool buzzerState, bool updt = 1)
{
    _display->fillRect(110, 29, 13, 15, GxEPD_WHITE);
    switch (buzzerState)
    {
    case 0:
      _display->drawBitmap(_bitmaps->buzzer.off, 110, 30, 18, 13, GxEPD_BLACK);
      break;

    case 1:
      _display->drawBitmap(_bitmaps->buzzer.on, 110, 30, 18, 13, GxEPD_BLACK);
      break;
    }
    if(updt == 1) this->_update(110, 29, 13, 15);
}

void Drawer::drawUpperText(String date, bool updt = 1)
{
    _display->setTextColor(GxEPD_BLACK);
    _display->setFont(&FreeSerifBold9pt7b);
    _display->setTextColor(GxEPD_BLACK);
    _display->setFont(&FreeSerifBold9pt7b);

    _display->fillRect(10, 2, 140, 18, GxEPD_WHITE);
    _display->setCursor(10, 16);
    _display->print(date);

    if(updt == 1) this->_update(10, 2, 140, 18);
}

void Drawer::drawMiddleText(int co2,  TimeStruct * time, bool updt = 1)
{
    _display->setTextColor(GxEPD_BLACK);
    _display->fillRect(40, 57, 59, 40, GxEPD_WHITE);
    _display->setCursor(40, 70);
    _display->setFont(&FreeSerifBold18pt7b);
    _display->print(co2);

    _display->fillRect(155, 57, 90, 40, GxEPD_WHITE);
     _display->setCursor(155, 70);
    String timeStr = "";
    if(time->hour > -1 && time->minute > -1)
    {
        if (time->hour < 10)
        {
            timeStr += "0";
            timeStr += String(time->hour);
        }
        else timeStr = String(time->hour);
        timeStr += ":";
        if (time->minute < 10)
        {
            timeStr += "0";
            timeStr += String(time->minute);
        }
        else timeStr += String(time->minute);
        _display->print(timeStr);
        //Serial.println(timeStr);
    }
    else
    {
        _display->setFont(&FreeSerifBold9pt7b);
        _display->print("No WiFi");
    } 
    

    if(updt == 1) 
    {
        this->_update(155, 57, 90, 40);
        delay(200);
        this->_update(40, 57, 59, 40);
    }
}

void Drawer::drawDownerText(float temp, int hudm, bool updt = 1)
{
    _display->setTextColor(GxEPD_BLACK);
    _display->setFont(&FreeSerifBold9pt7b);
    _display->fillRect(20, 102, 27, 18, GxEPD_WHITE);
    _display->setCursor(20, 115);
    _display->print(temp, 1);

    _display->fillRect(210, 102, 60, 18, GxEPD_WHITE);
    _display->setCursor(210, 115);
    _display->print(hudm);
    _display->print("%");

    if(updt == 1) 
    {
        this->_update(20, 102, 27, 18);
        delay(200);
        this->_update(210, 102, 60, 18);
    }
}

void Drawer::drawChart(ChartMode chartMode, AverageStr *avg)
{
    switch (chartMode)
    {
    case TEMP:
        _display->drawBitmap(_bitmaps->chart.temp_chart, 0, 0, 252, 122, GxEPD_BLACK);
        this->_drawChartData(30, 107, 9, 2, avg);

        if(page == TEMPCHART || page == HUDMCHART)   this->_update(0, 0, 252, 122);
        else this->_update();
        page = TEMPCHART;
        break;
    
    case HUDM:
        _display->drawBitmap(_bitmaps->chart.hudm_chart, 0, 0, 252, 122, GxEPD_BLACK);
        this->_drawChartData(30, 107, 9, 1, avg);
        if(page == TEMPCHART || page == HUDMCHART)   this->_update(0, 0, 252, 122);      
        else this->_update();
        page = HUDMCHART;
        break;
    }
}

void Drawer::_drawChartData(byte initial_x, byte initial_y, byte k_x, byte k_y, AverageStr *data)
{
    int lastCoords[2] = {-1, -1};

    for (byte i = 0; i < 24; i++)
    {
        if (data->hour[i] > 0)
        {
            byte x = initial_x;
        x = x + i * k_x;

        byte y = initial_y;
        y = y - round(data->hour[i]) * k_y;

        _display->drawRect(x - 1, y - 1, 3, 3, GxEPD_BLACK);
        if (lastCoords[0] != -1 && lastCoords[1] != -1)
            {
                _display->drawLine(x - 1, y, lastCoords[0] + 1, lastCoords[1], GxEPD_BLACK);
            }
        lastCoords[0] = x;
        lastCoords[1] = y;
        //Serial.println("temp: " + String(data[i]) + ";  hour: " + String(i) + ";  x: " + String(x) + ";  y: " + String(y) + ";");
        }
        else 
        {
            //Serial.println("Bad data:  " + String(data[i]) + ";  hour: " + String(i));
            lastCoords[0] = -1;
            lastCoords[1] = -1;
        }
    }
}

void Drawer::_update(int x, int y, int width, int height)
{
    _display->updateWindow(x, y, width, height, true);
}

void Drawer::_update()
{
    _display->update();
}

Drawer::~Drawer()
{
    delete &page;
    delete _bitmaps;
}