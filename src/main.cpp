#include <Arduino.h>
#include <GxEPD.h>
#include <Drawer.h>
#include <Bitmap.h>

// select the display class to use, only one, copy from GxEPD_Example
#include <GxGDE0213B1/GxGDE0213B1.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

float tempValGraf[24][6] =
{
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};

float tempValGrafAvg [24];

GxIO_Class io(SPI, /*CS=D8*/ D8, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class epaper(io, /*RST=D4*/ 2, /*BUSY=D2*/ 4);

Bitmaps bitmaps PROGMEM;
TimeStruct _time;

Drawer display(&epaper, &bitmaps);

void setup() {
  Serial.begin(115200);
  for(byte i = 0; i < 24; i++)
  {
    tempValGrafAvg[i] = i+1;
  }

  epaper.init();
  epaper.setRotation(1);
  epaper.eraseDisplay();
  
}

void loop() {
  _time.hour = random(0, 24);
  _time.minute = random(0, 60);
  display.drawAll(100, 1, "22.12.2021", random(0, 2000), &_time, 23.5, 80, 1);
  delay(5000);
  _time.hour = random(0, 24);
  _time.minute = random(0, 60);
  display.drawAll(50, 1, "22.12.2021", random(0, 2000), &_time, 23.5, 80, 1);
  delay(5000);
  display.drawChart(ChartMode::TEMP, tempValGrafAvg);
  delay(5000);
  for(byte i = 0; i < 24; i++)
  {
    tempValGrafAvg[i] = i+24;
  }
  display.drawChart(ChartMode::HUDM, tempValGrafAvg);
  delay(5000);
}
