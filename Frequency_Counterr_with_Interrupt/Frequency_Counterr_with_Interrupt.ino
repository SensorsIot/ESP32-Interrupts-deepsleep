#include <LiquidCrystal_I2C.h>
//#include <WiFi.h>

volatile int count = 0;
unsigned long lastEntry, lastDisplay;
int frequency, tst;

#define INPUTPIN 15
#define OSCPIN 17

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR isr() {
  portENTER_CRITICAL(&synch);
  digitalWrite(OSCPIN, 1);
  digitalWrite(OSCPIN, 0);
  count++;
  portEXIT_CRITICAL(&synch);
}


void setup() {
  // initialize LCD
  lcd.init();
//WiFi.mode(WIFI_OFF);
 // btStop();
  Serial.begin(115200);
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());
  // turn on LCD backlight
  lcd.backlight();
  pinMode(INPUTPIN, INPUT_PULLUP);
  pinMode(OSCPIN, OUTPUT);
  attachInterrupt(INPUTPIN, isr, RISING);
  lastEntry = millis();
}

void loop() {
  if (millis() > lastEntry + 1000) {
    frequency = count;
    display(0, "frequency=", frequency);
    lastEntry = millis();
    count = 0;
  }
  if (millis() > lastDisplay + 500) {
    lastDisplay = millis();
    display(2, "Other=", tst++);
  }
}

void display(int line, String tex, int val) {
  // set cursor to first column,first row
 
  lcd.setCursor(0, line);
  // print message
  lcd.print(tex);
  
 lcd.setCursor(11, line);
  // clear
  lcd.print("          ");
  lcd.setCursor(11, line);
  // set cursor to first column, second row
  lcd.print(String(val, DEC));
}
