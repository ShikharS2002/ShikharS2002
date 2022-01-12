/*
Programmer Name: - Shikhar Singh
Program Title: - Motion Sensor Email
*/
#include <WiFi.h>
#include <HTTPClient.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.3937

const char wifi_SSID[] = "Redmi";
const char wifi_PASS[] = "Shikhar@02";
int wifi_TIMEOUT = 20000;

const String Host_Name = "https://maker.ifttt.com/trigger/";
const String Motion_Event = "INTRUSION_DETECTED";
const String Sound_Event = "SOUND_DETECTED";
const String Path_Name = "/with/key/i8JvcJcAH4BRJ8gwBlBItMky-cKL-z_R1wH88hysZep";
String Query_String = "?value1=20&value2=40&value3=90";

HTTPClient http;

uint32_t duration = 0;
float distance_CM = 0;
float distance_INCH = 0;

uint8_t trig = 18;
uint8_t echo = 5;
uint8_t sound = 19;

uint8_t motion_doonce = 0;
uint32_t motion_time = 0;
uint32_t motion_timeout = 120000;

uint8_t sound_doonce = 0;
uint32_t sound_time = 0;
uint32_t sound_timeout = 120000;

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
  Serial.println(Host_Name + Event_Name + Path_Name + Query_String);

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

  pinMode(15, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(sound, INPUT);

  Serial.println(" ");
  Serial.println("-------------------------------------------------");
  Serial.print("Connecting to Wifi Network: ");
  Serial.println(wifi_SSID);

  connect_to_wifi();

  Serial.println("-------------------------------------------------");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trig, LOW);
  digitalWrite(echo, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance_CM = duration * SOUND_SPEED;
  distance_INCH = distance_CM * CM_TO_INCH;

  if(digitalRead(sound) == HIGH) {
    Serial.println("Sound Detected");
    sound_doonce = 1;
    send_email(Sound_Event);
    sound_time = millis();
  }

  if(distance_INCH <= 15.00 && motion_doonce == 0) {
    Serial.println("Intrusion Detected");
    motion_doonce = 1;
    send_email(Motion_Event);
    motion_time = millis();
  }

  if(millis() - motion_time >= motion_timeout) {
    motion_doonce = 0;
  }

  if(millis() - sound_time >= sound_timeout) {
    sound_doonce = 0;
  }
}
