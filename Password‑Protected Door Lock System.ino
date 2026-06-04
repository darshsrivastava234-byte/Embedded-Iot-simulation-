#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define PASSWORD_LENGTH 5

Servo doorServo;
LiquidCrystal lcdDisplay(A0, A1, A2, A3, A4, A5);

int servoPosition = 0;

char enteredPassword[PASSWORD_LENGTH];
char masterPassword[PASSWORD_LENGTH] = "1001";
byte enteredCount = 0;

bool isDoorOpen = false;
char pressedKey;

/*--- Keypad Setup ---*/
const byte ROWS = 4;
const byte COLS = 4;
char keypadLayout[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad keypad(makeKeymap(keypadLayout), rowPins, colPins, ROWS, COLS);

/*--- Main Action ---*/
void setup()
{
  doorServo.attach(9, 2000, 2400);
  closeDoor();
  lcdDisplay.begin(16, 2);
  lcdDisplay.print("Protected Door");
  showLoading("Loading");
  lcdDisplay.clear();
}

void loop()
{
  if (isDoorOpen)
  {
    pressedKey = keypad.getKey();
    if (pressedKey == '#')
    {
      lcdDisplay.clear();
      closeDoor();
      lcdDisplay.print("Door is closed");
      delay(3000);
      isDoorOpen = false;
    }
  }
  else
  {
    requestPassword();
  }
}

void showLoading(char message[]) {
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print(message);

  for (int i = 0; i < 9; i++) {
    delay(1000);
    lcdDisplay.print(".");
  }
}

void clearPassword()
{
  while (enteredCount != 0)
  { 
    enteredPassword[enteredCount--] = 0;
  }
}

void closeDoor()
{
  for (servoPosition = 90; servoPosition >= 0; servoPosition -= 10) { 
    doorServo.write(servoPosition);
  }
}

void openDoor()
{
  for (servoPosition = 0; servoPosition <= 90; servoPosition += 10) {
    doorServo.write(servoPosition);  
  }
}

void requestPassword()
{
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("Enter Password");
  
  pressedKey = keypad.getKey();
  if (pressedKey)
  {
    enteredPassword[enteredCount] = pressedKey;
    lcdDisplay.setCursor(enteredCount, 1);
    lcdDisplay.print(enteredPassword[enteredCount]);
    enteredCount++;
  }

  if (enteredCount == PASSWORD_LENGTH - 1)
  {
    if (!strcmp(enteredPassword, masterPassword))
    {
      lcdDisplay.clear();
      openDoor();
      lcdDisplay.print(" Door is Open ");
      isDoorOpen = true;
      delay(5000);
      showLoading("Waiting");
      lcdDisplay.clear();
      lcdDisplay.print(" Time is up! ");
      delay(1000);
      closeDoor();
      isDoorOpen = false;      
    }
    else
    {
      lcdDisplay.clear();
      lcdDisplay.print(" Wrong Password ");
      isDoorOpen = false;
    }
    delay(1000);
    lcdDisplay.clear();
    clearPassword();
  }
}
