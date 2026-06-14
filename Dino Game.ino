#include <LiquidCrystal.h>

// --- Game Timing Constants ---
#define INITIAL_DELAY 150     // Starting delay per frame (slowest speed)
#define MAX_SPEED_DELAY 40    // Maximum speed (fastest speed, smallest delay)

// --- Player Physics Constants ---
#define JUMP_FRAMES 3         // Number of frames the player stays in air
#define COLLISION_LEFT 1      // Left boundary for collision detection
#define COLLISION_RIGHT 2     // Right boundary for collision detection
#define PLAYER_GROUND 0       // Player on ground row
#define PLAYER_AIR 1          // Player in air row

// --- Hardware Pins ---
#define PIN_JUMP_BUTTON 13    // Button for jumping
#define PIN_SPEAKER 6         // Speaker for sound effects
#define PIN_RESET 7           // Reset pin for restarting game

// --- Background Music Frequencies ---
const float backgroundMelody[] = {
  293.66, 246.94, 293.66, 196.0, 293.66, 246.94, 
  293.66, 196.0, 329.63, 261.63
};

// --- LCD Setup ---
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(PIN_JUMP_BUTTON, INPUT);
  pinMode(PIN_SPEAKER, OUTPUT);
  lcd.begin(16, 2);
  randomSeed(analogRead(0)); // Random seed for obstacle positions
  loadCustomSprites();
  
  // --- START SCREEN ---
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("T-Rex Dino");
  lcd.setCursor(1, 1);
  lcd.print("Press to Start");
  
  // Wait until player presses jump button
  while(digitalRead(PIN_JUMP_BUTTON) == LOW) {
    delay(10); 
  }
  
  tone(PIN_SPEAKER, 600, 100); // Start sound
  lcd.clear();
  updateScore(0);
}

void loop() {
  bool playerRow;                  // Current player position (ground/air)
  int obstacle1Pos, obstacle2Pos;  // Positions of obstacles
  static bool isJumping = false;   // Jump state
  static byte jumpFrame = 0;       // Jump frame counter
  static unsigned int frameCount = 0; 
  static unsigned int score = 0;
  static int frameDelay = INITIAL_DELAY;

  // --- Game Acceleration ---
  if (frameCount % 50 == 0 && frameDelay > MAX_SPEED_DELAY) {
    frameDelay -= 2; // Gradually increase speed
  }

  // --- Background Music & Jump Input ---
  if (!isJumping) {
    tone(PIN_SPEAKER, backgroundMelody[(frameCount / 4) % 10]);
    isJumping = digitalRead(PIN_JUMP_BUTTON);
  }

  // --- Jump Physics ---
  if (isJumping && jumpFrame <= JUMP_FRAMES) {
    playerRow = PLAYER_AIR;
    jumpFrame++;
  } else {
    playerRow = PLAYER_GROUND;
    jumpFrame = 0;
    isJumping = false;
  }

  // --- Draw Player ---
  drawPlayer(0, playerRow);
  
  // --- Move Obstacles ---
  obstacle1Pos = moveObstacle1(playerRow);
  obstacle2Pos = moveObstacle2(playerRow);

  // --- Collision Detection ---
  if (playerRow == PLAYER_GROUND && 
     ((obstacle1Pos <= COLLISION_RIGHT && obstacle1Pos >= COLLISION_LEFT) || 
      (obstacle2Pos <= COLLISION_RIGHT && obstacle2Pos >= COLLISION_LEFT))) {
    triggerGameOver();
  }

  // --- Scoring ---
  if (isJumping && 
     ((obstacle1Pos <= (COLLISION_LEFT + 1) && obstacle1Pos >= COLLISION_LEFT) || 
      (obstacle2Pos < (COLLISION_LEFT + 1) && obstacle2Pos >= COLLISION_LEFT))) {
    score++;
    updateScore(score);
  }

  delay(frameDelay); 
  frameCount++;
}

// --- Load Custom LCD Sprites ---
void loadCustomSprites() {
  byte sprite0[8] = {0,0,0,0,4,6,7,7}; 
  byte sprite1[8] = {0,0,0,0,0,0,0,17};
  byte sprite2[8] = {7,13,15,15,15,15,31,31}; 
  byte sprite3[8] = {30,31,31,31,16,30,0,0};
  byte sprite4[8] = {3,1,0,0,0,0,0,0}; 
  byte sprite5[8] = {31,31,31,31,15,6,2,3};
  byte sprite6[8] = {31,30,30,28,24,16,16,24}; 
  byte sprite7[8] = {24,8,0,0,0,0,0,0};
  lcd.createChar(0, sprite0); lcd.createChar(1, sprite1); 
  lcd.createChar(2, sprite2); lcd.createChar(3, sprite3);
  lcd.createChar(4, sprite4); lcd.createChar(5, sprite5); 
  lcd.createChar(6, sprite6); lcd.createChar(7, sprite7);
}

// --- Draw Player on LCD ---
void drawPlayer(byte x, bool row) {
  if (row == PLAYER_GROUND) {
    lcd.setCursor(x, 0); lcd.write((unsigned char)0); lcd.write(1); lcd.write(2); lcd.write(3);
    lcd.setCursor(x, 1); lcd.write(4); lcd.write(5); lcd.write(6); lcd.write(7);
  } else {
    lcd.setCursor(x, 1); lcd.print("    ");
    lcd.setCursor(x, 0); lcd.write(4); lcd.write(5); lcd.write(6); lcd.write(7);
  }
}

// --- Obstacle 1 Movement ---
int moveObstacle1(bool isPlayerAirborne) {
  static int x = random(15, 20);
  lcd.setCursor(x, 1);
  if (x > 3) lcd.print(" ");
  else if (x <= 3 && !isPlayerAirborne) lcd.write((unsigned char)x + 4);
  if (x >= 0) { 
    x--; lcd.setCursor(x, 1); lcd.write(221); 
  } else x = random(15, 20);
  return x;
}

// --- Obstacle 2 Movement ---
int moveObstacle2(bool isPlayerAirborne) {
  static int x = random(25, 30);
  lcd.setCursor(x, 1);
  if (x > 3) lcd.print(" ");
  else if (x <= 3 && !isPlayerAirborne) lcd.write((unsigned char)x + 4);
  if (x >= 0) { 
    x--; lcd.setCursor(x, 1); lcd.write(243); 
  } else x = random(25, 30);
  return x;
}

// --- Update Score Display ---
void updateScore(int score) {
  lcd.setCursor(6, 0); lcd.print("Score:");
  if (score < 10) lcd.print("00"); 
  else if (score < 100) lcd.print("0");
  lcd.print(score);
}

// --- Game Over Screen ---
void triggerGameOver() {
  noTone(PIN_SPEAKER);
  while (true) {
    lcd.setCursor(4, 1); lcd.print(" Game Over ");
    delay(500);
    if (digitalRead(PIN_JUMP_BUTTON)) startNewGame();
    lcd.setCursor(4, 1); lcd.print("           ");
    delay(500);
    if (digitalRead(PIN_JUMP_BUTTON)) startNewGame();
  }
}

// --- Restart Game ---
void startNewGame() {
  pinMode(PIN_RESET, OUTPUT);
  digitalWrite(PIN_RESET, LOW); // Reset Arduino
}
