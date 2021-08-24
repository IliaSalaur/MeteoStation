#ifndef COORDS_H
#define COORDS_H

#define COORDS213

#ifdef COORDS213
struct Coords
{
    int size[2] = {250, 122};
    int battery[4] = {221, 6, 26, 14};
    int buzzer[4] = {110, 29, 13, 15};
    int upperTextCursor[2] = {10, 16};
    int upperTextRect[4] = {10, 2, 140, 18};
    int co2TextCursor[2] = {40, 70};
    int co2TextRect[4] = {40, 57, 59, 40};
    int timeText[2] = {155, 70};
    int timeRect[4] = {155, 57, 90, 40};
};
#endif

#endif