#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// Colors for the LCD display
#define RED 0x1 
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

// Function prototypes to avoid compiler errors
void choosePneumo();
void mainMenuDraw();
void operateMainMenu();
double flowCount(double);
void countdown();
void calibrate();
void findK(double*, double*);
void maxRate();
void inspiratoryCapacity(double);
void expiratoryCapacity(double);
void tidalVolume(double);
void vitalCapacity(double);

// Initialize variables and menu items
int pneumo {0};
bool chosen {false};
double analogPin {A0};
double val {0};
double offset {0};
double flow {0};
double maxFlow {0};
double minFlow {0};
double kPlus1 {0};
double kPlus2 {0};
double kPlus3 {0};
double kMinus1 {0};
double kMinus2 {0};
double kMinus3 {0};
double kPlusMetal {570};
double kMinusMetal {1300};
double kPlusPlastic {370};
double kMinusPlastic {1200};
double kPlusAverage {0};
double kMinusAverage {0};
unsigned long timeStart {0};
String menuItems[] = {"CALIBRATE     ", "MAX RATE      ", "INP CAP       ", "EXP CAP       ", "TIDAL VOLUME  ", "VITAL CAP    "};
int menuPage {-1};
int maxMenuPages {((sizeof(menuItems) / sizeof(String))) - 2};

// Define UI prompt arrows
byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};

byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};

byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  // Calculate the offset
  for (int n = 1; n <= 100; n++) {
    offset += analogRead(analogPin);
  }
  offset = offset/100;
  // Turn on the LCD Screen
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setBacklight(TEAL);
  lcd.setCursor(0, 0);
  lcd.print("HELLO! :)");
  lcd.setCursor(0, 1);
  lcd.print("CASSIE + GRANT");
  delay(2500);
  lcd.clear();
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);
}

// Resets function returns to address 0
void(*resetFunc) (void) = 0;

void loop() {
  // Stay here until pneuomtach is chosen
  while (!chosen) {
    choosePneumo();
  }
  // Create the main UI
  mainMenuDraw();
  operateMainMenu();
}

// Choose correct K constant for pneumotach
void choosePneumo()  {
  uint8_t buttons = lcd.readButtons();
  lcd.setCursor(0, 0);
  lcd.write(byte(1));
  lcd.print(" FOR PLASTIC");
  lcd.setCursor(0, 1);
  lcd.write(byte(2));
  lcd.print(" FOR METAL");
  if (buttons) {
    // Up button selects plastic pneumotach
    if (buttons & BUTTON_UP) {
      kPlusAverage = kPlusPlastic;
      kMinusAverage = kMinusPlastic;
      pneumo = 1;
      chosen = true;
    }
    // Down button selects plastic pneumotach
    if (buttons & BUTTON_DOWN) {
      kPlusAverage = kPlusMetal;
      kMinusAverage = kMinusMetal;
      pneumo = 2;
      chosen = true;
    }
    lcd.clear();
  }
}

void mainMenuDraw() {
  lcd.setCursor(0, 0);
  flow = flowCalc(analogPin);
  // Update max rates if needed
  if (flow < minFlow) minFlow = flow;
  if (flow > maxFlow) maxFlow = flow;
  // Show the current flow
  if (flow == 0) {
    lcd.print(flow, 5);
    lcd.print(" L/m   ");
  }
  else {
    lcd.print(flow, 2);
    lcd.print(" L/m    ");
  }
  lcd.setCursor(1, 1);
  // Print the selected menu item with arrows
  lcd.print(menuItems[menuPage + 1]);
  // First item has no up arrow
  if (menuPage == -1) {
    lcd.setCursor(15, 0);
    lcd.print("  ");
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > -1 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
    // Last item has no down arrow
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
    lcd.setCursor(15, 1);
    lcd.print(" ");
  }
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
}

void operateMainMenu() {
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    // Right button enters selected item
    if (buttons & BUTTON_RIGHT) {
      switch (menuPage) {
        case -1:
          calibrate();
          break;
        case 0:
          maxRate();
          break;
        case 1:
          inspiratoryCapacity(analogPin);
          break;
        case 2:
          expiratoryCapacity(analogPin);
          break;
        case 3:
          tidalVolume(analogPin);
          break;
        case 4:
          vitalCapacity(analogPin);
          break;
      }
      mainMenuDraw();
    }
    // Up button scrolls up
    if (buttons & BUTTON_UP) {
      menuPage = menuPage - 1;
      menuPage = constrain(menuPage, -1, maxMenuPages);
      mainMenuDraw();
    }
    // Down button scrolls down
    if (buttons & BUTTON_DOWN) {
      menuPage = menuPage + 1;
      menuPage = constrain(menuPage, -1, maxMenuPages);
      mainMenuDraw();
    }
    // Left button resets
    if (buttons & BUTTON_LEFT) {
      resetFunc();
    }
    }
  }

void countdown() {
  // Displays 3 second countdown
  for (int i = 3; i >= 1; i--) {
    lcd.setCursor(0, 0);
    lcd.print(i);
    delay(1000);
    lcd.clear();
  }
}

// Returns current flow through device
double flowCalc(double pin) {
  // Take in analog input
  val = analogRead(analogPin) - offset;
  // No flow if inside hysteresis
  if (val >= -10 && val <= 10) {
    flow = 0;
  }
  // Positive = expiration
  else if (val > 10) {
    flow = val * 5 * kPlusAverage / 1024;
  }
  // Negative == inspiration
  else if (val < -10) {
    flow = val * 5 * kMinusAverage / 1024;
  }
  return flow;
}

