#ifndef METEOSTATION_CONFIG_H
#define METEOSTATION_CONFIG_H

#define DISPLAY213 //2.13 inch
//#define DISPLAY290 //2.9 inch

#define WIFI_SSID "leonid"
#define WIFI_PASS "sl820710"

#define TIMEZONE 3
#define CO2_MAX_LIMIT 1700

#define EMULATE_CO2 false

#ifdef DISPLAY213
#define WIDTH_D 252
#define HEIGHT_D 122
#elif defined(DISPLAY290)
#define WIDTH_D 296
#define HEIGHT_D 128
#endif

#endif