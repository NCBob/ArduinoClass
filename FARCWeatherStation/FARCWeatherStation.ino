//FARC Weather Station Program
//This runs on the Arduino that has all the sensors connected to it and is out on location
//Bob Gusek
//Arduino Class
//2/11/2016
//changes
//2/14/2016 - Added DHT22 code
//9/23/2016 - refactored into seperate file and finsihed DHT code

//includes
#include "WeatherStation.h"

//Defines
//#define DEBUG
#define DEBUGRAW
//pin DHT22 is connected to
/*#define DHTPIN 2
#define DHTTYPE DHT22
*/

//Variable declarations
WeatherStation _weatherStation;
unsigned long _lastMillis;
unsigned int _interval = 5000;
boolean _refresh = false;

void setup() 
{
  Serial.begin(9600);

  _weatherStation.init();
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
      delay(1000);   Serial.print("Last Millis: ");
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
       Serial.print("Humidity:    ");
      Serial.println(_weatherStation.humidity());
      Serial.print("Temperature: ");
      Serial.print(_weatherStation.tempF());
      Serial.println(" F");
      Serial.print("Heat Index:  ");
      Serial.print(_weatherStation.heatIndexF());
      Serial.println(" F");
      Serial.print("Temperature: ");
      Serial.print(_weatherStation.tempC());
      Serial.println(" C");
      Serial.print("Heat Index:  ");
      Serial.print(_weatherStation.heatIndexC());
      Serial.println(" C");
      Serial.print("Pressure:    ");
      Serial.println(_weatherStation.pressure());
      Serial.println("-------------------");
}