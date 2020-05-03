#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

volatile int count = 0;
volatile int frequency;
hw_timer_t * timer = NULL;

#define INPUTPIN 15
#define OSCPIN 17

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR isr() {
  portENTER_CRITICAL(&synch);
  count++;
  digitalWrite(OSCPIN, 1);
  digitalWrite(OSCPIN, 0);
  portEXIT_CRITICAL(&synch);
}

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  frequency=count;
  count=0;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

void setup() {
  WiFi.mode(WIFI_OFF);
  btStop();
  lcd.init();
  Serial.begin(115200);
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());
  // turn on LCD backlight
  lcd.backlight();
  pinMode(INPUTPIN, INPUT_PULLUP);
  pinMode(OSCPIN, OUTPUT);
  attachInterrupt(INPUTPIN, isr, RISING);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void loop() {
  display(0, "frequency=", frequency);
  delay(1000);
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
