int moistureValue = 0;

// Motor driver pins
int motorIn1 = 7;   
int motorIn2 = 4;   
int motorEnable = 10; 

// LED pins
int dryLed = 8;    
int wetLed = 9;    

void setup() {
  pinMode(A0, INPUT);          
  Serial.begin(9600);

  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnable, OUTPUT);

  pinMode(dryLed, OUTPUT);
  pinMode(wetLed, OUTPUT);

  digitalWrite(motorEnable, HIGH); 
}

void loop() {
  moistureValue = analogRead(A0);
  Serial.println(moistureValue);

  if (moistureValue < 100) {
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);

    digitalWrite(dryLed, HIGH);
    delay(200);
    digitalWrite(dryLed, LOW);
    delay(200);

    digitalWrite(wetLed, LOW);

  } else {
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);

    digitalWrite(wetLed, HIGH);
    delay(200);
    digitalWrite(wetLed, LOW);
    delay(200);

    digitalWrite(dryLed, LOW);
  }
}
