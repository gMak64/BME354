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

String menuItems[] = {"CALIBRATE  ", "OPTION B  ", "OPTION C  ", "OPTION D  ", "OPTION E  "};

int menuPage = -1;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5);
int cursorPosition = -1;

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
  Serial.println(menuPage);
  pinMode(2, INPUT);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setBacklight(TEAL);
  lcd.setCursor(0, 0);
  lcd.print("WELCOME!");
  delay(2500);
  lcd.clear();
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);
}

void loop() {
  mainMenuDraw();
  operateMainMenu();
}

void mainMenuDraw() {
  Serial.print(menuPage);
  lcd.setCursor(0, 0);
  lcd.print("FLOW:" );
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);
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
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
    lcd.setCursor(15, 1);
    lcd.write(" ");
  }
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
}

void operateMainMenu() {
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    if (buttons & BUTTON_SELECT) {
      switch (menuPage) {
        case -1:
          calibrate();
          break;
        case 0:
          menuItem2();
          break;
        case 1:
          menuItem3();
          break;
        case 2:
          menuItem4();
          break;
        case 3:
          menuItem5();
          break;
      }
      mainMenuDraw();
    }
    if (buttons & BUTTON_UP) {
      menuPage = menuPage - 1;
      menuPage = constrain(menuPage, -1, maxMenuPages);
      mainMenuDraw();
    }
    if (buttons & BUTTON_DOWN) {
      menuPage = menuPage + 1;
      menuPage = constrain(menuPage, -1, maxMenuPages);
      mainMenuDraw();
    }
  }
}

void calibrate() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A FOR APPLE");
  delay(2000);
  lcd.clear();
}

void menuItem2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("B FOR BEAR");
  delay(2000);
  lcd.clear();
}

void menuItem3() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("C FOR CASSIE");
  delay(2000);
  lcd.clear();
}

void menuItem4() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("D FOR DUNCE");
  delay(2000);
  lcd.clear();
}

void menuItem5() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("E IS FOR ELF");
  delay(2000);
  lcd.clear();
}
