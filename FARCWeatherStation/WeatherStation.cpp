// Weather Station Library
// Arduino Class at FARC
// Wrapper for all the components in the weather station
#include "Arduino.h"
#include "WeatherStation.h"
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Wire.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT _dht(DHTPIN, DHTTYPE);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
int _soilPin = A0;
int _soilMax = 750;
int _soilMin = 80;
int _GY30addr = 0x23; // GY-30 Address
byte _gy30Buff[2];
bool bmpValid = false;

WeatherStation::WeatherStation(int soilPin)
{
    _soilPin = soilPin;
}

float WeatherStation::tempF()
{
    float tempF = _dht.readTemperature(true);
    if(isnan(tempF))
    {
        return -99.99;
    }
    return tempF;  
}

float WeatherStation::tempC()
{
    float tempC = _dht.readTemperature();
    if(isnan(tempC))
    {
        return -99.99;
    }
    return tempC;
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

float WeatherStation::heatIndexF()
{
    return _dht.computeHeatIndex(tempF(), humidity());
}

float WeatherStation::heatIndexC()
{
    //(°F  -  32)  x  5/9
    float heatIndex = heatIndexF();
    heatIndex = heatIndex - 32;
    heatIndex = heatIndex * (5.0/9.0);
    return heatIndex;
}

float WeatherStation::pressure()
{
    if(bmpValid)
    {
        sensors_event_t event;
        bmp.getEvent(&event);
        return event.pressure * 0.0295299830714;
    }

  return -199.99;
}

float WeatherStation::soilMoisture()
{
    float value = analogRead(_soilPin);

    if(value > _soilMax)
    {
        value = _soilMax;
    }
    else if (value < _soilMin)
    {
        value = _soilMin;
    }
    value = (1 - ((value-_soilMin)/(_soilMax-_soilMin))) * 100;
    
    return value; 
}

float WeatherStation::rawSoilMoisture()
{
    return analogRead(_soilPin);
}

void WeatherStation::init()
{
    Wire.begin();
    GY30Init();
    if(bmp.begin())
    {
        bmpValid = true;
    }
}

int WeatherStation::lux()
{
    float val = -99.99;

    if(GY30Read() == 2)
    {
        val = ((_gy30Buff[0]<<8)|_gy30Buff[1])/1.2;
        if (val<0)
        {
            val = -99.99;
        }
    }
    return (int)val;
}

byte WeatherStation::GY30Read()
{
    byte i=0;
    Wire.beginTransmission(_GY30addr);
    Wire.requestFrom(_GY30addr, 2);
    while(Wire.available())
    {
        _gy30Buff[i] = Wire.read(); 
        i++;
    }
    Wire.endTransmission();  
    return i;
}

void WeatherStation::GY30Init()
{
    Wire.beginTransmission(_GY30addr);
    Wire.write(0x10); // 1 [lux] resolution
    Wire.endTransmission();  
}



// byte BH1750_Read(int address){
  
//   byte i=0;
//   Wire.beginTransmission(address);
//   Wire.requestFrom(address, 2);
//   while(Wire.available()){
//     buff[i] = Wire.read(); 
//     i++;
//   }
//   Wire.endTransmission();  
//   return i;
// }
