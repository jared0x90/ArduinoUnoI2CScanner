/*
   Jared's Arduino Uno R3 i2c Scanner

   Notes
   -----
   Set serial to 115200
   Tested on the Arduino Uno R3 and several clones.

   i2c Pins
   --------
   SDA = A4
   SCL = A5

   inspired by Circuit Magic's
   http://www.circuitmagic.com/arduino/oled-display-i2c-128x64-with-arduino-tutorial/

  Copyright (c) 2016 Jared De Blander
  jared@deblander.org

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.   
*/

#include <Wire.h>
#define SCAN_DELAY_SECONDS    5

void setup()
{
  Wire.begin();
  Serial.begin(115200);
}

void loop()
{
  bool found[128];

  byte error;
  byte address;

  int error_count;
  int device_count;


  error_count = 0;
  device_count = 0;
  /*
     Scan from address 0x01 to 0x7F.
     Address 0x00 is the general call address and should not be scanned
     http://www.i2c-bus.org/addressing/general-call-address/
  */

  Serial.println("Scanning i2c from 0x01 to 0x7F...");

  for (address = 0x01; address <= 0x7F; address++ ) {
    /*
       Assume no device at this address
    */
    found[address] = false;

    Serial.print("Trying 0x");
    if (address < 16) Serial.print("0");
    Serial.print(address, HEX);
    Serial.print("... ");
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      /*
         A device was found. Record device found at this address
      */
      found[address] = true;
      Serial.print("i2c device found! Address: 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      device_count++;
    } else if (error == 4) {
      error_count++;
      Serial.print("Unknow error! Address: 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
    } else {
      Serial.print("Nothing found. Code: 0x");
      Serial.print(error, HEX);
    }
    Serial.println();
  }
  Serial.println("--------------------------------------");
  if (device_count == 0) {
    Serial.println("No i2c devices found");
  } else {
    Serial.print("i2c Device Count: ");
    Serial.println(device_count);
    Serial.println("Device listing: ");
    for (address = 1; address <= 127; address++ ) {
      if (found[address]) {
        Serial.print("0x");
        if (address < 16) Serial.print("0");
        Serial.println(address, HEX);
      }
    }
  }
  Serial.println("--------------------------------------");
  if (error_count == 0) {
    Serial.println("No errors detected this scan.");
  } else {
    Serial.print("Error count: ");
    Serial.println(error_count);
  }
  Serial.println("--------------------------------------");
  Serial.print("Next scan in ");
  Serial.print(SCAN_DELAY_SECONDS);
  Serial.print(" seconds");
  for (int delay_counter = 0; delay_counter < SCAN_DELAY_SECONDS; delay_counter++) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
}
