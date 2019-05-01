// unsigned long to store time because ms gets large quickly
unsigned long previousTime = 0;

// time between samples (milliseconds)
const long interval = 10;

// omega (radial frequency) of the sine function in radians/s
// for this example use a temporal frequency of 0.5 Hz,
// so, a radial frequency of 2*pi*0.5 rad/s = pi rad/s
// M_PI is a pre-defined global variable = 3.1416 in Arduino code
float omega = M_PI;

// sine function's argument (angle = omega*t) starts at 0
float angle = 0;
float angle2 = 0;

void setup() {
  // make sure Serial Plotter is open to visualize data in graph form
  Serial.begin(9600);
  delay(2000); // wait 2 secs until the board fully wakes up.
}

void loop() {
  // find current time (milliseconds)
  unsigned long currentTime = millis();

  // if time difference is greater than the time between samples, then
  // compute sine function’s argument for next sample (i.e. new angle)
  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;

    // update angle incrementally
    angle = angle + ((float)interval / 1000 * omega);
    angle2 = angle2 + ((float)interval / 1000 * omega);

    // when phase >2*pi reset to 0 so phase stays between 0 & 2*pi
    if (angle >= 2 * M_PI) {
      angle = angle - 2 * M_PI;
      angle2 = angle2 - 2 * M_PI;
    }

    // compute output of ADC simulator
    // this is a 1 bit ADC (only output can be 0 or 1, so need to
    // assess value of the sinusoid, compare to threshold and print
    if (sin(angle) < 0) {
      Serial.print(0);
    }
    else {
      Serial.print(1);
    }
    // seperate the second coordinate to Plot with comma
    Serial.print(",");
    // plot the original sine signal
    Serial.println(sin(angle));
    // separate the third coordinate to Plot with comma
    Serial.print(",");
    if (sin(angle2) < -0.5 && sin(angle2) >= -1) {
      Serial.print(0);
    }
    else if (sin(angle2) < 0 && sin(angle2) >= -0.5) {
      Serial.print(0.33);
    }
    else if (sin(angle2) < 0.5 && sin(angle2) >= 0) {
      Serial.print(0.66);
    }
    else {
      Serial.print(1);
    }
    // separate the fourth coordinate to Plot with comma
    Serial.print(",");
    Serial.println(sin(angle2));
  }
}
