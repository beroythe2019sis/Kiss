/* 
 Created by Andrew Payne and Jason Kelly Johnson
 Latest Update March 25th, 2015 
 Copyright 2015 | All Rights Reserved
 
 This Firmata allows you to control an Arduino board from Rhino/Grasshopper/Firefly.
 Updates, Questions, Suggestions visit: http://www.fireflyexperiments.com
 
 1. Plug Arduino boards into your USB port; confirm that your Arduino's green power LED in on
 2. Select your specific Arduino Board and Serial Port (Tools > Board; Tools > Serial Port) *Take note of your Serial Port COM #
 3. Verify (play button) and Upload (upload button) this program to your Arduino, close the Arduino program
 4. then open ... Rhino/Grasshopper/Firefly
 
 Note: The Firefly Firmata sets the following pins to perform these functions:
 
 *****ON STANDARD BOARDS (ie. Uno, Diecimila, Duemilanove, Lillypad, Mini, etc.)*****
 ANALOG IN pins 0-5 are set to return values (from 0 to 1023) for analog sensors
 DIGITAL IN pins 2,4,7 will return 0's or 1's; for 3 potential digital sensors (buttons, switches, on/off, true/false, etc.)
 DIGITAL/ANALOG OUT pins 3,5,6,11 (marked with a ~) can be used to digitalWrite, analogWrite, or Servo.write depending on the input status of that Firefly pin
 DIGITAL OUT pins 8,9,10,12,13 can be used to digitalWrite, Servo.write, or analogWrite depending on the input status of that Firefly pin
 
 *****ON MEGA BOARDS (ie. ATMEGA1280, ATMEGA2560)*****
 ANALOG IN pins 0-15 will return values (from 0 to 1023) for 16 analog sensors 
 DIGITAL IN pins 22-31 will return 0's or 1's; for digital sensors (buttons, switches, on/off, true/false, etc.) 
 DIGITAL/ANALOG OUT pins 2-13 can be used to digitalWrite, analogWrite, or Servo.write depending on the input status of that Firefly pin
 DIGITAL OUT pins 32-53 can be used to digitalWrite, Servo.write, or analogWrite depending on the input status of that Firefly pin
 
 *****ON LEONARDO BOARDS*****
 ANALOG IN pins 0-5 are set to return values (from 0 to 1023) for analog sensors
 DIGITAL IN pins 2,4,7 will return 0's or 1's; for 3 potential digital sensors (buttons, switches, on/off, true/false, etc.)
 DIGITAL/ANALOG OUT pins 3,5,6,11 (marked with a ~) can be used to digitalWrite, analogWrite, or Servo.write depending on the input status of that Firefly pin
 DIGITAL OUT pins 8,9,10,12,13 can be used to digitalWrite, Servo.write, or analogWrite depending on the input status of that Firefly pin
 
  *****ON DUE BOARDS (ie. SAM3X8E)*****
 ANALOG IN pins 0-11 will return values (from 0 to 4095) for 12 analog sensors 
 DIGITAL IN pins 22-31 will return 0's or 1's; for digital sensors (buttons, switches, on/off, true/false, etc.) 
 DIGITAL/ANALOG OUT pins 2-13 can be used to digitalWrite, analogWrite, or Servo.write depending on the input status of that Firefly pin
 DIGITAL OUT pins 32-53 can be used to digitalWrite, Servo.write, or analogWrite depending on the input status of that Firefly pin
 DAC0 and DAC1 can be used to output an analog voltage on those pins (only available on DUE boards)
 */

 /*
  File: serialBraccio.ino

  Copyright 2018 Stefan Strömberg, stefangs@nethome.nu

  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
  in compliance with the License. You may obtain a copy of the License at
  http://www.apache.org/licenses/LICENSE-2.0
  Unless required by applicable law or agreed to in writing, software distributed under the License
  is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
  either express or implied. See the License for the specific language governing permissions and
  limitations under the License.
*/

