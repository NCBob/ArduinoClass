// Weather Station Library
// Arduino Class at FARC
// Wrapper for all the components in the weather station
#ifndef WeatherStation_h
#define WeatherStation_h
#include "Arduino.h"

class WeatherStation
{
    public:
        WeatherStation(int soilPin);
        float tempF();
        float tempC();
        float humidity();
        float heatIndexF();
        float heatIndexC();
        float pressure();
        float soilMoisture();
        float rawSoilMoisture();
        int lux();
        String time();
        void setTime(float setYear, float setMonth, float setDay, float setHour, float setMinute, float setSecond);
        uint16_t uv();
        void init();
    private:
        void GY30Init();
        byte GY30Read();
        float _temp;
        String padValue(int value);
};

#endif
