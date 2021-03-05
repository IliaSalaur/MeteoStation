#include <Arduino.h>
#include <GxEPD.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include <Sensors.h>
#include <Drawer.h>
#include <Bitmap.h>

// select the display class to use, only one, copy from GxEPD_Example
#include <GxGDE0213B1/GxGDE0213B1.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#define TIMEZONE 2

String getNTPData();

const char *ssid     = "hinev1";
const char *password = "069052345";
int lastMin = -1;
bool wifiState = 0;

AverageStr temperature;
AverageStr humidity;

GxIO_Class io(SPI, /*CS=D8*/ D8, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class epaper(io, /*RST=D4*/ 2, /*BUSY=D2*/ 4);

MillisTimer *ntpTimer = new MillisTimer;
Bitmaps bitmaps PROGMEM;
TimeStruct _time;

Drawer display(&epaper, &bitmaps);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

DHT dht(D1, DHT11);
Temp_Sensor temp_sensor(&dht);
Hudm_Sensor hudm_sensor(&dht);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(30);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  MillisTimer *wifiTimer = new MillisTimer;
  wifiTimer->start(10000, 1);

  while (!wifiTimer->isReady()) 
  {
    delay (500);
    Serial.print (".");
    if(WiFi.status() == WL_CONNECTED)
    {
      wifiState = 1;
      break;
    } 
  }

  dht.begin();
  temp_sensor.begin();
  hudm_sensor.begin();

  Serial.println(hudm_sensor.getDebug());
  Serial.println(hudm_sensor.getDebug());


  epaper.init();
  epaper.setRotation(1);
  epaper.eraseDisplay();
  
  ntpTimer->start(10000, 0);
}

void loop() {
  if(ntpTimer->isReady())
  {
     getNTPData();
  }  

  if(lastMin != _time.minute)
  {
    display.drawAll(50, 1, getNTPData(), 996, &_time, temp_sensor.getData(), hudm_sensor.getData(), 1);
  }

  if(Serial.available())
  {
    switch (Serial.read())
    {
    case 'h':
      display.drawChart(HUDM, &humidity);
      break;
    
    case 't':
      display.drawChart(TEMP, &temperature);
      break;
    }
  }

  hudm_sensor.handleAverageData(_time.hour, &humidity);
  temp_sensor.handleAverageData(_time.hour, &temperature);

}



String getNTPData()
{
  if ( wifiState == 1)
  {
    timeClient.update();
    _time.minute = timeClient.getMinutes();
    _time.hour = timeClient.getHours() + TIMEZONE;
    lastMin = _time.minute;

    String date;
    date = timeClient.getFormattedDate();
    date.remove(date.indexOf("T"));
    /*
      //formDat.replace("-", ".");
      String day = formDat.substring(8);
      String month = formDat.substring(5, 7);
      String year = formDat.substring(0, 4);
      formDat = day + " . " + month + " . " + year;
    */
   return date;
  }

  else 
  {
    _time.hour = -1;
    _time.minute = -1;
    return String(""); 
  }
}
