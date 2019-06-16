/*
List of sensors:
HSCR04
DHT11
MIC-Adafruit
MLX90614 IR Temp Sensor
PIR IR Motion Sensor
*/




/*
  Ultrasonic Sensor HC-SR04 and Arduino Tutorial
  by Dejan Nedelkovski,
  www.HowToMechatronics.com
*/
// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;





// Adafruit MLX90614 Library - Version: Latest 
#include <SimpleDHT.h>
// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);





/****************************************
  Example Sound Level Sketch for the
  Adafruit Microphone Amplifier
****************************************/

const int sampleWindow = 200; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;




/*************************************************** 
  This is a library example for the MLX90614 Temp Sensor

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1748
  ----> https://www.adafruit.com/products/1749

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//PIR_ir_motion_sensor
int LED = 13;             // the pin that the LED is atteched to
int PIR = 7;              // the pin that the sensor is atteched to




void setup() {
  pinMode(trigPin, OUTPUT); // (HCSR04)Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // (HCSR04) Sets the echoPin as an Input

    pinMode(PIR, INPUT);   // (PIR) initialize sensor as an input
  
  Serial.begin(9600); // Starts the serial communication option: 115200
  
  mlx.begin();  
}





void loop() {



///*

  // MLX IR TEMP SENSOR
  Serial.print("1) Ambient: "); 
  Serial.println(mlx.readAmbientTempC()); 
  Serial.print("2) Object: "); 
  Serial.println(mlx.readObjectTempC()); 
  //delay(500);


//*/

///*


  //PIR_ir_motion_sensor
if (digitalRead(PIR) == HIGH) { // check if the sensor is HIGH
    digitalWrite(LED, HIGH);      // turn LED ON 
    Serial.println("3) motion detected"); 
    //delay(100);                   // delay 100 milliseconds 
  } 
  else {
    digitalWrite(LED, LOW);       // turn LED OFF
    Serial.println("3) no motion");
    //delay(100);              x     // delay 100 milliseconds
  }

//*/



///*

  // MIC_Adafruit
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    Serial.print("4) Mic Max: ");
    Serial.println (signalMax);
    Serial.print("5) Mic Min: ");
    Serial.println (signalMin);


//*/




///*

  // HC-SR04
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("6) Distance: ");
  Serial.println(distance);

//*/



///*

  // DHT11
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1500);
    return;
  }
  Serial.print("7) DHT Temp: ");
  Serial.println((int)temperature); 
  Serial.print("8) DHT Humid: "); 
  Serial.println((int)humidity); 
  delay (1500);

//*/













  
}
