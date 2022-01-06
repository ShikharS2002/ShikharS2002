/* Programmer Name: - Shikhar Singh
   Class: - SE 7
   Batch: - F7
   Roll Number: - 22338

   Weather Monitoring System Code.

   Program Philosophy: - Snsors provide data which is stored in variables , the data is converted to a string and displayed on an lcd.
                         The strings are catenated into a long string and are sent serially to nodemcu module for iot interfacing.
*/


//--------Importing Libraries-----------------------//

#include <LiquidCrystal.h>   // LCD Library
#include <Adafruit_BMP085.h> // BMP Sensor Library
#include "DHT.h"             // DHT Sensor Library
#include <SoftwareSerial.h>
//--------------------------------------------------//


//----------Pin definitions and classes-------------------------------//

#define DHTPIN 8             // Defining Pin of DHT sensor
#define rain_pin 9           // Defining Pin of rain sensor

Adafruit_BMP085 bmp;                     // Initialising bmp class
DHT dht(DHTPIN, DHT11);                  // Initialising DHT class
LiquidCrystal lcd(13, 12, 5, 4, 3, 2);     // Initialising LCD class
SoftwareSerial myserial(10, 11);
//---------------------------------------------------------------------//


//--------------------Variable Definitions-----------------------------------------------------------//

char temp[16];        // Defining variables to store values of temp, reletive humidity and pressure.
char RH[16];
char pressure[16], pressure1[16];
int i;
//---------------------------------------------------------------------------------------------------//


//---------------------------------------------------------------------------------------------------//
//Main Code

void setup() {
  
  Serial.begin(9600);     // Defining serial baud rate
  myserial.begin(115200); //Defining NodeMCU communication baud rate
  lcd.begin(16, 4);       // Initialising LCD
  dht.begin();            // Initialising DHT sensor
  if (!bmp.begin()) {     // Checking for error in serial communication
    lcd.print("Error");
    while(1) {}           // If serial communication not started program terminates
  }
}

void loop() {
  delay(15000);          // Wait 15 second in between readings

  float temp_value = dht.readTemperature();    // Get value of temperature
  float RH_value = dht.readHumidity();         // Get value of relative humidity
  float pressure_value = bmp.readPressure();   // Get value of pressure
  int rain_value = digitalRead(rain_pin);  

  dtostrf(temp_value, 3, 1, temp);   // converting values of quantities into strings
  dtostrf(RH_value, 3, 1, RH);
  dtostrf(pressure_value, 3, 0, pressure);

  for (i = 0; i < 4; i++) {         // Discarding decimal point in pressure value
    pressure1[i] = pressure[i];
  }

  pressure_value = ((1000 * (pressure1[0] - 0x30) + 100 * (pressure1[1] - 0x30) + 10 *(pressure1[2] - 0x30) + (pressure1[3] - 0x30)));
  
  //-------------------Sending data to ESP8266----------------//
  myserial.print('*');                   //Printing start character
  myserial.print(temp_value, 0);         //Printing temp, RH and pressure values
  myserial.print(RH_value, 0);
  myserial.print(pressure_value, 0);
  myserial.println('#');                 //Printing end character 
  //---------------------------------------------------------//

  //----------------Printing data on LCD----------------------//
  lcd.setCursor(0, 0);   // Printing Temperature on LCD 
  lcd.print("Temp = ");
  lcd.print(temp);

  lcd.setCursor(0, 1);   // Printing Relative Humidity on LCD
  lcd.print("RH = ");
  lcd.print(RH);

  lcd.setCursor(0, 2);      // Printing Pressure on LCD
  lcd.print("Pressure = ");
  lcd.print(pressure1);

  lcd.setCursor(0, 3);     // Checking if rain is detected
  if(rain_value == HIGH) {
    lcd.print("Clear Weather");
  }
  else {
    lcd.print("Rainy Weather");
  }
}
//Program End