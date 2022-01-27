#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <WiFiUdp.h>
#include <NTPClient.h>
struct TimeStruct
{
  uint8_t hour;
  uint8_t minute;

  TimeStruct() : hour(0), minute(0){}

  TimeStruct(uint8_t h, uint8_t m) : hour(h), minute(m){}

};

class TimeManager
{
private:
  WiFiUDP ntpUDP;
  NTPClient* timeClient;
  int offset = 0;

  uint8_t _processOfsset()
  {
    uint8_t h = timeClient->getHours();

    if((h + offset) / 24 == 1)
    {
      return h + offset - 24;
    }
    else if(h + offset < 0)
    {
      return 24 + (h + offset);
    }
    return h + offset;
  }

public:
  TimeManager(int toffset)
  {
    timeClient = new NTPClient(ntpUDP);
    timeClient->setUpdateInterval(5000U);
    offset = toffset;
  }

  TimeStruct getTime()
  {
    timeClient->update();
    return TimeStruct(this->_processOfsset(), timeClient->getMinutes());
  }

  String getTimeFormatted()
  {
    timeClient->update();
    uint8_t h = this->_processOfsset();
    uint8_t m = timeClient->getMinutes();

    String timeStr;
    timeStr += (h < 10) ? String(0) + String(h):String(h);
    timeStr += String(":");
    timeStr += (m < 10) ? String(0) + String(m):String(m);
    return timeStr;
  }

  String getDateFormatted(int format = 0)
  {
    timeClient->update();
    String date;
    date = timeClient->getFormattedDate();
    date.remove(date.indexOf("T"));

    if(format)
    {
      date.replace('-', '.');
    }
    return date;
  }
};

#endif