// Weather Station Library
// Arduino Class at FARC
// Wrapper for all the components in the weather station
#ifndef WeatherStation_h
#define WeatherStation_h
#include "Arduino.h"

class WeatherStation
{
    public:
        WeatherStation();
        float tempF();
        float tempC();
        float humidity();
        float heatIndex();
    private:
        float _temp;
};

#endif
