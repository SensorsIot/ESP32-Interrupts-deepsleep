#include <LiquidCrystal_I2C.h>

int count = 0;
unsigned long lastEntry;
int stat = 0;

#define INPUTPIN 15
#define OSCPIN 17

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup() {
  // initialize LCD
  lcd.init();
  Serial.begin(115200);
  // turn on LCD backlight
  lcd.backlight();
  pinMode(INPUTPIN, INPUT_PULLUP);
  pinMode(OSCPIN, OUTPUT);
}

void loop() {
  display(count);
  count = 0;
  lastEntry = millis();
  while (millis() < lastEntry + 1000) {

    bool inputSignal = (digitalRead(INPUTPIN));
    switch (stat) {
      case 0:
        if (inputSignal) {
          count++;
            digitalWrite(OSCPIN, 1);
  digitalWrite(OSCPIN, 0);
          stat = 1;
        }
        break;

      case 1:
        if (!inputSignal) stat = 0;
        break;

      default: 
        break;
    }
  }
}

void display(int val) {
  lcd.clear();
  // set cursor to first column,first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Frequency=");

  // set cursor to first column, second row
  lcd.setCursor(11, 0);
  lcd.print(String(val, DEC));
}
