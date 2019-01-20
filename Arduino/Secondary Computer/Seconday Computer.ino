// UAT SPACE M2 FLIGHT COMPUTER MAIN PROGRAM
// Copyright (c) 2018, UAT Space. All rights reserved.
// Copyrights licensed under the Apache-2.0 License.
// Author : Brandon Nay, branay@uat.edu, linkedin.com/in/brandon-nay/

// NOTE: The SparkFunCCS811 library has a compile error on its end.
//       To fix this, comment out line 61:64 inside the cpp source.

#include <SparkFunCCS811.h>   // Air quality sensor on the environmental combo board
#include <SparkFunBME280.h>   // Pressure sensor on the environmental combo board
#include <Adafruit_Sensor.h>  // General library required by Adafruit sensors
#include <Adafruit_LSM9DS1.h> // Gyro board (gyro, accelerometer, magnetometer)
#include <SD.h>               // SD
//#include <Wire.h>           // disabled unless needed (I2C related)

#define PRINTER   Serial2	    // Serial for debugging :: Serial2 for flight
#define solarPanel      5     // Analog pin 5
#define wind1           4     // Analog pin 5
#define wind2           3     // Analog pin 5 
#define SDCS            8     // CHECK THAT THIS IS ACCURATE (SD Shield)
// STILL NEED ANALOG READ FOR SOLAR AND WIND TURBINES

// I2C
Adafruit_LSM9DS1 gyro = Adafruit_LSM9DS1(); // create 'Adafruit_LSM9DS1' object named 'gyro'
BME280 bme;                                 // define 'BME280'           object named 'bme'

// SD
File data;	// always open - write with data.print("") - save with data.flush()
String logString;

////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void createLog();
void printError(int);
void readBME();
void readGyro();
void startBME();	// can throw error 1
void startDebug();
void startGyro();	// can throw error 2
void startSD();		// can throw error 3 4 5

////////////////////////////////////////////////////////////////////////////////////////////
// SETUP ///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  if (PRINTER == Serial) startDebug();
  startSD();
  //  open data file - file remains open and is saved to via data.flush()
  data = SD.open("data.txt", FILE_WRITE);
  startGyro();
  startBME();
}

////////////////////////////////////////////////////////////////////////////////////////////
// LOOP ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  for(int i = 0; i < 50; i++){
    createLog();
    data.println(logString);
  }
  data.flush();
}

//  PRINTER.print(GPS.hour - 7);  // -7 accounts for arizona's timezone offset
//  PRINTER.print(GPS.minute);
//  PRINTER.print(GPS.seconds);
//  PRINTER.print(',');
//  PRINTER.print(GPS.latitudeDegrees, 4);    // 4 specifies 4 decimal places; default 2
//  PRINTER.print(',');
//  PRINTER.print(GPS.longitudeDegrees, 4);   // 4 specifies 4 decimal places; default 2
//  PRINTER.print(',');
//  PRINTER.print(GPS.altitude);
//  PRINTER.print(',');
//  readBME();
//  PRINTER.print(',');
//  readGyro();
//  PRINTER.println();

//  Retrievable GPS data formats

//  GPS.fix
//  GPS.hour
//  GPS.minute
//  GPS.seconds
//  GPS.milliseconds
//  if (GPS.fix)
//  {
//    GPS.latitude
//    GPS.lat
//    GPS.longitude
//    GPS.lon
//    GPS.latitudeDegrees   // works with google maps
//    GPS.longitudeDegrees  // works with google maps
//    GPS.speed
//    GPS.altitude
//  }

////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void createLog()
{
  gyro.read();
  sensors_event_t a, m, g, temp;
  gyro.getEvent(&a, &m, &g, &temp);

  logString = "";

  logString += bme.readTempF();
  logString += ',';
  logString += bme.readFloatPressure();
  logString += ',';
  logString += bme.readFloatAltitudeMeters();
  logString += ',';
  logString += bme.readFloatHumidity();
  logString += ',';
  logString += analogRead(solarPanel);
  logString += ',';
  logString += analogRead(wind1);
  logString += ',';
  logString += analogRead(wind2);
  logString += ',';
  logString += a.acceleration.x;
  logString += ',';
  logString += a.acceleration.y;
  logString += ',';
  logString += a.acceleration.z;
  logString += ',';
  logString += m.magnetic.x;
  logString += ',';
  logString += m.magnetic.y;
  logString += ',';
  logString += m.magnetic.z;
  logString += ',';
  logString += g.gyro.x;
  logString += ',';
  logString += g.gyro.y;
  logString += ',';
  logString += g.gyro.z;
  return;
}

