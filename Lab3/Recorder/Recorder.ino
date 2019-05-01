#include <Wire.h> //Libraries needed to run the code
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define RED 0x1 //Colors for the LCD display
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

int samples[204];
int maxSamples = 204;


void setup() { //Variables used to generate the functions
  Serial.begin(9600);
  lcd.begin(16, 2);
  setPwmFrequency(10, 1);
  lcd.clear();
  lcd.print("PRESS SELECT");
  lcd.setCursor(0, 1);
  lcd.print("TO START");
  lcd.setBacklight(TEAL);
}

void loop() { //Turn on board, display message with directions
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    if (buttons & BUTTON_SELECT) { //When select is pressed ...
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RECORDING");
      for (int i{0}; i < 204; i ++) {
        samples[i] = analogRead(A0); //Read max input allowed
        delay(1);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("UP = ONCE");
      lcd.setCursor(0, 1);
      lcd.print("DOWN = CONT");
    }

    if (buttons & BUTTON_UP) { //When up is pressed ...
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ONE REPLAY");
      for (int i{0}; i < 204; i ++) {
        analogWrite(10, (int) (samples[i] / 4)); //Display sample once
        delay(1);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("UP = ONCE");
      lcd.setCursor(0, 1);
      lcd.print("DOWN = CONT");
    }

    if (buttons & BUTTON_DOWN) { //When down is pressed ...
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PRESS SELECT");
      lcd.setCursor(0, 1);
      lcd.print("TO STOP");
      while (1) {
        for (int i{0}; i < 204; i ++) {
          analogWrite(10, (int) (samples[i]/4)); //Display sample until
          delay(1);
        }
        if (buttons & BUTTON_SELECT) break; //Select is pressed
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("UP = ONCE");
      lcd.setCursor(0, 1);
      lcd.print("DOWN = CONT");
    }
  }
}


void setPwmFrequency(int pin, int divisor) { //Used to set pulse wave modulation frequency,
  //or how quickly Arduino can change its state from high to low
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
