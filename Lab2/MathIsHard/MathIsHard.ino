

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int operandA {8346};
int operandB {172};
int result {0};
  char pb5on = 1 << 5; // set bit 5 (pin 13)
  char pb5off = 0;
  while (1) {
    PORTB = pb5on;
    PORTB = pb5off;
    result = operandA * operandB;
    PORTB = pb5on;
    PORTB = pb5off;
    PORTB = pb5on;
    PORTB = pb5off;
  }
}

