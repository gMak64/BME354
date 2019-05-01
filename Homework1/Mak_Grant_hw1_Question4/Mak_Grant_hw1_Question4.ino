void setup(){
 Serial.begin(9600);
 delay(2000); // wait until the board fully wakes up.
}

void loop(){
 float adcValue = analogRead(A1); //simulating analogRead()
 float temperatureC = adcValue/1024*5*50; //(50C/V)
 Serial.println(temperatureC);
}
