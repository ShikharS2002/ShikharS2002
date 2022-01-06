#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 5, 4, 3, 2);

int RS = 13;
int EN = 12;
int FAULT_DETEC = 10;
int BUZZER = 9;
int RELAY = 8;

float temp = 0, current = 0;
int bat_1 = 0, bat_2 = 0, bat_3 = 0;
float bat_1_v = 0.0, bat_2_v = 0.0, bat_3_v = 0.0;
float temp_v = 0.0, current_v = 0.0;
float temp_v1 = 0.0;

int bat_1_con, bat_2_con, bat_3_con;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(RS, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(FAULT_DETEC, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);

  lcd.begin(16, 4);

  digitalWrite(FAULT_DETEC, LOW);
  digitalWrite(BUZZER, LOW);

  digitalWrite(RELAY, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  bat_1 = analogRead(A3);
  bat_2 = analogRead(A4);
  bat_3 = analogRead(A5);
  temp = analogRead(A1);

  bat_3_v = ((float)bat_3/1023) * 5;
  bat_2_v = (((float)bat_2/1023) * 5) - bat_3_v;
  bat_1_v = ((((float)bat_1/1023) * 5) *2) - (bat_3_v + bat_2_v);
  temp_v = (((float)temp/1023) * 5);

  temp_v1 = ((5 - temp_v)/0.01) - 250;

  bat_1_con = (bat_1_v >= 1.95) ? 1 : 0;
  bat_2_con = (bat_2_v >= 1.95) ? 1 : 0;
  bat_3_con = (bat_3_v >= 1.95) ? 1 : 0;

  if(bat_1_v < 1.95 or bat_2_v < 1.95 or bat_3_v < 1.95) {
    digitalWrite(FAULT_DETEC, HIGH);
  }

  if((bat_1_v + bat_2_v + bat_3_v) > 5.00) {
    digitalWrite(RELAY, HIGH);
    current = analogRead(A2);
    current_v = (((((float)current/1023) * 5)- 2.5)/0.185) * 1000;
  }

  else {
    digitalWrite(RELAY, LOW);
    current_v = 0.0;
  }

  if(temp_v1 >= 40) {
    digitalWrite(BUZZER, HIGH);
  }

  else {
    digitalWrite(BUZZER, LOW);
  }

  lcd.setCursor(0, 0);
  lcd.print("Bat1  Bat2  Bat3");
 
  lcd.setCursor(0, 1);
  if(bat_1_con == 1) {
    lcd.print("OK");
  }

  else {
    lcd.print("LB");
  }

  lcd.print("     ");

  if(bat_2_con == 1) {
    lcd.print("OK");
  }

  else {
    lcd.print("LB");
  }

  lcd.print("     ");

  if(bat_3_con == 1) {
    lcd.print("OK");
  }

  else {
    lcd.print("LB");
  }
  

  lcd.setCursor(0, 2);
  lcd.print("Curr = ");
  lcd.print(current_v);
  lcd.print(" mA");

  lcd.setCursor(0, 3);
  lcd.print("Temp = ");
  lcd.print(temp_v1);

}