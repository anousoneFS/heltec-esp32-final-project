#include "heltec.h"
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <ESP32Servo.h>
#include "Wire.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHT.h"
//#include "DFRobot_PH.h"
//#include <EEPROM.h>
//
//
//DFRobot_PH ph;
//#define ESPADC 4096.0   //the esp Analog Digital Convertion value
//#define ESPVOLTAGE 3300 //the esp voltage supply value
//#define PH_PIN 2    //the esp gpio data pin number
float voltage, phValue = 2.4, temp = 32;


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
unsigned short counter = 0;
float rawRange = 4096;
float logRange = 5.0;



bool previousCycle = 0;
bool isAuto = true;
bool button_water_state = 0;
bool btn_is_auto_status = LOW;
bool servo_previous = LOW;
bool btn_servo_status = LOW;
bool haveWifi = LOW;
bool isAutoFire;
bool isBuzzerOn = true;
unsigned long previousMil = 0;
unsigned long countCycleMil = 0;
unsigned int timeSubtract = 1500000;
unsigned long time_buzzer_electric;
bool previous = 0;
String values = "";
String temperature;
String humidity;
unsigned short light;   // proberm
String my_date_format;
short step_buzzer = 1;
unsigned long time_song1 = 0;
unsigned long previous_buzzer = 0;

int freq = 2000;
int channel = 0;
int resolution = 8;

#define FIREBASE_HOST "project-x-23d73-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "2IuejmsdoYAX3vxR78PIocqgilsAQkHUXCj9gomD"


#define DHTPIN 17
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LIGHT_SENSOR_PIN 36
#define PUMP 12
#define SERVO_PIN  14
#define BUTTON_WATER 27
#define BUTTON_PUMP 26
#define BUTTON_IS_AUTO 25
#define BUTTON_SERVO 23
#define CHECK_ELECTRIC_PIN 34
#define BUZZER_PIN 33
#define LED_PIN 15



//#define WIFI_SSID "CEIT_ROBOT"
//#define WIFI_PASSWORD "qazplmwsxokn"

#define WIFI_SSID "XLGO-0AF7"
#define WIFI_PASSWORD "67021280"

//#define WIFI_SSID "Amazon"
//#define WIFI_PASSWORD "Ice@mazon"

void setup_variable() {
  //  pinMode(LED, OUTPUT);
  //  digitalWrite(LED, HIGH);
  pinMode(PUMP, OUTPUT);
  pinMode(BUTTON_WATER, INPUT);
  pinMode(BUTTON_PUMP, INPUT);
  pinMode(BUTTON_IS_AUTO, INPUT);
  pinMode(BUTTON_SERVO, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(CHECK_ELECTRIC_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
//  pinMode(PH_PIN, INPUT);

//  EEPROM.begin(32);//needed to permit storage of calibration value in eeprom
//  ph.begin();

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(BUZZER_PIN, channel);

}
