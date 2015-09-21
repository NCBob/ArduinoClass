  //FARC Weather Station Console
#include <SPI.h>
#include "RTClib.h"
#include <Wire.h>
#include "RF24.h"
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(9,10);

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

RTC_Millis rtc;

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
int reqToken = 0;
int errorCount = 0;
int contrast = 50;

int button1 = A0;    
int button2 = A1;    
int button3 = A2;    
int button4 = A3;    

float tempC = -99.9;
float tempF = -99.9;
float pressure = -99.9;
float humidity = -99.9;
float uv = -99.9;
float soilValue = -99.9;

long lastTempCCheck = 0;
long lastTempFCheck = 0;
long lastPressureCheck = 0;
long lastHumidityCheck = 0;
long lastUVCheck = 0;
long lastSoilValueCheck = 0;

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(9600);
  display.begin();
  display.setContrast(contrast);
  SplashScreen();
  //
  // Setup and configure rf radio
  //

  radio.begin();
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,50);
  radio.setPayloadSize(8);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
}

void loop(void)
{
  CheckKeys();
  DateTime now = rtc.now();
  
  switch (reqToken) 
  {
    case 0:
      SetTime(GetWeatherData(reqToken), now.month(), now.day(), now.hour(), now.minute(), now.second());
      reqToken = 1;
      break;
    case 1:
      SetTime(now.year(), GetWeatherData(reqToken), now.day(), now.hour(), now.minute(), now.second());
      reqToken = 2;
      break;
    case 2:
      SetTime(now.year(), now.month(), GetWeatherData(reqToken), now.hour(), now.minute(), now.second());
      reqToken = 3;
      break;
    case 3:
      SetTime(now.year(), now.month(), now.day(), GetWeatherData(reqToken), now.minute(), now.second());
      reqToken = 4;
      break;
    case 4:
      SetTime(now.year(), now.month(), now.day(), now.hour(), GetWeatherData(reqToken), now.second());
      reqToken = 5;
      break;
    case 5:
      SetTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), GetWeatherData(reqToken));
      reqToken = 6;
      break;
    case 6:
      if(lastTempFCheck < now.unixtime())
      {
        Serial.println("Checking TempF");
        tempF = GetWeatherData(reqToken);
        lastTempFCheck = now.unixtime() + 60;
      }
      reqToken = 7;
      break;
    case 7:
      if(lastTempCCheck < now.unixtime())
      {
        Serial.println("Checking TempC");
        tempC = GetWeatherData(reqToken);
        lastTempCCheck = now.unixtime() + 60;
      }
      reqToken = 8;
      break;
    case 8:
      if(lastPressureCheck < now.unixtime())
      {
        Serial.println("Checking Pressure");
        pressure = GetWeatherData(reqToken);
        lastPressureCheck = now.unixtime() + 2;
      }
      reqToken = 9;
      break;
    case 9:
      if(lastHumidityCheck < now.unixtime())
      {
        Serial.println("Checking Humidity");
        humidity = GetWeatherData(reqToken);
        lastHumidityCheck = now.unixtime() + 360;
      }
      reqToken = 10;
      break;
    case 10:
      uv = GetWeatherData(reqToken);
      reqToken = 11;
      break;
    case 11:
      soilValue = GetWeatherData(reqToken);
      reqToken = 6;
      break;
  }
    delay(100);

    TitleScreen();
    DisplayOutput(GetDateTime(),0,0);
    DisplayOutput(tempF," F", 36, 8);
    DisplayOutput(pressure, " in", 36, 16);
    DisplayOutput(humidity, " %", 36, 24);
    DisplayOutput(uv, 36, 32);
    DisplayOutput(soilValue, " %", 36, 40);
    display.display();
    delay(100);
    display.clearDisplay();  
 }


float GetWeatherData(int reqType)
{
  float rcvData = -99.9;
  
  // First, stop listening so we can talk.
  radio.stopListening();
  
  bool ok = radio.write( &reqType, sizeof(int));
  
  if (!ok)
  {
    errorCount += 1;
  }   
  
  // Now, continue listening
  radio.startListening();
  // Wait here until we get a response, or timeout (250ms)
  unsigned long started_waiting_at = millis();
  bool timeout = false;
  while ( ! radio.available() && ! timeout )
    if (millis() - started_waiting_at > 1000 )
      timeout = true;
      
  if (timeout)
  {
    errorCount += 1;
  }
  else
  {    
    // Grab the response, compare, and send to debugging spew
    radio.read(&rcvData, sizeof(float));
  }
  return rcvData;
}

void TitleScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  DisplayOutput("Temp:", 0, 8);
  DisplayOutput("Press:", 0, 16);
  DisplayOutput("Hum:", 0, 24);
  DisplayOutput("UV:", 0, 32);
  DisplayOutput("Soil:", 0, 40);
}

void SplashScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  DisplayOutput("Welcome to the", 0, 0);
  DisplayOutput("     FARC", 0, 8);
  display.setTextSize(2);
  DisplayOutput("WeatherStation", 0, 16);
  display.setTextSize(1);
  display.display();
  delay(1000);
}

void DisplayMessage(String msg)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.println(msg);
  display.display();
  delay(1000);
}

String PadValue(int value)
{
    String returnVal;
    
    if(value < 10){
        returnVal = "0";
        returnVal += value;
    }
    else if (value < 100)
    {
        returnVal = String(value);
    }
    else if (value > 1900)
    {
      returnVal = String(value).substring(2);
    }
    else
    {
      returnVal = "99";
    }

    return returnVal;
}

String GetDateTime()
{
  DateTime now = rtc.now();
  String dateString;
  dateString = PadValue(now.month());
  dateString += "/";
  dateString += PadValue(now.day());
  dateString += "/";
  dateString += PadValue(now.year());
  dateString += " ";
  dateString += PadValue(now.hour());
  dateString += ":";
  dateString += PadValue(now.minute());
  
  return dateString;
}

void SetTime(float setYear, float setMonth, float setDay, float setHour, float setMinute, float setSecond)
{
  rtc.adjust(DateTime(setYear, setMonth, setDay, setHour, setMinute, setSecond));
}

void DisplayOutput(String displayString, int x, int y)
{
  display.setCursor(x,y);
  display.println(displayString);
}

void DisplayOutput(float displayFloat, int x, int y)
{
  display.setCursor(x,y);
  display.print(displayFloat);
}

void DisplayOutput(float displayFloat, String suffix, int x, int y)
{
  display.setCursor(x,y);
  display.print(displayFloat);
  display.println(suffix);
}

void DisplayOutput(int displayInt, int x, int y)
{
  display.setCursor(x,y);
  display.println(displayInt);
}

void CheckKeys()
{
  int keyPress = 0;
  
  if(!digitalRead(button1))
    keyPress = 1;
  else if (!digitalRead(button2))
    keyPress = 2;
  else if (!digitalRead(button3))
    keyPress = 3;
  else if (!digitalRead(button4))
    keyPress = 4;
    
  switch(keyPress)
  {
    case 1:
      DisplayMessage("Errors: " + String(errorCount));
      delay(2000);
    break;
    case 2:
      reqToken = 0;
    break;
    case 3:
      contrast += 1;
      display.setContrast(contrast);
      Serial.println(contrast);
    break;
    case 4:
      contrast -= 1;
      display.setContrast(contrast);
      Serial.println(contrast);
    break;
  }
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  keyPress = 0;
}
