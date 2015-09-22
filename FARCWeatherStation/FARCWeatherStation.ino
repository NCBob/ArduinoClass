//FARC Weather Station Program
//This runs on the Arduino that has all the sensors connected to it and is out on location
//Bob Gusek
//Arduino Class
//9/21/2015

//Includes
#include "DHT.h";
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

//Defines
#define DEBUG
#define DEBUGRAW
#define DHTPIN 2
#define DHTTYPE DHT22

//Declarations
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

//Declaring default values if nothing has been read from the sensors
float humidity = -99.99;
float tempF = -99.9;
float tempC = -99.9;
float currPressure = -99.99;
bool bmpValid = false;
const int uvPin = A0;
int uvIndex = -99;
const int soilPin = A1;
float soilValue = -99;

void setup() {
  Serial.begin(9600);
  dht.begin();
  bmpValid = bmp.begin();
  #ifdef DEBUG
  Serial.println("DEBUG MODE");  
  #endif
}

void loop() {
  #ifdef DEBUG
  Serial.println("Humidity:   " + String(GetHumidity()));
  Serial.println("Temp F:     " + String(GetTempF()));
  Serial.println("Temp C:     " + String(GetTempC()));
  Serial.println("Pressure:   " + String(GetPressure()));
  Serial.println("UV Value:   " + String(GetUVIndex()));
  Serial.println("Soil Value: " + String(GetSoilValue()));
  Serial.println();
  delay(1000);
  #else
    #ifdef DEBUGRAW
    GetUVIndex();
    GetSoilValue();
    delay(1000);
    #endif
  #endif
}

float GetTempC()
{
    //reads in C pass TRUE to read in F
    tempC = dht.readTemperature();
    if (isnan(tempC))
    {
      tempC = -99.9;
    }
    return tempC;
}

float GetTempF()
{
    //reads in C pass TRUE to read in F
    tempF = dht.readTemperature(true);
    if (isnan(tempF))
    {
        tempF = -99.9;
    }
  return tempF;
}

float GetHumidity()
{
  humidity = dht.readHumidity();
  if (isnan(humidity))
  {
      humidity = -99.99;
  }
 return humidity;
}

float GetPressure()
{
  if(bmpValid)
  {
    sensors_event_t event;
    bmp.getEvent(&event);
    currPressure = event.pressure * 0.0295299830714;
  }
  else
  {
    currPressure = -99.99;
  }
  return currPressure;
}

int GetUVIndex()
{
  #ifdef DEBUGRAW
  Serial.print("Raw UV: ");
  Serial.println(analogRead(uvPin)); 
  #endif
  uvIndex = analogRead(uvPin)/10;
  return uvIndex;
}

float GetSoilValue()
{
  soilValue = analogRead(soilPin);
  #ifdef DEBUGRAW
  Serial.print("Raw soil value:");
  Serial.println(soilValue);
  #endif
  //max is around 700 so we determine percentage of moisture where 750 = dry
  if(soilValue > 500)
    soilValue = 500;
  soilValue = (1 - (soilValue/500)) * 100;
  return soilValue; 
}
