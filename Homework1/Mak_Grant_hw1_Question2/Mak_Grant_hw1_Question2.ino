const unsigned long unit = 200;
unsigned long dot = unit * 1;
unsigned long dash = unit * 3;
unsigned long letterSpace = unit * 3;
unsigned long wordSpace = unit * 7;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  delay(2000);
}


void loop() {
  morse('s');
  delay(letterSpace);
  morse('o');
  delay(letterSpace);
  morse('s');
  delay(letterSpace);
}

void morse(char c) {
  switch (c) {
    case 's' :
      for (int n = 1; n <= 3; n++) {
        Serial.print('.');
        digitalWrite(LED_BUILTIN, HIGH);
        delay(dot);
        digitalWrite(LED_BUILTIN, LOW);
        if (n != 3) {
          delay(unit);
        }
      }
      break;
    case 'o' :
      for (int n = 1; n <= 3 ; n++) {
        Serial.print('-');
        digitalWrite(LED_BUILTIN, HIGH);
        delay(dash);
        digitalWrite(LED_BUILTIN, LOW);
        if (n != 3) {
          delay(unit);
        }
      }
      break;
  }
}
