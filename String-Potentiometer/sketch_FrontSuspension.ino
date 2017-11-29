#include <SD.h>

#include <SPI.h>

#include <Wire.h>

//String Potentiometer testing apparatus for Front Suspension.
//Jacob Springer

File myFile;

const char TAB = '\t';
const int buttonPin = 2;

int buttonState = 0;
int cal0 = 0;
int cal1 = 0;
int cal2 = 0;
int cal3 = 0;
int cvt1 = 0;
int cvt2 = 0;

void setup() {
  Wire.begin(); //join i2c bus (address optional for master)
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600); //initialize serial communication at 9600 bits per second
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}


void loop() {
  
  
  int sensor0 = analogRead(A0); //read the input on analog pin 0
  int sensor1 = analogRead(A1);
  int sensor2 = analogRead(A2);
  int sensor3 = analogRead(A3);
  Wire.requestFrom(8,8); //Request 8 bytes from slave arduino
  while (Wire.available()) {
    int cvt1 = Wire.read(); //CVT primary
  }
  Wire.requestFrom(9,8);//Requests 8 bytes from 9
  while (Wire.available()) {
    int cvt2 = Wire.read(); //CVT secondary
  }

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    cal0 = analogRead(A0);
    cal1 = analogRead(A1);
    cal2 = analogRead(A2);
    cal3 = analogRead(A3);
    delay(200);
  }

  Serial.print(sensor0-cal0); //print out the value obtained from pots
  Serial.print(TAB); //Used to divide data sets
  Serial.print(sensor1-cal1);
  Serial.print(TAB);
  Serial.print(sensor2-cal2);
  Serial.print(TAB);
  Serial.print(sensor3-cal3);
  Serial.print(TAB);
  Serial.print(cvt1); //println creates a new line
  Serial.print(TAB);
  Serial.println(cvt2);
  
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(sensor0-cal0);
    myFile.print(TAB);
    myFile.print(sensor1-cal1);
    myFile.print(TAB);
    myFile.print(sensor2-cal2);
    myFile.print(TAB);
    myFile.println(sensor3-cal3);
    myFile.print(TAB);
    myFile.print(cvt1);
    myFile.print(TAB);
    myFile.println(cvt2);

    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
    }

  delay(50); //delay in between reads for stability
}


