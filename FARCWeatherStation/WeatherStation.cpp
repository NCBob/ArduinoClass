// Weather Station Library
// Arduino Class at FARC
// Wrapper for all the components in the weather station
#include "Arduino.h"
#include "WeatherStation.h"
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT _dht(DHTPIN, DHTTYPE);

WeatherStation::WeatherStation()
{
    _temp = -99;
}

float WeatherStation::tempF()
{
    return _dht.readTemperature(true);  
}

float WeatherStation::tempC()
{
    return _dht.readTemperature();
}

float WeatherStation::humidity()
{
    float humidity = _dht.readHumidity();
    if (isnan(humidity))
    {
        humidity = -199.99;
    }
    return humidity;
}

float WeatherStation::heatIndex()
{
    return _dht.computeHeatIndex(tempF(), humidity());
}