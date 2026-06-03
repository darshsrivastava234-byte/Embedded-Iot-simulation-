#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

// Button pins
#define buttonBJP 7
#define buttonINC 8
#define buttonAAP 4
#define buttonOTH 12
#define buttonResult 13

// LED pins
#define ledGreen 9
#define ledRed 10

// Vote counters
int votesBJP = 0;
int votesINC = 0;
int votesAAP = 0;
int votesOTH = 0;

void setup() {
  // Configure buttons with internal pull-ups
  pinMode(buttonBJP, INPUT_PULLUP);
  pinMode(buttonINC, INPUT_PULLUP);
  pinMode(buttonAAP, INPUT_PULLUP);
  pinMode(buttonOTH, INPUT_PULLUP);
  pinMode(buttonResult, INPUT_PULLUP);

  // LEDs
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);

  // LCD setup
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("    EVM ");
  lcd.setCursor(0,1);
  lcd.print("Circuit design ");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0,0); lcd.print("BJP");
  lcd.setCursor(4,0); lcd.print("INC");
  lcd.setCursor(8,0); lcd.print("AAP");
  lcd.setCursor(12,0); lcd.print("OTH");
}

void loop() {
  // Update vote counts on LCD
  lcd.setCursor(1,1); lcd.print(votesBJP);
  lcd.setCursor(5,1); lcd.print(votesINC);
  lcd.setCursor(9,1); lcd.print(votesAAP);
  lcd.setCursor(13,1); lcd.print(votesOTH);

  // BJP button
  if(digitalRead(buttonBJP) == LOW) {
    votesBJP++;
    digitalWrite(ledGreen, HIGH);
    delay(50); // debounce
    digitalWrite(ledGreen, LOW);
    while(digitalRead(buttonBJP) == LOW); // wait for release
  }

  // INC button
  if(digitalRead(buttonINC) == LOW) {
    votesINC++;
    digitalWrite(ledGreen, HIGH);
    delay(50);
    digitalWrite(ledGreen, LOW);
    while(digitalRead(buttonINC) == LOW);
  }

  // AAP button
  if(digitalRead(buttonAAP) == LOW) {
    votesAAP++;
    digitalWrite(ledGreen, HIGH);
    delay(50);
    digitalWrite(ledGreen, LOW);
    while(digitalRead(buttonAAP) == LOW);
  }

  // OTH button
  if(digitalRead(buttonOTH) == LOW) {
    votesOTH++;
    digitalWrite(ledGreen, HIGH);
    delay(50);
    digitalWrite(ledGreen, LOW);
    while(digitalRead(buttonOTH) == LOW);
  }

  // Result button
  if(digitalRead(buttonResult) == LOW) {
    digitalWrite(ledRed, HIGH);
    int totalVotes = votesBJP + votesINC + votesAAP + votesOTH;

    lcd.clear();
    if(totalVotes == 0) {
      lcd.print("No Voting...");
    } else {
      if(votesBJP > votesINC && votesBJP > votesAAP && votesBJP > votesOTH) lcd.print("BJP Wins");
      else if(votesINC > votesBJP && votesINC > votesAAP && votesINC > votesOTH) lcd.print("INC Wins");
      else if(votesAAP > votesBJP && votesAAP > votesINC && votesAAP > votesOTH) lcd.print("AAP Wins");
      else if(votesOTH > votesBJP && votesOTH > votesINC && votesOTH > votesAAP) lcd.print("OTH Wins");
      else lcd.print("Tie / No Result");
    }

    delay(3000); // show result briefly
    lcd.clear();

    // Reset votes
    votesBJP = 0;
    votesINC = 0;
    votesAAP = 0;
    votesOTH = 0;

    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, LOW);

    // Redisplay party names
    lcd.setCursor(0,0); lcd.print("BJP");
    lcd.setCursor(4,0); lcd.print("INC");
    lcd.setCursor(8,0); lcd.print("AAP");
    lcd.setCursor(12,0); lcd.print("OTH");
  }
}
