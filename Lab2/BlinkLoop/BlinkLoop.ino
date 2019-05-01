void setup() {
  pinMode(LED_BUILTIN, OUTPUT); //Turn on the build in LED
}

void loop() {
  char pb5on = 1 << 5; // Set a bit = 1 (on), shift it left (pin13)
  char pb5off = 0; // Set a bit = 0 (off)
  while (1) { // Turns pin13/bit5 on and off forever
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
  }
}

