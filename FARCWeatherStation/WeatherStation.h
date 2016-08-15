// Weather Station Library
// Arduino Class at FARC
// Wrapper for all the components in the weather station
#ifndef WeatherStation_h
#define WeatherStation_h
#include "Arduino.h"

class WeatherStation
{
    public:
        WeatherStation(int dhtPin);
        float tempF();
        float tempC();
        int dhtPin();
    private:
        int _dhtPin;
        float _temp;
};

#endif
