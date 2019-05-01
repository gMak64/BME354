void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Turn the built in LED on
  Serial.begin(9600); // Activate the Serial Monitor; bit rate = 9600 bd

void loop() {
  char pb5on = 1 << 5; // set bit 5 to 1/on (pin 13)
  char pb5off = 0; // set bit 5 to 0/off (pin 13)
  char pb4on = 1 << 4; // set bit 4 to 1/on (pin 12)
  char pb4off = 0; //set bit 4/off (pin 12)
  while (1) {
    PORTB = pb5on; // Turn on the LED
    delay(2000);
    PORTB = pb5off; // Turn off the LED
    Serial.println("12345678901234567890"); // Read and print
    PORTB = pb4on; // Turn on pin 12 for timekeeping purposes
    delay(2000);
    PORTB = pb4off; // Turn off pin 12
  }
}
