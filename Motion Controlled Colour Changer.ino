// Pin assignments
int buzzerPin = 3;
int pirPin = 7;
int redPin = 9;
int greenPin = 11;
int bluePin = 10;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

// Function to set RGB LED color
void RGB_color(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

// Function to fade smoothly between two colours
void fadeColor(int r1, int g1, int b1, int r2, int g2, int b2, int steps, int delayTime) {
  for (int i = 0; i <= steps; i++) {
    int r = r1 + (r2 - r1) * i / steps;
    int g = g1 + (g2 - g1) * i / steps;
    int b = b1 + (b2 - b1) * i / steps;
    RGB_color(r, g, b);
    delay(delayTime);
  }
}

void loop() {
  if (digitalRead(pirPin) == HIGH) {
    digitalWrite(buzzerPin, HIGH);

    // Cycle smoothly through multiple colours
    fadeColor(255, 0, 0, 0, 255, 0, 50, 20);   // Red → Green
    fadeColor(0, 255, 0, 0, 0, 255, 50, 20);   // Green → Blue
    fadeColor(0, 0, 255, 255, 255, 0, 50, 20); // Blue → Yellow
    fadeColor(255, 255, 0, 255, 0, 255, 50, 20); // Yellow → Magenta
    fadeColor(255, 0, 255, 0, 255, 255, 50, 20); // Magenta → Cyan
    fadeColor(0, 255, 255, 255, 0, 0, 50, 20);   // Cyan → Red

    while (digitalRead(pirPin) == HIGH); // Wait until motion stops
  } else {
    digitalWrite(buzzerPin, LOW);
    RGB_color(0, 0, 0); // LED OFF when no motion
  }
}
