#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define RED 0x1 // Colors for the LCD display
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

double analogPin = A0;
double val;
double flow;
double offset;
double kPlus1;
double kPlus2;
double kPlus3;
double kMinus1;
double kMinus2;
double kMinus3;
double kPlusAverage;
double kMinusAverage;
unsigned long timeStart;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  Serial.println("CALIBRATING, DON'T BREATHE");
  for (int n = 1; n <= 20; n++) {
    offset += analogRead(analogPin);
  }
  offset = offset / 20;
  offset = offset * 5 / 1024;
  Serial.println("CALIBRATION DONE!");
  lcd.begin(16, 2); // Set up LCD screen
  lcd.clear();
  lcd.setBacklight(GREEN);
  delay(2000); // Wait until the board fully wakes up.
}

void loop() {
  uint8_t buttons = lcd.readButtons();
  lcd.setCursor(0, 0);
  lcd.print("PRESS TO: "); // Print current temperature
  lcd.setCursor(0, 1);
  lcd.print("FIND K: ");
  if (buttons) {
    if (buttons & BUTTON_SELECT) {
      findK(kPlus1, kMinus1);
      delay(3500);
      findK(kPlus2, kMinus2);
      delay(3500);
      findK(kPlus3, kMinus3);
      delay(3500);
      kPlusAverage = (kPlus1 + kPlus2 + kPlus3) / 3;
      kMinusAverage = (kMinus1 + kMinus2 + kMinus3) / 3;
      Serial.print("K+: ");
      Serial.println(kPlusAverage);
      Serial.print("K-: ");
      Serial.println(kMinusAverage);
    }
  }
}

void findK(double &kPlus, double &kMinus) {
  double positiveVoltage = 0;
  double negativeVoltage = 0;
  int posCount = 0;
  int negCount = 0;
  lcd.clear();
  for (int i = 3; i >= 1; i--) {
    lcd.setCursor(0, 0);
    lcd.print(i);
    delay(1000);
    lcd.clear();
  }
  lcd.print("BREATHE!");
  while ((millis() - timeStart) <= 3000) {
    delay(10);
    val = analogRead(analogPin);
    flow = val * 5 / 1024;
    if (flow >= (offset + 0.02)) {
      positiveVoltage += ( (flow - offset) * 10.2);
    }
    else if (flow <= (offset - 0.02)) { // Maybe change the range a little bit
      negativeVoltage += ( (offset - flow) * 10.2);
    } 
  }
  kPlus = 3 * 60000 / positiveVoltage;
  kMinus = 3 * 60000 / negativeVoltage;
}
