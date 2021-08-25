#ifndef AVERAGE_H
#define AVERAGE_H
struct Average //\brief Average an array of float that contains the value of average temperature per hour
{
    float a[24];
    Average()
    {
        for(int i = 0; i < 24; i++)
        {
            a[i] = -1.0F;
        }
    }
}; 
#endif