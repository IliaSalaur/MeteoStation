#include <Arduino.h>
#include <Config.h>
#include <ESP8266WiFi.h>
#include <Debug.h>

#include <Context.h>
#include <Button.h>
#include <Sensors.h>
#include <DrawerMod.h>
#include <TimeManager.h>
#include <BlynkSimpleEsp8266.h>

#define AUTH_TOKEN "qqklouv9Shw7gGmSzenYJTh7wWwQbApl"


Average temperature;
Average humidity;
Average co2;

GxIO_Class io(SPI, /*CS=D8*/ D8, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class epaper(io, /*RST=D4*/ 2, /*BUSY=D2*/ 4);


DrawerMod display(&epaper, WIDTH_D, HEIGHT_D);
Button button(D0);

MHZ19PWM mhz(D6, MHZ_ASYNC_MODE);
DHT dht(D1, DHT11);

Temp_Sensor temp_sensor(&dht);
Hudm_Sensor hudm_sensor(&dht);
CO2_Sensor co2_sensor(&mhz, EMULATE_CO2);

TimeManager* timeNtp = nullptr;

bool wf_connected = 0;
int i = 0;

DrawContextClass home;
DrawContextClass tempChart;
DrawContextClass hudmChart;
DrawContextClass co2Chart;

void handleDisplay(bool redraw = 0)
{
  static uint32_t htmr = -60000;
  htmr = (redraw) ? -1:htmr;
  if(millis() - htmr >= 61000)
  {
    htmr = millis();
    display.redrawLastPage();
  }
}

void syncRedraw(uint8_t min)
{
  static uint8_t _lastMin = 0;
  if(_lastMin != min)
  {
    handleDisplay(1);
    _lastMin = min;
  }
}

void onMaxLimit()
{
  Blynk.notify(String("Открой окно! CO2 превысил концентрацию в ") + String(CO2_MAX_LIMIT) + String("ppm!"));
  DEBUG("LIMIT!!!");
}

void handleBlynkData()
{
  static uint32_t tmr = 0;
  if(millis() - tmr >= 10000)
  {
    tmr = millis();
    Blynk.virtualWrite(V0, temp_sensor.getData());
    Blynk.virtualWrite(V1, hudm_sensor.getData());
    Blynk.virtualWrite(V2, co2_sensor.getData());
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(30);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setAutoReconnect(true);

  MillisTimer wifiTimer;
  wifiTimer.start(20000, 1);

  while (!wifiTimer.isReady()) 
  {
    delay (500);
    Serial.print (".");
    if(WiFi.status() == WL_CONNECTED)
    {
      timeNtp = new TimeManager(TIMEZONE);
      Blynk.begin(AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
      wf_connected = 1;
      break;
    } 
  }
  
  for(int i = 0; i < 24; i++)
  {/*
    co2.a[i] = float(random(0, 2000));
    temperature.a[i] = float(random(0, 50));
    humidity.a[i] = float(random(0, 100));*/
  }
  
  dht.begin();
  temp_sensor.begin();
  hudm_sensor.begin();
  co2_sensor.begin();
  co2_sensor.onMaxLimit(onMaxLimit, CO2_MAX_LIMIT);

  Serial.println(temp_sensor.getDebug());
  Serial.println(hudm_sensor.getDebug());
  
  home.setContext(Context::getHome());

  hudmChart.setContext(Context::getHumidityChart(&humidity));

  tempChart.setContext(Context::getTempChart(&temperature));

  co2Chart.setContext(Context::getC02Chart(&co2));

  display.addPage(home.getContext(), " ");
  display.addPage(tempChart.getContext(), "Home");
  display.addPage(hudmChart.getContext(),"Home");
  display.addPage(co2Chart.getContext(), "Home");

  display.begin();
  display.drawPage("Home");
}

void loop()
{
  i++;
  home.set<ElText>("date")->text = timeNtp->getDateFormatted();
  home.set<ElText>("text")->text = String("kek") + String(i);
  home.set<ElText>("time")->text = timeNtp->getTimeFormatted();
  home.set<ElText>("hudm")->text = String(hudm_sensor.getData(), 0) + String("%");
  home.set<ElText>("temp")->text = String(temp_sensor.getData(), 1);
  home.set<ElText>("co2")->text = String(co2_sensor.getData(), 0);
  //home.set<ElBitmap>("buzzer")->bitmap = (i % 2 == 1) ? bm.buzzer.off : bm.buzzer.on;
  //home.set<ElRect>("bper")->width = (i % 5) * 4;

  switch(button.getState())
  {
  case ButtonStates::HOLDED:
    display.drawPage("Home");
    break;

  case ButtonStates::CLICKED:
    display.drawNextPage();
    break;
  }

  if(WiFi.status() == WL_CONNECTED)
  {
    Blynk.run();
    hudm_sensor.handleAverageData(timeNtp->getTime().hour, &humidity);
    temp_sensor.handleAverageData(timeNtp->getTime().hour, &temperature);
    co2_sensor.handleAverageData(timeNtp->getTime().hour, &co2);
    syncRedraw(timeNtp->getTime().minute);
    handleBlynkData();
  }
  else{
    WiFi.reconnect();
  }
  handleDisplay();
}



