
void initial_data_from_firebase() {
  time_fish_fire1 = Firebase.getString("setting/timeFish1");
  if (Firebase.failed()) {
    Serial.print("firebase.setString failded!!!");
    Serial.println(Firebase.error());
    return;
  }
  delay(400);
  time_fish_fire2 = Firebase.getString("setting/timeFish2");
  if (Firebase.failed()) {
    Serial.print("firebase.setString failded!!!");
    Serial.println(Firebase.error());
    return;
  }
  delay(400);
  if (Firebase.getString("setting/auto") == "1") isAutoFire = true;
  else isAutoFire = false;

  if (Firebase.failed()) {
    Serial.print("firebase.setString failded!!!");
    Serial.println(Firebase.error());
    return;
  }

  delay(400);
  Serial.println(">>>>>>>>>>>>>>");
  Serial.print("time fish fire1 = ");
  Serial.print(time_fish_fire1);
  Serial.print(", time fish fire2 = ");
  Serial.println(time_fish_fire2);
  Firebase.setInt("arduino_streaming", 7);
  delay(100);
  step_fish = 1;
}


// set package data
//        values = "";
//        values += "{\"time\":";
//        values += "\"";
//        values += String(time_sensor);
//        values +=  "\",\"temp\":";
//        values +=  "\"";
//        values += String(t);
//        values += ",\"humid\":";
//        values += "\"";
//        values += String(h);
//        values +=  "\",\"light\":";
//        values +=  "\"";
//        values += String(light_sensor_value);
//        values += "\"}";
