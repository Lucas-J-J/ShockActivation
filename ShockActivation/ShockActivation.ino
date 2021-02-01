// SD card libraries
#include <SPI.h>
#include <SD.h>
const int chipSelect = 4;

// Gryro libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
//#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

// Assigns aunique ID to the sensor
//Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(1);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(2);

// Red: 5V
// Black: GND
int RL = A0; // Rear Left: A0
int FL = A1; // Front Left: A1
int FR = A2; // Front Right: A2
int RR = A3; // Rear Right: A3
//int TestDuration = 60*1000; // ms
int RLVal; int FLVal; int FRVal; int RRVal;


int initialTime;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
  {;}

  // SD card set up
  Serial.print("Initialize SD card...");

  // SD Card initialization
  // See if the card is present and can be initialized:
  if(!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or nor present");
    while(1);
  }
  Serial.println("card initialized");

  Serial.println("Sensor part");
  // Adafruit sensor initialization
  /*if(!gyro.begin())
  {
    Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }*/
  if(!accel.begin())
  {
    Serial.print("Ooops, no acel detected");
    while(1);
  }
  
  Serial.println("Time(ms) \tValue");

  initialTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  RLVal = analogRead(RL); FLVal = analogRead(FL);
  FRVal = analogRead(FR); RRVal = analogRead(RR);

  String dataString = CreateDataString();

  File dataFile = SD.open("DataFile.txt", FILE_WRITE);

  // if the file is available, write to it:
  if(dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    //Print to serial monitor
    Serial.println(dataString);
  } else
  {
    // if the file can't open, pop up an error
    Serial.println("error opening");
  }

 // if(millis() > TestDuration)
   // delay(30*1000);
}

String CreateDataString()
{
  String dataString = "";

  // Append data
  dataString += String(millis()-initialTime) + "\t";
  dataString += String(RLVal) + "\t";
  dataString += String(FLVal) + "\t";
  dataString += String(FRVal) + "\t";
  dataString += String(RRVal) + "\t";

  sensors_event_t event;
  /*gyro.getEvent(&event);
  dataString += String(event.gyro.x) + "\t";
  dataString += String(event.gyro.y) + "\t";
  dataString += String(event.gyro.z) + "\t";
  */
  accel.getEvent(&event);
  dataString += String(event.acceleration.x) + "\t";
  dataString += String(event.acceleration.y) + "\t";
  dataString += String(event.acceleration.z) + "\t";
  return dataString;
}
