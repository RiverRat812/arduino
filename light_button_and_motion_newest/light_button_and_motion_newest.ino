const int lightPin =  9;      // the number of the LED pin

// Pin Array and Pin Count, 2 is PIR and 3 is switch currently
int             inputPin[] = { 2, 3 };
int             pinCount = 2;
int             pirState = LOW;
int             val = 0;
unsigned long   timer = 0;
unsigned long previousMillis = 0;
const long interval = 300000;

void setup()
{
  // Serial.begin(9600);
  pinMode(lightPin, OUTPUT);
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(inputPin[thisPin], INPUT);
  }
}

void loop()
{
  unsigned long currentMillis = millis();
  // Serial.println(timer);
    if (currentMillis - previousMillis >= interval) {
    digitalWrite(lightPin, LOW); // turn LED OFF
    timer = 0;
    previousMillis = currentMillis;
  } else {
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      val = digitalRead(inputPin[thisPin]);
     // Serial.println(val);
      // value
      if (val == HIGH) {
        if (pirState == LOW) {
          digitalWrite(lightPin, HIGH); // turn LED ON
          timer = 0;
          pirState = HIGH;
        }
      } else {
        if (pirState == HIGH) {
          pirState = LOW;
        }
        timer++;
      }
    }
  }
}