/*
  This example reads robot positions from the serial port and puts
  the Braccio in those positions. A position is formatted as:

  P90,90,90,90,90,73,100<NL>

  Where the numbers are the joint angels in the same order as in the Position class and the last
  number is the speed of the movement in degrees per second.
  When the arm has reached the specified position it will answer with the string:

  OK<NL> 

  You can also issue the Home command (H), to put the robot in the home position, the Off command (0)
  to turn off the robot or the On command (1) to turn on the robot:

  H<NL>

  Note, that when opening the serial port to Arduino, the Arduino restarts, which causes
  it to become non responsive for serial input for a couple of seconds. Therefore a
  controller program should wait at least 3 seconds after opening the port before sending
  any commands. An example in Python:

  s = serial.Serial('COM4', 115200, timeout=5)
  time.sleep(3)
  s.write(b'P0,90,20,90,90,73,100\n')
  print(s.readline())
  s.write(b'P90,90,20,90,90,73,100\n')
  print(s.readline())

  You should also wait until you get an "OK" (or "E0") from the Arduino before sending the
  next position, like in the above example using readline().
*/

#include <Servo.h>            // F R attach Servo library (http://www.arduino.cc/playground/ComponentLib/Servo)
#include <pins_arduino.h>     // F attach arduino pins header file to determine which board type is being used

#include "BraccioRobot.h"     // R
#define INPUT_BUFFER_SIZE 50  // R

static char inputBuffer[INPUT_BUFFER_SIZE];   // R
Position armPosition;                         // R

#define BAUDRATE 115200       // F Set the Baud Rate to an appropriate speed
#define BUFFSIZE 512          // F buffer one command at a time

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

char buffer[BUFFSIZE];        // F declare buffer
uint8_t bufferidx = 0;        // F a type of unsigned integer of length 8 bits
char *parseptr;               // F
char buffidx;                 // F

int counter = 0;              // F
int numcycles = 1000;         // F

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)                                                // F declare variables for STANDARD boards
  Servo Servo13, Servo12, Servo11, Servo10, Servo9, Servo8, Servo6, Servo5, Servo3;                           // F
  Servo SERVO_CONFIG[] = {Servo13, Servo12, Servo11, Servo10, Servo9, Servo8, Servo6, Servo5, Servo3};       // F declare array of Servo objects
  int WRITE_PIN_CONFIG[] = {13,12,11,10,9,8,6,5,3};      // F
  int READ_APIN_CONFIG[] = {0,1,2,3,4,5};   // F
  int READ_DPIN_CONFIG[] = {2,4,7};     // F
#endif  // F

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)          // F                                         // declare variables for LEONARDO board
  Servo Servo13, Servo12, Servo11, Servo10, Servo9, Servo8, Servo6, Servo5, Servo3;   // F       
  Servo SERVO_CONFIG[] = {Servo13, Servo12, Servo11, Servo10, Servo9, Servo8, Servo6, Servo5, Servo3};  // F     // declare array of Servo objects
  int WRITE_PIN_CONFIG[] = {13,12,11,10,9,8,6,5,3};  // F 
  int READ_APIN_CONFIG[] = {0,1,2,3,4,5}; // F
  int READ_DPIN_CONFIG[] = {2,4,7};  // F
#endif  // F

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)        // F                // declare variables for MEGA boards
  Servo Servo2, Servo3, Servo4, Servo5, Servo6, Servo7, Servo8, Servo9, Servo10, Servo11, Servo12, Servo13, Servo32, Servo33, Servo34, Servo35, Servo36, Servo37, Servo38, Servo39, Servo40, Servo41, Servo42, Servo43, Servo44, Servo45, Servo46, Servo47, Servo48, Servo49, Servo50, Servo51, Servo52, Servo53;   // F
  Servo SERVO_CONFIG[] = {Servo2, Servo3, Servo4, Servo5, Servo6, Servo7, Servo8, Servo9, Servo10, Servo11, Servo12, Servo13, Servo32, Servo33, Servo34, Servo35, Servo36, Servo37, Servo38, Servo39, Servo40, Servo41, Servo42, Servo43, Servo44, Servo45, Servo46, Servo47, Servo48, Servo49, Servo50, Servo51, Servo52, Servo53};   // F // declare array of Servo objects
  int WRITE_PIN_CONFIG[] = {2,3,4,5,6,7,8,9,10,11,12,13,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53};   // F
  int READ_APIN_CONFIG[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};  // F
  int READ_DPIN_CONFIG[] = {22,23,24,25,26,27,28,29,30,31};  // F
#endif  // F

