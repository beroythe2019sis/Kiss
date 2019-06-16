int LED = 13;             // the pin that the LED is atteched to
int PIR = 7;              // the pin that the sensor is atteched to

void setup() {
  pinMode(LED, OUTPUT);   // initalize LED as an output
  pinMode(PIR, INPUT);    // initialize sensor as an input
  Serial.begin(9600);     // initialize serial
}

void loop(){
  if (digitalRead(PIR) == HIGH) { // check if the sensor is HIGH
    digitalWrite(LED, HIGH);      // turn LED ON 
    Serial.println("motion detected"); 
    delay(100);                   // delay 100 milliseconds 
  } 
  else {
    digitalWrite(LED, LOW);       // turn LED OFF
    Serial.println("no motion");
    delay(100);                   // delay 100 milliseconds
  }
}