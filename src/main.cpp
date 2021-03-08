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
#define DELAY delay(1);

String getNTPData();
int analogReadBut();
byte buttonPressed();
void drawNextPage(Page pageNow);

const char *ssid     = "hinev1";
const char *password = "069052345";
int lastMin = -1;
bool wifiState = 0;
bool butTimerStart = 0;
uint32_t analogTimer = 0;
uint32_t butTimer = 0;
uint32_t ntpTimerMillis = 0;

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
  pinMode(A0, INPUT);
  
  dht.begin();
  temp_sensor.begin();
  hudm_sensor.begin();

  Serial.println(hudm_sensor.getDebug());
  Serial.println(hudm_sensor.getDebug());


  epaper.init();
  epaper.setRotation(1);
  epaper.eraseDisplay();
  
  ntpTimer->start(10000, 1);
}

void loop() {
  if(millis() - ntpTimerMillis > 10000)
  {
    ntpTimerMillis = millis();
    Serial.println("ntpTimer ready");
    getNTPData();
  }  

  if(lastMin != _time.minute)
  {
    display.drawAll(50, 1, getNTPData(), 996, &_time, temp_sensor.getData(), hudm_sensor.getData(), 1);
    lastMin = _time.minute;
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

  switch(buttonPressed())
  {
    case 2:
      display.drawAll(50, 1, getNTPData(), 996, &_time, temp_sensor.getData(), hudm_sensor.getData(), 1);
      break;

    case 1:
      drawNextPage(display.getPage());
      break;

    case 0: 
      break;
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
    switch(timeClient.getHours())
    {
      case 22:
        _time.hour = 0;
        break;

      case 23:
        _time.hour = 1;
        break;

      default:
        _time.hour = timeClient.getHours() + TIMEZONE;
        break;
    }

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

   Serial.println(String(_time.hour) + String(":") + String(_time.minute));
   DELAY

   return date;
  }

  else 
  {
    _time.hour = -1;
    _time.minute = -1;
    return String(""); 
  }
}

byte buttonPressed()
{
  int analog = analogReadBut();
  int milHolded;

  if(analog > 700 && butTimerStart == 0)
  {
    butTimer = millis();
    butTimerStart = 1;
    
    Serial.println("butTimer start");
    
  }
  else if(analog < 700 && butTimerStart == 1)
  {
    butTimerStart = 0;
    milHolded = millis() - butTimer;

    Serial.println(String("butTimer stop , milHolded: ") + String(milHolded));


    if(milHolded > 900) return 2;
    else if(milHolded > 80) return 1;
    else return 0;
  }
  else return 0;
}

int analogReadBut()
{
  static int lastRead;
  if(millis() - analogTimer > 10) 
  {
    analogTimer = millis();
    lastRead = analogRead(A0);
    Serial.print("Analog: ");
    Serial.println (lastRead);
    return lastRead;
  }
  else return lastRead;
}

void drawNextPage(Page pageNow)
{
  pageNow = display.getPage();
  switch(pageNow)
  {
    case FRAME:
      display.drawChart(TEMP, &temperature);
      break;

    case TEMPCHART:
      display.drawChart(HUDM, &humidity);
      break;
  
    case HUDMCHART:
      display.drawAll(50, 1, getNTPData(), 996, &_time, temp_sensor.getData(), hudm_sensor.getData(), 1);
      break;

    case CLEAR:
      display.drawAll(50, 1, getNTPData(), 996, &_time, temp_sensor.getData(), hudm_sensor.getData(), 1);
      break;
  }
}


