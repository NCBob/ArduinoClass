// Weather Station Library
// Arduino Class at FARC
// Wrapper for all the components in the weather station
#include "Arduino.h"
#include "WeatherStation.h"

WeatherStation::WeatherStation(int dhtPin)
{
    _dhtPin = dhtPin;
    _temp = -99;
}

float WeatherStation::tempF()
{
    return 78;
}

float WeatherStation::tempC()
{
    return 26;
}

int WeatherStation::dhtPin()
{
    return _dhtPin;
}