#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Single LCD
LiquidCrystal_I2C Monitor(0x27, 16, 2);   // One LCD

// Sensors
int Temp_sensor = A1;       // Temperature sensor
int LDR = A2;               // Light sensor
int Moisture_sensor = A3;   // Soil moisture sensor

// Relays
int relay1 = 2;   // Bulb
int relay2 = 3;   // Fan
int relay3 = 4;   // Pump

// Servos
Servo servo1;     // Vent control
Servo servo2;     // Shade control

// Buzzer
int buzzerPin = 12;

// Timing variables
unsigned long event_LDR = 250UL;
unsigned long event_Temp = 370UL;
unsigned long event_Moisture = 500UL;

unsigned long prevTime_LDR = 0;
unsigned long prevTime_Temp = 0;
unsigned long prevTime_Moisture = 0;

void setup() {
  Monitor.init();
  Monitor.clear();
  Monitor.backlight();

  Serial.begin(9600);

  pinMode(Temp_sensor, INPUT);
  pinMode(LDR, INPUT);
  pinMode(Moisture_sensor, INPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  // Attach servos
  servo1.attach(9);
  servo2.attach(10);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - prevTime_LDR >= event_LDR) {
    LightSensor();
    prevTime_LDR = currentTime;
  }

  if (currentTime - prevTime_Temp >= event_Temp) {
    TempControl();
    prevTime_Temp = currentTime;
  }

  if (currentTime - prevTime_Moisture >= event_Moisture) {
    MoistureControl();
    prevTime_Moisture = currentTime;
  }
}

// Temperature + Servo1 + Buzzer
void TempControl() {
  int T_value = analogRead(Temp_sensor);
  float voltage = (T_value * 5.0) / 1023.0;
  float tempC = (voltage - 0.5) * 100; // adjust formula for your sensor

  Monitor.setCursor(0, 0);
  Monitor.print("Temp:");
  Monitor.print(tempC);
  Monitor.print("C   ");

  if (tempC >= 35) {
    digitalWrite(relay2, HIGH);   // Fan ON
    servo1.write(90);             // Vent open
  } else {
    digitalWrite(relay2, LOW);    // Fan OFF
    servo1.write(0);              // Vent closed
  }

  // 🔔 Buzzer alert if temp > 40°C
  if (tempC > 40) {
    digitalWrite(buzzerPin, HIGH);   // Buzzer ON
  } else {
    digitalWrite(buzzerPin, LOW);    // Buzzer OFF
  }
}

// Light + Servo2 control
void LightSensor() {
  int LDR_value = analogRead(LDR);
  float intensity = (float)LDR_value / 1023.0;

  Monitor.setCursor(0, 1);
  Monitor.print("Light:");
  Monitor.print(intensity);
  Monitor.print("   ");

  if (intensity < 0.2) {
    digitalWrite(relay1, HIGH);   // Bulb ON
    servo2.write(0);              // Shade closed
  } else {
    digitalWrite(relay1, LOW);    // Bulb OFF
    servo2.write(90);             // Shade open
  }
}

// Soil moisture + Pump control (automatic only)
void MoistureControl() {
  int M_value = analogRead(Moisture_sensor);
  float M_percentage = (M_value / 1023.0) * 100;

  Serial.print("Moisture: ");
  Serial.print(M_percentage);
  Serial.println("%");

  if (M_percentage < 40) {
    digitalWrite(relay3, HIGH);   // Pump ON
    Serial.println("Watering Plants");
  } else {
    digitalWrite(relay3, LOW);    // Pump OFF
    Serial.println("No Water Needed");
  }
}
