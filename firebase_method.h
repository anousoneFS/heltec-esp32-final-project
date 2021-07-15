void initial_data_from_firebase() {
  time_fish_fire1 = Firebase.getString("setting/timeFish1");
  if (Firebase.failed()) {
    Serial.print("firebase.setString failded!!!");
    Serial.println(Firebase.error());
    return;
  }
  delay(100);
  time_fish_fire2 = Firebase.getString("setting/timeFish2");
  if (Firebase.failed()) {
    Serial.print("firebase.setString failded!!!");
    Serial.println(Firebase.error());
    return;
  }
  delay(100);
  if (Firebase.getString("setting/auto") == "1") isAutoFire = true;
  else isAutoFire = false;

  if (Firebase.failed()) {
    Serial.print("firebase.setString failded!!!");
    Serial.println(Firebase.error());
    return;
  }

  delay(100);
  Serial.println(">>>>>>>>>>>>>>");
  Serial.print("time fish fire1 = ");
  Serial.print(time_fish_fire1);
  Serial.print(", time fish fire2 = ");
  Serial.println(time_fish_fire2);
  Firebase.setInt("arduino_streaming", 7);
  delay(100);
  step_fish = 1;
}


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
