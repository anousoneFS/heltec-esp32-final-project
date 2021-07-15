
#include "my_variable.h"
#include "images.h"
#include "my_devices.h"
#include "wifi_setup.h"
#include "firebase_method.h"
//#include "ntp_server.h"

void setup()
{
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(SERVO_PIN); // attaches the servo on pin 18 to the servo object
  myservo.write(pos);
  delay(100);

  setup_variable();
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);

  logo();
  delay(300);

  Heltec.display->clear();
  WIFISetUp();
  delay(3000);
  firebase_streaming();
  initial_data_from_firebase();
  dht.begin();
  get_time();
  read_sensor();
}

void control_servo() {
  for (pos = 90; pos >= 30; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(10);             // waits 15ms for the servo to reach the position
  }

  for (pos = 30; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(10);             // waits 15ms for the servo to reach the position
  }
}

void loop()
{

  display_sensor_value();

  // ---------- button auto on/off  ----------
  btn_is_auto_status = digitalRead(BUTTON_IS_AUTO);
  if (btn_is_auto_status == HIGH) {
    isAuto = HIGH;
  } else {
    isAuto = LOW;
  }
  // end button auto on/off

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  if (isAuto && isAutoFire) {
    // ------- control servo --------
    if (time_fish == time_fish_fire1) {
      if (step_fish == 1) {
        control_servo();
        step_fish = 2;
      }
    }

    if (time_fish == time_fish_fire2) {
      if (step_fish == 2) {
        control_servo();
        step_fish = 1;
      }
    }
    // end control servo

    // ---------- button water --------------
    button_water_state = digitalRead(BUTTON_WATER);
    if (button_water_state == HIGH && previousCycle == LOW) {
      countCycleMil = millis();
      Serial.println("in mode Auto");
    }
    previousCycle = button_water_state;

    if (millis() - countCycleMil > timeSubtract) {
      digitalWrite(PUMP, LOW);
      Serial.println("something went wrong.!!!");
    } else {
      digitalWrite(PUMP, button_water_state);
    }
    // end button water
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  } else {
    if (!isAuto) {

      // --------- control servo from button ----------
      btn_servo_status = digitalRead(BUTTON_SERVO);
      Serial.print("btn servo = ");
      Serial.println(btn_servo_status);
      if (btn_servo_status == HIGH && servo_previous == LOW) {
        control_servo();
      }
      servo_previous = btn_servo_status;
      // end control servo from button

      // --------- control pump from button ---------
      digitalWrite(PUMP, digitalRead(BUTTON_PUMP));
      // end control pump from button

    }
    Serial.println("in mode manual");
  }

  //   send every 10 min 600000
  if (millis() - previousMil > 120000) {
    // put package
    Firebase.setString("data_sensor_from_arduino/data_from_arduino", String(read_sensor()));
    // handle error
    if (Firebase.failed()) {
      Serial.print("firebase.setString failded!!!");
      Serial.println(Firebase.error());
      return;
    }
    previousMil = millis();
  }

}

// end void loop ------------------

void firebase_streaming() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.stream("arduino_streaming/", [](FirebaseStream stream) {
    Serial.println("Evant: " + stream.getEvent());
    Serial.println("Path: " + stream.getPath());
    Serial.println("Data: " + stream.getDataString());
    Serial.println(stream.getDataInt());
    if (stream.getEvent() == "put") {
      if (stream.getPath() == "/") {

        if (stream.getDataInt() < 2) {
          //            Serial.println("success leo");
          if (stream.getDataInt() && previous == 0) {
            if (!isAutoFire) {
              control_servo();
            }
          }
          previous = stream.getDataInt();
        } else {
          switch (stream.getDataInt()) {
            case 2:
              if (!isAutoFire) {
                //                  Serial.println("PUMP off");
                digitalWrite(PUMP, LOW);
              }
              break;
            case 3:
              if (!isAutoFire) {
                //                  Serial.println("PUMP on");
                digitalWrite(PUMP, HIGH);
              }
              break;
            case 4:
              if (btn_is_auto_status) {
                isAutoFire = false;
              } else {
                isAutoFire = true;
              }

              break;
            case 5:
              if (btn_is_auto_status) {
                isAutoFire = true;
              } else {
                isAutoFire = true;
              }
              break;
            case 6:
              initial_data_from_firebase();
              break;
            case 7:
              break;

            default:
              timeSubtract = stream.getDataInt();
              Serial.print("timeSubtract = ");
              Serial.println(timeSubtract);
          }
        }

      }
    }
  });

}
