//FARC Weather Station Program
//This runs on the Arduino that has all the sensors connected to it and is out on location
//Bob Gusek
//Arduino Class
//2/11/2016

//Defines
#define DEBUG
#define DEBUGRAW

void setup() {
  Serial.begin(9600);
  #ifdef DEBUG
    Serial.println("DEBUG MODE");  
  #endif
}

void loop() {
  #ifdef DEBUG
  #else
    #ifdef DEBUGRAW
    #endif
  #endif
}

