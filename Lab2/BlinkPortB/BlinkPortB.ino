void setup() {
pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
PORTB = 255;
PORTB = 0;
}

