#include <LiquidCrystal.h> 
#include <Servo.h>

// Function prototypes
void moveMotor1();
void moveMotor2();

// Servo objects for motor1 (vertical) and motor2 (horizontal)
Servo motor1;
Servo motor2;

// LCD object for displaying motor positions
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16,2); 
  lcd.print("Motor1 "); 
  lcd.setCursor(0,1);
  lcd.print("Motor2 ");
  
  motor1.attach(9);   // Motor1 servo on pin 9
  motor2.attach(10);  // Motor2 servo on pin 10
  
  motor1.write(90);   // Start at center position
  motor2.write(90); 
}

void loop(){
  // Read light intensity from sensors
  int sensorTop = analogRead(A0);
  int sensorBottom = analogRead(A1);
  int sensorLeft = analogRead(A3);
  int sensorRight = analogRead(A4);

  // Calculate averages for comparison
  int avgTopBottom = (sensorTop + sensorBottom) / 2;
  int avgLeftRight = (sensorLeft + sensorRight) / 2;
  int avgTopLeft = (sensorTop + sensorLeft) / 2;
  int avgBottomRight = (sensorBottom + sensorRight) / 2;

  // Adjust motor1 (vertical axis)
  if (avgTopBottom > avgLeftRight || avgTopBottom < avgLeftRight) {
    moveMotor1(sensorTop, sensorBottom);
  }

  // Adjust motor2 (horizontal axis)
  if (avgTopLeft > avgBottomRight || avgTopLeft < avgBottomRight) {
    moveMotor2(sensorLeft, sensorRight);
  }

  delay(10); 
}

// Function to adjust motor1 (vertical tilt) based on top and bottom sensor values
void moveMotor1(int sensorTop, int sensorBottom){
  int posMotor1 = motor1.read();
    
  if(sensorTop < sensorBottom){
    posMotor1--;  // Tilt downward
  } else {
    posMotor1++;  // Tilt upward
  }
  
  motor1.write(posMotor1);
  lcd.setCursor(12,0);
  lcd.print(posMotor1);
}

// Function to adjust motor2 (horizontal rotation) based on left and right sensor values
void moveMotor2(int sensorLeft, int sensorRight){
  int posMotor2 = motor2.read();
  
  if(sensorLeft < sensorRight){ 
    posMotor2--;  // Rotate left
  } else {
    posMotor2++;  // Rotate right
  }
  
  motor2.write(posMotor2);
  lcd.setCursor(12,1);
  lcd.print(posMotor2);
}
