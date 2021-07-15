
#include "my_variable.h"
#include "images.h"
#include "my_devices.h"
#include "wifi_setup.h"
#include "firebase_method.h"
//#include "ntp_server.h"


void setup()
{
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


void loop()
{
  read_light();
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

    //   send every 10 min 600000
    if (millis() - previousMil > 60000) {
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

    // control FROM control-box
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



}
