
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
  
//  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
//  timeClient.setTimeOffset(25200);

  firebase_streaming();
}

String get_time(){
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
//  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
//  Serial.print("DATE: ");
//  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  delay(1000);
  time_fish = formattedDate.substring(splitT+1, formattedDate.length() - 4);
  return timeStamp;
}

void loop()
{

  Heltec.display -> clear();
  Heltec.display -> drawString(0, 20, "Hour : ");
  Heltec.display -> drawString(50, 20, get_time());
  Heltec.display -> display();
  //  counter++;

  Serial.print("btn is auto = ");
  Serial.println(digitalRead(BUTTON_IS_AUTO));
  Serial.print("btn pump = ");
  Serial.println(digitalRead(BUTTON_PUMP));
  Serial.print("btn servo = ");
  Serial.println(digitalRead(BUTTON_SERVO));
  Serial.print("time fish = ");
  Serial.println(time_fish);
  delay(200);

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

    delay(200);
    Serial.println("in mode manual");
  }


  // => Read dht sensor data
  //  delay(5000);
  //  Serial.println ("run in main loop");

  // send every 10 min
  //  if (millis() - previousMil > 600000) {
  //    previousMil = millis();
  //    float h;
  //    float t;
  //    do  {
  //      delay(100);
  //      h = dht.readHumidity();
  //      t = dht.readTemperature();
  //      delay(2100);
  //
  //      // Check if any reads faiPUMP and exit early (to try again).
  //      if (isnan(h) || isnan(t)) {
  //        Serial.println(F("FaiPUMP to read from DHT sensor!"));
  //      } else {
  //        Serial.print(F("Humidity: "));
  //        Serial.print(h);
  //        Serial.print(F("%  Temperature: "));
  //        Serial.print(t);
  //        Serial.println("");
  //        // set package data
  //        values = "";
  //        values += "{\"time\":";
  //        values += "\"";
  //        values += String(previousMil);
  //        values +=  "\",\"temp\":";
  //        values +=  "\"";
  //        values += String(t);
  //        values += ",\"humid\":";
  //        values += "\"";
  //        values += String(h);
  //        values += "\"}";
  //        // put package
  //        Firebase.setString("sensor-values/20-5-2021/" + String(n), values);
  //        // handle error
  //        if (Firebase.faiPUMP()) {
  //          Serial.print("setting /number faiPUMP:");
  //          Serial.println(Firebase.error());
  //          return;
  //        }
  //      }
  //    } while (isnan(h) || isnan(t));
  //
  //    n++;
  //    Serial.println(n);
  //  }


}
