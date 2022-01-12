/*
Programmer Name: - Shikhar Singh
Program Title: - Motion Sensor Email
*/

#include <WiFi.h>
#include <HTTPClient.h>

const char wifi_SSID[] = "##########";
const char wifi_PASS[] = "##########";
int wifi_TIMEOUT = 20000;

String Host_Name = "https://maker.ifttt.com/trigger/";
String Path_Name = "/with/key/i8JvcJcAH4BRJ8gwBlBItMky-cKL-z_R1wH88hysZep";
String Query_String = "?value1=20&value2=40&value3=90";

HTTPClient http;

int motion = 0;
int sound = 0;
int sound_timeout = 2000;
int sound_doonce = 0;
int sound_start = 0;
int mail_timeout = 120000;
int doonce = 0;
int motion_start = millis();

void connect_to_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_SSID, wifi_PASS);
  int start_time = millis();

  while(WiFi.status() != WL_CONNECTED && (millis() - start_time != wifi_TIMEOUT)) {
    Serial.println(".");
    delay(1000);
  }

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Unable to Connect to Wifi");
    while(1);
  }

  else {
    Serial.println("Wifi Connected");
    Serial.print("SSID: ");
    Serial.println(wifi_SSID);
    Serial.print("IP Adress: ");
    Serial.println(WiFi.localIP());
  }
}

void send_email(String Event_Name) {
  int code;
  http.begin(Host_Name + Event_Name + Path_Name + Query_String);

  code = http.GET();

  if(code > 0) {
    Serial.println("File Found");

    if(code == HTTP_CODE_OK) {
      String res = http.getString();
      Serial.println(res);
    }

    else {
      Serial.printf("Http Code : ......... [GET]: %d\n", code);
    }
  }

  else {
    Serial.println("Failed to Send mail Error encountered");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(5, OUTPUT);
  pinMode(4, INPUT);

  doonce = 0;

  Serial.println(" ");
  Serial.println("-------------------------------------------------");
  Serial.print("Connecting to Wifi Network: ");
  Serial.println(wifi_SSID);

  connect_to_wifi();

  Serial.println("-------------------------------------------------");
}

void loop() {
  // put your main code here, to run repeatedly:
  motion = digitalRead(4);
  sound = digitalRead(33);

  if(motion == 0) {
    digitalWrite(5, LOW);
  }

  else {
    digitalWrite(5, HIGH);
    if(doonce == 0) {
      doonce = 1;
      motion_start = millis();
      send_email("MOTION_DETECTED");
    }
  }

  if(sound == 0) {
    digitalWrite(18, LOW);
  }
  
  else {
    digitalWrite(18, HIGH);
    if(sound_doonce == 0) {
      sound_doonce = 1;
      sound_start = millis();
      send_email("SOUND_DETECTED");
    }
  }

  if(doonce == 1 && millis() - motion_start == mail_timeout) {
    doonce = 0;
  }

  if(sound_doonce == 1 && millis() - sound_start == sound_timeout) {
    sound_doonce = 0;
  }
}
