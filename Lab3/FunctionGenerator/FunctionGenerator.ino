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

float angle = 0; //Variables used to generate the functions
int magnitude = 0;
int triangle;
float count = -255;


void setup() { //Turn on board, display message with directions
  Serial.begin(9600);
  lcd.begin(16, 2);
  setPwmFrequency(10, 1);
  lcd.clear();
  lcd.print("UP FOR TRIANGLE");
  lcd.setCursor(0, 1);
  lcd.print("DOWN FOR SINE");
  lcd.setBacklight(VIOLET);
}

void loop() {

  uint8_t buttons = lcd.readButtons(); //Sine wave when the down button is pressed
  if (buttons) {
    if (buttons & BUTTON_DOWN) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SINE");
      while (1) {
        generateSin(1000);
      }
    }
    if (buttons & BUTTON_UP) { //Generate a triangle wave the the up button is pressed
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("TRIANGLE");
      generateTriangle(100);
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

void generateSin(float rate) { //Function used to generate a sine wave
  for (float i {0}; i <= (rate / 10); i++) { //In a loop because is periodic
    angle = 2 * M_PI * (i / (rate / 10)); //Find the angle based on the time point
    magnitude = (int) (127 * (sin(angle) + 1)); //Get the magnitude of wavefrom from angle
    analogWrite(10, magnitude); //Display the sine value
    delay(rate / 1000); //Sampling rate delay
  }
}

void generateTriangle(float rate) { //Function used to generate a triangle wave
  while (1) { //Increments/decrements by one every time point
    if (count >= 255) { //Max value of oscilloscope
      count = -255;
    }
    triangle = abs(count); //All positive values for triangle wave
    count = count + 512 / (rate / 0.03); //Defines slope
    analogWrite(10, triangle); //Display trangle value
    delay(rate / 1000); //Sampling rate delay
  }
}
