#include "heltec.h"
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <ESP32Servo.h>
#include "Wire.h"
#include <NTPClient.h>
#include <WiFiUdp.h>


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

Servo myservo;  // create servo object to control a servo
short pos = 90;    // variable to store the servo position
String time_fish_fire1 = "6:30";
String time_fish_fire2 = "17:30";
short step_fish = 1;
String time_fish = "6:00";
String formattedDate;
String dayStamp;
String timeStamp;


unsigned short counter = 1;
bool previousCycle = 0;
bool isAuto = true;
bool button_water_state = 0;
bool btn_is_auto_status = LOW;
bool servo_previous = LOW;
bool btn_servo_status = LOW;
bool haveWifi = LOW;
bool isAutoFire = true;
unsigned long previousMil = 0;
unsigned long countCycleMil = 0;
unsigned int timeSubtract = 1500000;
bool previous = 0;
String values = "";

#define FIREBASE_HOST "project-x-23d73-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "2IuejmsdoYAX3vxR78PIocqgilsAQkHUXCj9gomD"


#define dhtpin 17
#define PUMP 12
#define servoPin  14
#define BUTTON_WATER 27
#define BUTTON_PUMP 26
#define BUTTON_IS_AUTO 25
#define BUTTON_SERVO 23

//#define WIFI_SSID "CEIT_ROBOT"
//#define WIFI_PASSWORD "qazplmwsxokn"

#define WIFI_SSID "XLGO-0AF7"
#define WIFI_PASSWORD "67021280"

//#define WIFI_SSID "Amazon"
//#define WIFI_PASSWORD "Ice@mazon"

void setup_variable(){
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
  myservo.write(pos);

//  pinMode(LED, OUTPUT);
//  digitalWrite(LED, HIGH);
  pinMode(PUMP, OUTPUT);
  pinMode(BUTTON_WATER, INPUT);
  pinMode(BUTTON_PUMP, INPUT);
  pinMode(BUTTON_IS_AUTO, INPUT);
  pinMode(BUTTON_SERVO, INPUT);
}
