#ifndef METEOSTATION_CONFIG_H
#define METEOSTATION_CONFIG_H

//#define DISPLAY213 //2.13 inch
#define DISPLAY290 //2.9 inch

#define WIFI_SSID "HUAWEI P30 Pro"
#define WIFI_PASS "12345678"
 
#define TIMEZONE 3 //часовой пояс
#define CO2_MAX_LIMIT 1200 // лимит со2

#define EMULATE_CO2 false

#ifdef DISPLAY213
#define WIDTH_D 252
#define HEIGHT_D 122
#elif defined(DISPLAY290)
#define WIDTH_D 296
#define HEIGHT_D 128
#endif

#endif