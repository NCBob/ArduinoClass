//FARC Weather Station Program
//This runs on the Arduino that has all the sensors connected to it and is out on location
//Bob Gusek
//Arduino Class
//2/11/2016
//changes
//2/14/2016 - Added DHT22 code
//9/23/2016 - refactored into seperate file and finsihed DHT code
//9/28/2016 - Fixed the heat index C reading from the WeatherStation class
//9/28/2016 - Added code for GY30 Light Intensity sensors
//9/28/2016 - Added code for GYVEML6070 UV sensor
//9/28/2016 - Added code for RTC module

//includes
#include "WeatherStation.h"

//Defines
//#define DEBUG
#define DEBUGRAW

//Variable declarations
WeatherStation _weatherStation(A1);
unsigned long _lastMillis;
unsigned int _interval = 5000;
boolean _refresh = false;

void setup() 
{
  Serial.begin(9600);

  _weatherStation.init();
  _weatherStation.setTime(2016,10,1,8,0,0);
  _lastMillis = millis() - _interval;
  #ifdef DEBUG
    Serial.println("DEBUG MODE"); 
  #else ifdef DEBUGRAW
    Serial.println("DEBUG RAW MODE"); 
  #endif
}

void loop() {
  CheckForRefresh();
  #ifdef DEBUG
    if(_refresh)
    {
      PrintData();
    }
  #else ifdef DEBUGRAW
    if(_refresh)
    {
      PrintData();
      delay(1000);   
      Serial.print("Soil:        ");
      Serial.println(_weatherStation.rawSoilMoisture());
      Serial.print("Last Millis: ");
      Serial.println(_lastMillis);
      Serial.print("Curr Millis: ");
      Serial.println(millis());
      Serial.println("-------------------");
    }
  #endif
  _refresh = false;
}

void CheckForRefresh()
{
  if(_lastMillis + _interval < millis())
  {
    _lastMillis = millis();
    _refresh = true;
  }
}

void PrintData()
{
  Serial.print("Time:            ");
  Serial.println(_weatherStation.time());
  Serial.print("Humidity:        ");
  Serial.println(_weatherStation.humidity());
  Serial.print("Temperature:     ");
  Serial.print(_weatherStation.tempF());
  Serial.println(" F");
  Serial.print("Heat Index:      ");
  Serial.print(_weatherStation.heatIndexF());
  Serial.println(" F");
  Serial.print("Temperature:     ");
  Serial.print(_weatherStation.tempC());
  Serial.println(" C");
  Serial.print("Heat Index:      ");
  Serial.print(_weatherStation.heatIndexC());
  Serial.println(" C");
  Serial.print("Pressure:        ");
  Serial.println(_weatherStation.pressure());
  Serial.print("Soil Moisture:   ");
  Serial.print(_weatherStation.soilMoisture());
  Serial.println(" %");
  Serial.print("Light Intensity: ");
  Serial.print(_weatherStation.lux());
  Serial.println(" lux");
  Serial.print("UV:              ");
  Serial.print(_weatherStation.uv());
  Serial.println(" uW/cm2");
  Serial.println("-------------------");
}