#if defined(__SAM3X8E__)            // F     // declare variables for DUE boards
  Servo FDAC0, FDAC1, Servo2, Servo3, Servo4, Servo5, Servo6, Servo7, Servo8, Servo9, Servo10, Servo11, Servo12, Servo13, Servo32, Servo33, Servo34, Servo35, Servo36, Servo37, Servo38, Servo39, Servo40, Servo41, Servo42, Servo43, Servo44, Servo45, Servo46, Servo47, Servo48, Servo49, Servo50, Servo51, Servo52, Servo53;    // F
  Servo SERVO_CONFIG[] = {FDAC0, FDAC1, Servo2, Servo3, Servo4, Servo5, Servo6, Servo7, Servo8, Servo9, Servo10, Servo11, Servo12, Servo13, Servo32, Servo33, Servo34, Servo35, Servo36, Servo37, Servo38, Servo39, Servo40, Servo41, Servo42, Servo43, Servo44, Servo45, Servo46, Servo47, Servo48, Servo49, Servo50, Servo51, Servo52, Servo53};    // F// declare array of Servo objects
  int WRITE_PIN_CONFIG[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53}; //Note: first two values correspond to the DAC pins
  int READ_APIN_CONFIG[] = {0,1,2,3,4,5,6,7,8,9,10,11};  // F
  int READ_DPIN_CONFIG[] = {22,23,24,25,26,27,28,29,30,31};  // F
#endif  // F

/*==============================================================================
 * SETUP() This code runs once
 *============================================================================*/
void setup()
{ 
  Init();                       // F //set initial pinmodes
  Serial.begin(BAUDRATE);       // F // Start Serial communication
  #if defined(__SAM3X8E__)      // F //if the connected board is an Arduino DUE
    analogReadResolution(12);   // F //Set the analog read resolution to 12 bits (acceptable values between 1-32 bits).  This is only for DUE boards
    analogWriteResolution(12);  // F // Set the analog write resolution to 12 bits (acceptable values between 1-32 bits).  This is only for DUE boards
  #endif                        // F
  BraccioRobot.init();
  
}

/*==============================================================================
 * LOOP() This code loops
 *============================================================================*/
void loop()
{
  if(Serial){                           // F
    ReadSerial();                       // F// read and parse string from serial port and write to pins
    if (counter >= numcycles){          // F// Wait every nth loop 
      ReadInputs();                     // F// get input data and print data to the serial port
      counter = 0;                      // F// reset the counter
    }                                   // F
    counter ++;                         // F // increment the writecounter
  }
      handleInput();                 
}

/*==============================================================================
 * FUNCTIONS() F
 *============================================================================*/

/*
* Initializes the digital pins which will be used as inputs
*/
void Init(){
  int len = sizeof(READ_DPIN_CONFIG)/sizeof(READ_DPIN_CONFIG[0]); //get the size of the array
  for(int i = 0; i < len; i++){
    pinMode(READ_DPIN_CONFIG[i], INPUT);
  }
}

/* 
* Reads the incoming ADC or digital values from the corresponding analog and digital input  
* pins and prints the value to the serial port as a formatted commma separated string
*/
void ReadInputs(){ 
  int len = sizeof(READ_APIN_CONFIG)/sizeof(READ_APIN_CONFIG[0]); //get the size of the array
  for(int i = 0; i < len; i++){
    int val = analogRead(READ_APIN_CONFIG[i]);  //read value from analog pins
    Serial.print(val); Serial.print(",");   
  }
  len = sizeof(READ_DPIN_CONFIG)/sizeof(READ_DPIN_CONFIG[0]); //get the size of the array
  for(int i = 0; i < len; i++){
    int val = digitalRead(READ_DPIN_CONFIG[i]); //read value from digital pins
    Serial.print(val); Serial.print(",");   
  }
  Serial.println("eol");  //end of line marker
}

/*
* Retrieve the latest incoming serial value and split the string at the comma delimeter.
* When a comma is found, the value is offloaded to a temporary variable and written
* to the corresponding digital pin.
*/
void ReadSerial(){
  char c;    // holds one character from the serial port
  if (Serial.available()) {
    c = Serial.read();         // read one character
    buffer[bufferidx] = c;     // add to buffer
    if (c == '\n') {  
      buffer[bufferidx+1] = 0; // terminate it
      parseptr = buffer;       // offload the buffer into temp variable
      int len = sizeof(WRITE_PIN_CONFIG)/sizeof(WRITE_PIN_CONFIG[0]); //get the size of the array
      for(int i = 0; i < len; i++){
        //parse all incoming values and assign them to the appropriate variable
        int val = parsedecimal(parseptr);       // parse the incoming number
        if(i != len - 1) parseptr = strchr(parseptr, ',')+1;   // move past the ","
        WriteToPin(WRITE_PIN_CONFIG[i], val, SERVO_CONFIG[i]);         //send value out to pin on arduino board
      }    
      bufferidx = 0;                             // reset the buffer for the next read
      return;                                    // return so that we don't trigger the index increment below
    }                                            // didn't get newline, need to read more from the buffer
    bufferidx++;                                 // increment the index for the next character
    if (bufferidx == BUFFSIZE-1) bufferidx = 0;  // if we get to the end of the buffer reset for safety
  }
}

