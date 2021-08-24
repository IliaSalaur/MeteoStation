#include <Arduino.h>
#include <Config.h>
#include <ESP8266WiFi.h>
#include <Debug.h>

#include <Button.h>
#include <Sensors.h>
#include <DrawerMod.h>
#include <TimeManager.h>
#include <Bitmap.h>

Average temperature;
Average humidity;
Average co2;

GxIO_Class io(SPI, /*CS=D8*/ D8, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class epaper(io, /*RST=D4*/ 2, /*BUSY=D2*/ 4);


DrawerMod display(&epaper);
Button button(D0);
DHT dht(D1, DHT11);
Temp_Sensor temp_sensor(&dht);
Hudm_Sensor hudm_sensor(&dht);
TimeManager* timeNtp = nullptr;
int i = 0;

DrawContextClass frame;
DrawContextClass tempChart;
DrawContextClass hudmChart;
DrawContextClass co2Chart;
Bitmaps bm;

void handleDisplay(bool redraw = 0)
{
  static uint32_t htmr = 0;
  htmr = (redraw) ? -1:htmr;
  if(millis() - htmr >= 15000)
  {
    htmr = millis();
    display.redrawLastPage();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(30);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  MillisTimer wifiTimer;
  wifiTimer.start(20000, 1);

  while (!wifiTimer.isReady()) 
  {
    delay (500);
    Serial.print (".");
    if(WiFi.status() == WL_CONNECTED)
    {
      timeNtp = new TimeManager(TIMEZONE);
      break;
    } 
  }
  
  for(int i = 0; i < 24; i++)
  {
    co2.a[i] = float(random(0, 2000));
    temperature.a[i] = float(random(0, 50));
    humidity.a[i] = float(random(0, 100));
  }

  dht.begin();
  temp_sensor.begin();
  hudm_sensor.begin();

  Serial.println(temp_sensor.getDebug());
  Serial.println(hudm_sensor.getDebug());
  
  frame.setContext(
    DrawContextBuilder("Home").addBitmap("frame", 0, 0, 252, 122, bm.frame, 3904)
    ->addText("date", 10, 16, 140, 9, "", &FreeSerifBold9pt7b)
    ->addText("temp", 20, 115, 27, 9, "", &FreeSerifBold9pt7b)
    ->addText("time", 155, 70, 90, 42, "", &FreeSerifBold18pt7b)
    ->addText("hudm", 210, 115, 27, 9, "", &FreeSerifBold9pt7b)
    ->addText("co2", 40, 70, 59, 42, "", &FreeSerifBold18pt7b)
    ->addBitmap("battery", 221, 6, 24, 12, bm.battery.low, 36)
    ->addRect("bwr", 224, 9, 15, 6, 1, GxEPD_WHITE)
    ->addRect("bper", 224, 9, 5, 6, 1)
    ->build()
    );

  DrawContextBuilder("lol").build();
  hudmChart.setContext(
    DrawContextBuilder("Hudm")
    .addChart("chart", 30, 107, 237, 0, &humidity, 0, 100, bm.chart.hudm_chart)
    ->build()
  );

  tempChart.setContext(
    DrawContextBuilder("Temp")
    .addChart("chart", 30, 107, 237, 0, &temperature, 0, 50, bm.chart.temp_chart)
    ->build()
  );

  co2Chart.setContext(
    DrawContextBuilder("Co2")
    .addChart("chart", 33, 91, 235, 15, &co2, 0, 2000, bm.chart.co2_chart)
    ->build()
  );

  display.addPage(frame.getContext());
  display.addPage(hudmChart.getContext());
  display.addPage(tempChart.getContext());
  display.addPage(co2Chart.getContext());

  display.begin();
  display.drawPage("Home");
}

void loop()
{
  i++;
  int co2Int = random(0, 2000);
  frame.set<ElText>("date")->text = timeNtp->getDateFormatted();
  frame.set<ElText>("text")->text = String("kek") + String(i);
  frame.set<ElText>("time")->text = timeNtp->getTimeFormatted();
  frame.set<ElText>("hudm")->text = String(hudm_sensor.getData(), 0) + String("%");
  frame.set<ElText>("temp")->text = String(temp_sensor.getData(), 1);
  frame.set<ElText>("co2")->text = String(co2Int);
  //frame.set<ElBitmap>("buzzer")->bitmap = (i % 2 == 1) ? bm.buzzer.off : bm.buzzer.on;
  //frame.set<ElRect>("bper")->width = (i % 5) * 4;

  switch(button.getState())
  {
  case ButtonStates::HOLDED:
    display.drawPage("Home");
    break;

  case ButtonStates::CLICKED:
    display.drawNextPage();
    break;
  }

  if(timeNtp)
  {
    hudm_sensor.handleAverageData(timeNtp->getTime().hour, &humidity);
    temp_sensor.handleAverageData(timeNtp->getTime().hour, &temperature);
    //co2_sensor.handleAverageData(timeNtp->getTime().hour, &co2);
    co2.a[random(0, 24)] = co2Int;
  }
  handleDisplay();
}

