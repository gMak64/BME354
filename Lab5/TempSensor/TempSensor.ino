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

void setup(){
 Serial.begin(9600);
 pinMode(LED_BUILTIN, OUTPUT);
 piinMode(2, OUTPUT);
 lcd.begin(16, 2);
 lcd.clear();
 lcd.setBacklight(BLUE);
 delay(2000); // wait until the board fully wakes up.
}

void loop(){
 float adcValue = analogRead(A1); //simulating analogRead()
 float temperatureC = (adcValue) * (0.154) - (28.36); //
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.println(temperatureC);
 delay(1000);
}