/*
* Send the incoming value to the appropriate pin using pre-defined logic (ie. digital, analog, or servo)
*/
void WriteToPin(int _pin, int _value, Servo _servo){
  if (_value >= 10000 && _value < 20000)            // check if value should be used for Digital Write (HIGH/LOW)
  {      
    if (_servo.attached()) _servo.detach();         // detach servo is one is attached to pin
    pinMode(_pin, OUTPUT);                       
    _value -= 10000;                                // subtract 10,000 from the value sent from Grasshopper 
    if (_value == 1) digitalWrite(_pin, HIGH);     
    else digitalWrite(_pin, LOW);   
  }   
  else if (_value >= 20000 && _value < 30000)       // check if value should be used for Analog Write (0-255)
  {
    if (_servo.attached()) _servo.detach();         // detach servo is one is attached to pin
    pinMode(_pin, OUTPUT);               
    _value -= 20000;                                // subtract 20,000 from the value sent from Grasshopper
    analogWrite(_pin, _value);                     
  }
  else if (_value >= 30000 && _value < 40000)       // check if value should be used for Servo Write (0-180)
  {
    _value -= 30000;                                // subtract 30,000 from the value sent from Grasshopper
    if (!_servo.attached())_servo.attach(_pin);     // attaches a Servo to the PWM pin (180 degree standard servos)                                    
    _servo.write(_value);                          
  }
  else if (_value >= 40000 && _value < 50000)       // check if value should be used for Analog Write (0-4096) for DACs
  {
    if (_servo.attached()) _servo.detach();         // detach servo is one is attached to pin
    pinMode(_pin, OUTPUT);               
    _value -= 40000;                                // subtract 40,000 from the value sent from Grasshopper
    WriteToDAC(_pin, _value);                     
  }
}

/*
* Parse a string value as a decimal
*/
uint32_t parsedecimal(char *str){
  uint32_t d = 0;
  while (str[0] != 0) {
    if ((str[0] > '50') || (str[0] < '0'))
      return d;
    d *= 10;
    d += str[0] - '0';
    str++;
  }
  return d;
}

/*
* Send the incoming value to the appropriate DAC for DUE boards. 
* Note: analogWrite resolution (default is 12 bits) is defined in the Setup function.
*/
  
  void WriteToDAC(int _pin, int _value){
    #if defined(__SAM3X8E__)
    if(_pin == 0) analogWrite(DAC0, _value);
    else if (_pin == 1) analogWrite(DAC1, _value);
    #endif
  }


  /*==============================================================================
 * FUNCTIONS() R
 *============================================================================*/

 void handleInput() {
  if (Serial.available() > 0) {
    byte result = Serial.readBytesUntil('\n', inputBuffer, INPUT_BUFFER_SIZE);
    inputBuffer[result] = 0;
    interpretCommand(inputBuffer, result);
  }
}

void interpretCommand(char* inputBuffer, byte commandLength) {
  if (inputBuffer[0] == 'P') {
    positionArm(&inputBuffer[0]);
  } else if (inputBuffer[0] == 'H') {
    homePositionArm();
  } else if (inputBuffer[0] == '0') {
    BraccioRobot.powerOff();
    Serial.println("OK");
  }  else if (inputBuffer[0] == '1') {
    BraccioRobot.powerOn();
    Serial.println("OK");
  } else {
    Serial.println("E0");
  }
  Serial.flush();
}

void
positionArm(char *in) {
  int speed = armPosition.setFromString(in);
  if (speed > 0) {
    BraccioRobot.moveToPosition(armPosition, speed);
    Serial.println("OK");
  } else {
    Serial.println("E1");
  }
}

void
homePositionArm() {
  BraccioRobot.moveToPosition(armPosition.set(90, 90, 90, 90, 90, 73), 100);
  Serial.println("OK");
}


 