// Returns the average of three K constants
void calibrate() {
  lcd.clear();
  // Find K three times
  findK(kPlus1, kMinus1);
  delay(3100);
  lcd.clear();
  findK(kPlus2, kMinus2);
  delay(3100);
  lcd.clear();
  findK(kPlus3, kMinus3);
  delay(3100);
  lcd.clear();
  if (pneumo == 1) {
    // For plastic pneumotach
    kPlusPlastic = (kPlus1 + kPlus2 + kPlus3) / 3;
    kPlusAverage = abs(kPlusPlastic);
    kMinusPlastic = (kMinus1 + kMinus2 + kMinus3) / 3;
    kMinusAverage = abs(kMinusPlastic);
  }
  else if (pneumo == 2) {
    // For metal pneumotach
    kPlusMetal = (kPlus1 + kPlus2 + kPlus3) / 3;
    kPlusAverage = abs(kPlusMetal);
    kMinusMetal = (kMinus1 + kMinus2 + kMinus3) / 3;
    kMinusAverage = abs(kMinusMetal);
  }
}

// Actually finds the K constant
void findK(double &kPlus, double &kMinus) {
  // Set running sums to 0
  double positiveVoltage = 0;
  double negativeVoltage = 0;
  // Prompt a "breath"
  countdown();
  lcd.print("BREATHE!");
  timeStart = millis();
  while ((millis() - timeStart) <= 3000) {
    // Samples for three seconds at 10 ms intervals
    delay(10);
    val = analogRead(analogPin);
    // Convert bitwise value to actual voltage
    flow = val * 5 / 1024;
    // Riemann sum of voltage readings with respect to time
    if (val >= (offset + 15)) {
      positiveVoltage += ((flow - (offset * 5 / 1024)) * 10.2);
    }
    else if (val <= (offset - 15)) { 
      negativeVoltage += (((offset * 5 / 1024 - flow)) * 10.2);
    }
  }
  // Convert to L/min
  kPlus = 3 * 60000 / positiveVoltage;
  kMinus = 3 * 60000 / negativeVoltage;
}

// Display max flow rates
void maxRate() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EXP: ");
  lcd.print(minFlow);
  lcd.print(" L/m");
  lcd.setCursor(0, 1);
  lcd.print("INP: ");
  lcd.print(maxFlow);
  lcd.print(" L/m");
  delay(3000);
  lcd.clear();
}

void inspiratoryCapacity(double pin) {
  double inpFlow {0};
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMALL BREATH IN");
  lcd.setCursor(0, 1);
  lcd.print("LARGE BREATH OUT");
  delay(2000);
  lcd.clear();
  countdown();
  lcd.print("BREATHE!");
  timeStart = millis();
  // Integrate desired flow with respect to time
  while ((millis() - timeStart) <= 5000) {
    delay(10);
    flow = abs(flowCalc(pin));
    if (flow > 10) {
      inpFlow += (flow * 10.5 / 6000);
    }
  }
  inpFlow /= 60; 
  lcd.setCursor(0,0);
  lcd.print("INP CAP: ");
  lcd.setCursor(0,1);
  lcd.print(" L ");
  lcd.print(inpFlow);
  delay(3000);
  lcd.clear();
}

void expiratoryCapacity(double pin) {
  double expFlow {0};
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMALL BREATH OUT");
  lcd.setCursor(0, 1);
  lcd.print("LARGE BREATH IN");
  delay(2000);
  lcd.clear();
  countdown();
  lcd.print("BREATHE!");
  timeStart = millis();
  // Integrate desired flow with respect to time
  while ((millis() - timeStart) <= 5000) {
    delay(10);
    flow = abs(flowCalc(pin));
    if (flow > 10) {
      expFlow += (flow * 10.5 / 6000);
    }
  }
  expFlow /= 60;
  lcd.setCursor(0,0);
  lcd.print("EXP CAP: ");
  lcd.setCursor(0,1);
  lcd.print(" L ");
  lcd.print(expFlow);
  delay(3000);
  lcd.clear();
}

void tidalVolume(double pin) {
  double tidalVolume {0};
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMALL BREATH IN");
  lcd.setCursor(0, 1);
  lcd.print("SMALL BREATH OUT");
  delay(2000);
  lcd.clear();
  countdown();
  lcd.print("BREATHE!");
  timeStart = millis();
  // Integrate desired flow with respect to time
  while ((millis() - timeStart) <= 6000) {
    delay(10);
    flow = abs(flowCalc(pin));
    if (flow > 10) {
      tidalVolume += (flow * 10.5 / 600);
    }
  }
  lcd.setCursor(0,0);
  lcd.print("TIDAL VOLUME: ");
  lcd.setCursor(0,1);
  lcd.print(tidalVolume);
  lcd.print(" mL ");
  Serial.println(tidalVolume);
  delay(3000);
  lcd.clear();
}

void vitalCapacity(double pin) {
  double vitalCapacity {0};
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BIG BREATH IN");
  lcd.setCursor(0, 1);
  lcd.print("BIG BREATH OUT");
  delay(2000);
  lcd.clear();
  countdown();
  lcd.print("BREATHE!");
  timeStart = millis();
  // Integrate desired flow with respect to time
  while ((millis() - timeStart) <= 8000) {
    delay(10);
    flow = abs(flowCalc(pin));
    if (flow > 10) {
      vitalCapacity += (flow * 10.5 / 6000);
    }
  }
  lcd.setCursor(0,0);
  lcd.print("VITAL CAP: ");
  lcd.setCursor(0,1);
  lcd.print(vitalCapacity);
  lcd.print(" L ");
  delay(3000);
  lcd.clear();
}