void printError(int n)
{
  PRINTER.println();
  PRINTER.print(">>ERROR: ");
  PRINTER.println(n);
  return;
}

void readBME()
{
  // always read temp first - reading temp triggeres all sensors to read
  PRINTER.print(bme.readTempF());
  PRINTER.print(',');
  PRINTER.print(bme.readFloatPressure());
  PRINTER.print(',');
  PRINTER.print(bme.readFloatAltitudeMeters());
  PRINTER.print(',');
  PRINTER.print(bme.readFloatHumidity());
}

void readGyro()
{
  // Getting event data:

  gyro.read();
  sensors_event_t a, m, g, temp;
  gyro.getEvent(&a, &m, &g, &temp);

  PRINTER.print(a.acceleration.x);
  PRINTER.print(',');
  PRINTER.print(a.acceleration.y);
  PRINTER.print(',');
  PRINTER.print(a.acceleration.z);
  PRINTER.print(',');
  PRINTER.print(m.magnetic.x);
  PRINTER.print(',');
  PRINTER.print(m.magnetic.y);
  PRINTER.print(',');
  PRINTER.print(m.magnetic.z);
  PRINTER.print(',');
  PRINTER.print(g.gyro.x);
  PRINTER.print(',');
  PRINTER.print(g.gyro.y);
  PRINTER.print(',');
  PRINTER.print(g.gyro.z);

  // Access event data the following ways:

  // Acceleration (M/s^2)
  // a.acceleration.x
  // a.acceleration.y
  // a.acceleration.z

  // Magnetometer (gauss)
  // m.magnetic.x
  // m.magnetic.y
  // m.magnetic.z

  // Gyroscope (degrees per second)
  // g.gyro.x
  // g.gyro.y
  // g.gyro.z
}

void startBME()
{
  if (!bme.beginI2C())
  {
    printError(1);
    return;
  }
}

void startDebug()
{
  Serial.begin(9600);
  while (!Serial);
}

void startGyro()
{
  if (!gyro.begin())
  {
    printError(2);
    return;
  }

  gyro.setupAccel(gyro.LSM9DS1_ACCELRANGE_2G);
  //gyro.setupAccel(gyro.LSM9DS1_ACCELRANGE_4G);
  //gyro.setupAccel(gyro.LSM9DS1_ACCELRANGE_8G);
  //gyro.setupAccel(gyro.LSM9DS1_ACCELRANGE_16G);

  gyro.setupMag(gyro.LSM9DS1_MAGGAIN_4GAUSS);
  //gyro.setupMag(gyro.LSM9DS1_MAGGAIN_8GAUSS);
  //gyro.setupMag(gyro.LSM9DS1_MAGGAIN_12GAUSS);
  //gyro.setupMag(gyro.LSM9DS1_MAGGAIN_16GAUSS);

  //gyro.setupGyro(gyro.LSM9DS1_GYROSCALE_245DPS);
  gyro.setupGyro(gyro.LSM9DS1_GYROSCALE_500DPS);
  //gyro.setupGyro(gyro.LSM9DS1_GYROSCALE_2000DPS);
}

void startSD()
{
  pinMode(SDCS, OUTPUT);

  // attempt to start SD card up to 3 times
  for (int i = 1; i <= 3; i++)
  {
    if (SD.begin(SDCS)) break;
    if (i == 3)
    {
      printError(3);
      return;
    }
  }

  //====CHECK FOR DATA FILE====//
  if (SD.exists("data.txt"))
  {
    // if dataDump doesn't exist, create it
    if (!SD.exists("dataDump.txt"))
    {
      File f = SD.open("dataDump.txt");
      f.close();
    }

    // copy content of data to dataDump
    File readFile = SD.open("data.txt", FILE_READ);
    File writeFile = SD.open("dataDump.txt", FILE_WRITE);
    if ((readFile) && (writeFile))
    {
      writeFile.println(" ");
      writeFile.println("NEW DUMP");
      writeFile.println(" ");
      byte data;
      while (readFile.available())
      {
        writeFile.write(readFile.read());
      }

      readFile.close();
      writeFile.close();
    }
    else
    {
      printError(4);
      return;
    }

    // remove original data file
    if (!SD.remove("data.txt"))
    {
      printError(5);
      return;
    }
  }

  // if the data file doesn't exist, create it
  if (!SD.exists("data.txt"))
  {
    File f = SD.open("data.txt", FILE_WRITE);
    f.close();
  }
  return;
}
