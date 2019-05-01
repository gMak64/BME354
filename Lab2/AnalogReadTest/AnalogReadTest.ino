void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  char pb5on = 1 << 5; // set bit 5 (pin 13)
  char pb5off = 0;
  char pb4on = 1 << 4; //set bit 4 (pin 12)
  char pb4off = 0;
  while (1) {
    PORTB = pb5on;
    delayMicroseconds(5);
    PORTB = pb5off;
    analogRead(1);
    PORTB = pb4on;
    delayMicroseconds(5);
    PORTB = pb4off;
  }

}
