/*
Programmer Name: - Shikhar Singh
Project: - Battery Monitoring System using Arduino
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 5, 4, 3, 2);  //Defining LCD pins.

int RS = 13;                            //Defining variables to store pins values.
int EN = 12;
int FAULT_DETEC = 10;
int BUZZER = 9;
int RELAY = 8;

float temp = 0, current = 0;            //Variables to store analog read values.
int bat_1 = 0, bat_2 = 0, bat_3 = 0;
float bat_1_v = 0.0, bat_2_v = 0.0, bat_3_v = 0.0;
float temp_v = 0.0, current_v = 0.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(RS, OUTPUT);                 //Defining pin modes for all input and output pins.
  pinMode(EN, OUTPUT);
  pinMode(FAULT_DETEC, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);

  lcd.begin(16, 2);

  digitalWrite(FAULT_DETEC, LOW);      //By default fault detector LED off, Buzzer off, Lamp disconnected to battery bank.
  digitalWrite(BUZZER, LOW);

  digitalWrite(RELAY, LOW);            //When relay is high the load is connected to the battery bank.
}

void loop() {
  // put your main code here, to run repeatedly:
  bat_3 = analogRead(A3);              //Reading analog values.
  bat_2 = analogRead(A4);
  bat_1 = analogRead(A0);
  temp = analogRead(A1);

  bat_1_v = ((float)bat_1/1023) * 5;   //Converting analog read values to voltages and temperature in degrees celcius.
  bat_2_v = ((((float)bat_2/1023) * 2) * 5) - bat_1_v;
  bat_3_v = ((((float)bat_3/1023) * 5) *2) - (bat_1_v + bat_2_v);
  temp_v = ((((float)temp/1023) * 5) / 0.01) - 50;

  if(bat_1_v < 2 or bat_2_v < 2 or bat_3_v < 2) {     //If any one battery faulty fault detector turned off.
    digitalWrite(FAULT_DETEC, HIGH);
  }
  
  if((bat_1_v + bat_2_v + bat_3_v) > 6.00) {          //If cumulative voltage is greater than 6 Volts load is connected and current is measured.
    digitalWrite(RELAY, HIGH);
    current = analogRead(A2);
    current_v = (((float)current / 1023) * 5);
  }

  else {                                              //If cumulative voltage less than 6V load disconnected and current set to 0.
    digitalWrite(RELAY, LOW);
    current_v = 0.0;
  }

  if(temp_v >= 40) {                                  //If temperature greater than 40 degrees buzzer activated.
    digitalWrite(BUZZER, HIGH);
  }

  else {                                              //Buzzer off when temperature less than 40 degrees.
    digitalWrite(BUZZER, LOW);
  }
   
  lcd.print("B1 B2 B3 T  Curr");                      //Printing first line of LCD.
  lcd.setCursor(0, 1);                                //Cursor set on second line.
  if(bat_1_v >= 2) {                                  //If battery voltage greater than 2V O is printed else L is printed.
    lcd.print("O  ");
  }
  else {
    lcd.print("L  "); 
  }
  
  if(bat_2_v >= 2) {
    lcd.print("O  ");
  }
  else {
    lcd.print("L  "); 
  }
  
  if(bat_3_v >= 2) {
    lcd.print("O  ");
  }
  else {
    lcd.print("L  "); 
  }
  
  lcd.print((int)temp_v);                             //Printing temperature value. 
  lcd.print(" ");
  lcd.print(current_v);                               //Printing current value.
}
