  #include <Wire.h> //Libraries needed to run the code
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

float setPoint = 36.8; // Temperature parameters
float hysteris = 0.5;


void setup() {
  Serial.begin(9600); 
  pinMode(2, OUTPUT); // Speaker pin
  pinMode(7, OUTPUT); // Heater pin
  digitalWrite(7, LOW); // Heater is initially off
  lcd.begin(16, 2); // Set up LCD screen
  lcd.clear();
  lcd.setBacklight(GREEN);
  delay(2000); // Qait until the board fully wakes up.
}

void loop() {
  float lowTemp = setPoint - hysteris; // Low boundary
  float highTemp = setPoint + hysteris; // High boundary

  buttonPress(); // Used to control hysteris and setpoint
  tone(2,0);

  float adcValue = analogRead(A1); // Read value from thermistor
  float temperatureC = (adcValue) * (0.154) - (28.36); // Convert value to tempeature 
  lcd.clear();
  lcd.setBacklight(GREEN);
  lcd.setCursor(0, 0);
  lcd.print("Current: "); // Print current temperature
  lcd.print(temperatureC, 2);
  lcd.setCursor(0, 1);
  lcd.print("Set Point: "); // Print setpoint
  lcd.print(setPoint, 2);
  delay(1000);

// If current temperature is below the setpoint
  while (temperatureC < lowTemp) { 
    buttonPress();
    tone(2, 880); // Play alarm tone
    digitalWrite(7, HIGH); // Turn on heater
    lcd.clear();
    lcd.setBacklight(BLUE); // Turn screen blue
    lcd.setCursor(0, 0);
    lcd.print("Current: ");
    lcd.print(temperatureC, 2);
    lcd.setCursor(0, 1);
    lcd.print("Set Point: ");
    lcd.print(setPoint, 2);
    delay(1000);
    adcValue = analogRead(A1); // Update temp
    temperatureC = (adcValue) * (0.154) - (28.36); 
  } 

// If current temperature is above the setpoint
  while (temperatureC > highTemp) { 
    buttonPress();
    tone(2, 880); // Play alarm tone
    digitalWrite(7, LOW); // Turn off heater
    lcd.clear();
    lcd.setBacklight(RED); // Turn screen red
    lcd.setCursor(0, 0);
    lcd.print("Current: ");
    lcd.print(temperatureC, 2);
    lcd.setCursor(0, 1);
    lcd.print("Set Point: ");
    lcd.print(setPoint, 2);
    delay(1000);
    adcValue = analogRead(A1); // Update temp
    temperatureC = (adcValue) * (0.154) - (28.36); 
  } // Keep looping until cool enough
}

void buttonPress() {
  uint8_t buttons = lcd.readButtons(); 
  if (buttons) { // Down lowers setpoint
    if (buttons & BUTTON_DOWN) { 
      if (setPoint >= 33) { // Unless below 33C
        setPoint --;
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SET POINT: ");
        lcd.setCursor(0, 1);
        lcd.print("TOO LOW: ");
        delay(500);
      }
    }
    if (buttons & BUTTON_UP) { // Up raises setpoint
      if (setPoint <= 39) { // Unless above 39C
        setPoint ++;
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SET POINT: ");
        lcd.setCursor(0, 1);
        lcd.print("TOO LOW: ");
        delay(500);
      }
    }
    if (buttons & BUTTON_RIGHT) { // Right raises hysteris
      if (hysteris <= 2.0) {
        hysteris += 0.5;
      }
    }
    if (buttons & BUTTON_LEFT) { // Left  lowers hysteris
      if (hysteris >= 2.0) {
        hysteris -= 0.5;
      }
    }
  }
}
