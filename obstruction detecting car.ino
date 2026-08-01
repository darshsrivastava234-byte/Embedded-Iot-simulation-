// --- Sensor Pin Setup ---
const int trigSensor = 7;
const int echoSensor = 2;

// --- Motor Driver Pins (L293D) ---
const int motorEnable = 13;   // Enable bridge
const int leftMotorFwd = 11;  // Left motor forward
const int leftMotorRev = 9;   // Left motor reverse
const int rightMotorFwd = 5;  // Right motor forward
const int rightMotorRev = 3;  // Right motor reverse

// --- Control Parameters ---
const int safeDistance = 30;  // Distance threshold in cm

void setup() {
  Serial.begin(115200);

  // Motor pin configuration
  pinMode(motorEnable, OUTPUT);
  pinMode(leftMotorFwd, OUTPUT);
  pinMode(leftMotorRev, OUTPUT);
  pinMode(rightMotorFwd, OUTPUT);
  pinMode(rightMotorRev, OUTPUT);

  digitalWrite(motorEnable, HIGH); // Activate motor driver

  // Ultrasonic sensor configuration
  pinMode(trigSensor, OUTPUT);
  pinMode(echoSensor, INPUT);
  digitalWrite(trigSensor, LOW);
  
  Serial.println("System Ready...");
}

void loop() {
  int measuredDistance = readDistance();

  if (measuredDistance > 0 && measuredDistance < safeDistance) {
    // Obstacle detected
    Serial.print("Object detected at: ");
    Serial.println(measuredDistance);
    
    obstacleAvoidance();
  } else {
    // Path clear
    driveForward();
  }
  
  delay(50); // Stability delay
}

// --- Movement Functions ---

void driveForward() {
  digitalWrite(leftMotorFwd, HIGH);
  digitalWrite(leftMotorRev, LOW);
  digitalWrite(rightMotorFwd, HIGH);
  digitalWrite(rightMotorRev, LOW);
}

void driveBackward(int duration) {
  digitalWrite(leftMotorFwd, LOW);
  digitalWrite(leftMotorRev, HIGH);
  digitalWrite(rightMotorFwd, LOW);
  digitalWrite(rightMotorRev, HIGH);
  delay(duration);
}

void rotateRight(int duration) {
  digitalWrite(leftMotorFwd, HIGH);
  digitalWrite(leftMotorRev, LOW);
  digitalWrite(rightMotorFwd, LOW);
  digitalWrite(rightMotorRev, HIGH); // Counter rotation
  delay(duration);
}

void haltMotion(int duration) {
  digitalWrite(leftMotorFwd, LOW);
  digitalWrite(leftMotorRev, LOW);
  digitalWrite(rightMotorFwd, LOW);
  digitalWrite(rightMotorRev, LOW);
  delay(duration);
}

// --- Logic Functions ---

void obstacleAvoidance() {
  haltMotion(500);
  driveBackward(800);
  haltMotion(200);
  
  // Rotate to search for clear path
  rotateRight(600); 
  haltMotion(200);
}

int readDistance() {
  digitalWrite(trigSensor, LOW);
  delayMicroseconds(2);
  digitalWrite(trigSensor, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigSensor, LOW);

  long echoTime = pulseIn(echoSensor, HIGH, 30000); 
  
  if (echoTime == 0) return 400; // No obstacle detected
  return echoTime / 58; // Convert to cm
}
