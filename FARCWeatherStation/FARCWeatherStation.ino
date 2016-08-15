//FARC Weather Station Program
//This runs on the Arduino that has all the sensors connected to it and is out on location
//Bob Gusek
//Arduino Class
//2/11/2016
//changes
//2/14/2016 - Added DHT22 code

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
WeatherStation _weatherStation(2);
// DHT _dht(DHTPIN, DHTTYPE);
float _tempF = -99;
float _tempC = -99;
float _humidity = -99;
float _heatIndex = -99;
unsigned long _lastMillis;
unsigned int _interval = 5000;
boolean _refreshed = false;

void setup() {
  Serial.begin(9600);
  
  _lastMillis = 0;
  #ifdef DEBUG
    Serial.println("DEBUG MODE"); 
    Serial.print("DHT Pin:");
    Serial.println(_weatherStation.dhtPin());
  #else ifdef DEBUGRAW
    Serial.println("DEBUG RAW MODE"); 
    Serial.print("DHT Pin:");
    Serial.println(_weatherStation.dhtPin());
  #endif
}

void loop() {
  Refresh();
  #ifdef DEBUG
    if(_refreshed)
    {
      Serial.print("Humidity:    ");
      Serial.println(_humidity);
      Serial.print("Temperature: ");
      Serial.print(_tempF);
      Serial.println(" F");
      Serial.print("             ");
      Serial.print(_tempC);
      Serial.println(" C");
      Serial.print("Heat Index:  ");
      Serial.println(_heatIndex);
      Serial.println("-------------------");
      _refreshed = false;
    }
  #else ifdef DEBUGRAW
    if(_refreshed)
    {
      Serial.print("Humidity:    ");
      Serial.println(_humidity);
      Serial.print("Temperature: ");
      Serial.print(_tempF);
      Serial.println(" F");
      Serial.print("             ");
      Serial.print(_tempC);
      Serial.println(" C");
      Serial.print("Heat Index:  ");
      Serial.println(_heatIndex);
      Serial.println("-------------------");
      delay(1000);   Serial.print("Last Millis: ");
      Serial.println(_lastMillis);
      Serial.print("Curr Millis: ");
      Serial.println(millis());
      Serial.println("-------------------");
      _refreshed = false;
    }
  #endif
}

void Refresh()
{
  if(_lastMillis + _interval < millis())
  {
    GetHumidity();
    _weatherStation.tempF();
    _weatherStation.tempC();
    GetHeatIndex();
    _lastMillis = millis();
    _refreshed = true;
  }
}

float GetHumidity()
{

}

float GetTempF()
{
  // _tempF = _dht.readTemperature(true);
}

float GetTempC()
{
  // _tempC = _dht.readTemperature();
}

float GetHeatIndex()
{
  // _heatIndex = _dht.computeHeatIndex(_tempF, _humidity);
}

