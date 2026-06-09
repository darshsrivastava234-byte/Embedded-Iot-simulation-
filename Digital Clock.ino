// Include the LCD library
#include <LiquidCrystal.h>

// Initialize LCD interface pins
LiquidCrystal lcd(2,3,4,5,6,7);

// Time and date variables
int seconds = 0, minutes = 0, hours = 0;
int day = 1, month = 1;
int modeSelector = 0;   // Used to switch between editing hours/minutes/day/month
int buttonStateHour = 1, buttonStateMinute = 1, buttonStateDay = 1;
int adjustDirection = 0; // +1 or -1 depending on button pressed

// Month display characters (Oct=A, Nov=B, Dec=C)
char monthSymbols[13] = {' ','1','2','3','4','5','6','7','8','9','A','B','C'};

// Days in each month (Feb=29 for leap year assumption)
int daysInMonth[13] = {0,31,29,31,30,31,30,31,31,30,31,30,31};

// Custom LCD segments for big digits
byte bar1[8] = {B11100,B11110,B11110,B11110,B11110,B11110,B11110,B11100};
byte bar2[8] = {B00111,B01111,B01111,B01111,B01111,B01111,B01111,B00111};
byte bar3[8] = {B11111,B11111,B00000,B00000,B00000,B00000,B11111,B11111};
byte bar4[8] = {B11110,B11100,B00000,B00000,B00000,B00000,B11000,B11100};
byte bar5[8] = {B01111,B00111,B00000,B00000,B00000,B00000,B00011,B00111};
byte bar6[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B11111,B11111};
byte bar7[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00111,B01111};
byte bar8[8] = {B11111,B11111,B00000,B00000,B00000,B00000,B00000,B00000};

void setup() {
  // Load custom characters into LCD
  lcd.createChar(1,bar1);
  lcd.createChar(2,bar2);
  lcd.createChar(3,bar3);
  lcd.createChar(4,bar4);
  lcd.createChar(5,bar5);
  lcd.createChar(6,bar6);
  lcd.createChar(7,bar7);
  lcd.createChar(8,bar8);

  lcd.begin(16, 2);

  // Input buttons for setting time/date
  pinMode(8,INPUT);   // Mode selector (shift)
  pinMode(9,INPUT);   // Increment (add)
  pinMode(10,INPUT);  // Decrement (sub)
}

// --- Digit drawing functions (unchanged) ---
void custom0(int col){ lcd.setCursor(col,0); lcd.write(2); lcd.write(8); lcd.write(1); lcd.setCursor(col,1); lcd.write(2); lcd.write(6); lcd.write(1);}
void custom1(int col){ lcd.setCursor(col,0); lcd.write(32); lcd.write(32); lcd.write(1); lcd.setCursor(col,1); lcd.write(32); lcd.write(32); lcd.write(1);}
void custom2(int col){ lcd.setCursor(col,0); lcd.write(5); lcd.write(3); lcd.write(1); lcd.setCursor(col,1); lcd.write(2); lcd.write(6); lcd.write(6);}
void custom3(int col){ lcd.setCursor(col,0); lcd.write(5); lcd.write(3); lcd.write(1); lcd.setCursor(col,1); lcd.write(7); lcd.write(6); lcd.write(1);}
void custom4(int col){ lcd.setCursor(col,0); lcd.write(2); lcd.write(6); lcd.write(1); lcd.setCursor(col,1); lcd.write(32); lcd.write(32); lcd.write(1);}
void custom5(int col){ lcd.setCursor(col,0); lcd.write(2); lcd.write(3); lcd.write(4); lcd.setCursor(col,1); lcd.write(7); lcd.write(6); lcd.write(1);}
void custom6(int col){ lcd.setCursor(col,0); lcd.write(2); lcd.write(3); lcd.write(4); lcd.setCursor(col,1); lcd.write(2); lcd.write(6); lcd.write(1);}
void custom7(int col){ lcd.setCursor(col,0); lcd.write(8); lcd.write(8); lcd.write(1); lcd.setCursor(col,1); lcd.write(32); lcd.write(32); lcd.write(1);}
void custom8(int col){ lcd.setCursor(col,0); lcd.write(2); lcd.write(3); lcd.write(1); lcd.setCursor(col,1); lcd.write(2); lcd.write(6); lcd.write(1);}
void custom9(int col){ lcd.setCursor(col,0); lcd.write(2); lcd.write(3); lcd.write(1); lcd.setCursor(col,1); lcd.write(7); lcd.write(6); lcd.write(1);}

void printNumber(int value, int col) {
  switch(value){
    case 0: custom0(col); break;
    case 1: custom1(col); break;
    case 2: custom2(col); break;
    case 3: custom3(col); break;
    case 4: custom4(col); break;
    case 5: custom5(col); break;
    case 6: custom6(col); break;
    case 7: custom7(col); break;
    case 8: custom8(col); break;
    case 9: custom9(col); break;
  }
}

void loop() {
  // Handle mode selection and increment/decrement
  if(digitalRead(8) && buttonStateHour==1){
    modeSelector++;
    buttonStateHour=0;
    modeSelector%=5;
  } else if(!digitalRead(8) && buttonStateHour==0){
    buttonStateHour=1;
  }

  if(digitalRead(9) && buttonStateMinute==1){
    adjustDirection=1;
    buttonStateMinute=0;
  } else if(!digitalRead(9) && buttonStateMinute==0){
    buttonStateMinute=1;
  }

  if(digitalRead(10) && buttonStateDay==1){
    adjustDirection=-1;
    buttonStateDay=0;
  } else if(!digitalRead(10) && buttonStateDay==0){
    buttonStateDay=1;
  }

  // Adjust time/date depending on mode
  switch(modeSelector){
    case 1: hours+=adjustDirection; if(hours>23) hours=0; if(hours<0) hours=23; break;
    case 2: minutes+=adjustDirection; if(minutes>59) minutes=0; if(minutes<0) minutes=59; break;
    case 3: day+=adjustDirection; if(day>daysInMonth[month]) day=daysInMonth[month]; if(day<1) day=1; break;
    case 4: month+=adjustDirection; if(month>12) month=1; if(month<1) month=12; if(day>daysInMonth[month]) day=1; break;
  }
  adjustDirection=0;

  // Time progression
  if(seconds>59){ seconds=0; minutes++; }
  if(minutes>59){ minutes=0; hours++; }
  if(hours>23){ hours=0; day++; }
  if(day>daysInMonth[month]){ day=1; month++; }
  if(month>12){ month=1; }

  // Display hours
  printNumber(hours/10,0);
  printNumber(hours%10,3);

  // Display minutes
  printNumber(minutes/10,7);
  printNumber(minutes%10,10);

  // Display seconds (simple digits)
  lcd.setCursor(14,0);
  lcd.print(seconds/10);
  lcd.print(seconds%10);

  // Display date
  lcd.setCursor(13,1);
  lcd.print(monthSymbols[month]);
  lcd.print(day/10);
  lcd.print(day%10);

  // Blinking colon
  if(modeSelector==0){
    seconds++;
    lcd.setCursor(6,0); lcd.print(" ");
    lcd.setCursor(6,1); lcd.print(" ");
    lcd.setCursor(13,0); lcd.print(" ");
    delay(500);
    lcd.setCursor(6,0); lcd.print(".");
    lcd.setCursor(6,1); lcd.print(".");
    lcd.setCursor(13,0); lcd.print(":");
    delay(500);
  }
}
