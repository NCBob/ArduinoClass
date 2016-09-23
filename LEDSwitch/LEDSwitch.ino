bool ledState = false;
const int ledPin = 5;
const int switchPin = 8;

void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  digitalWrite(ledPin, ledState);
}

void loop() 
{
  if(!digitalRead(switchPin))
  {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
  
  delay(500);
}


