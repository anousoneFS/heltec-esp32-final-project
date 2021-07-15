

void control_servo() {
  for (pos = 90; pos >= 30; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }

  for (pos = 30; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
}

String get_time() {
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);

  String Year = dayStamp.substring(0, 4);
  String Month = dayStamp.substring(5, 7);
  String Day = dayStamp.substring(dayStamp.length() - 2, dayStamp.length());
  my_date_format = Day + "-" + Month + "-" + Year + " " + timeStamp;


  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  delay(1000);
  time_fish = formattedDate.substring(splitT + 1, formattedDate.length() - 4);

  if (time_fish.substring(0, 1) == "0") {
    time_fish = time_fish.substring(1, time_fish.length());
    if (time_fish.substring(time_fish.length() - 2, time_fish.length() - 1) == "0") {
      time_fish = time_fish.substring(0, 1) + time_fish.substring(time_fish.length() - 1, time_fish.length() - 1);
    }
  }

  Serial.print("time fish = ");
  Serial.println(time_fish);
  return my_date_format;
}

void display_sensor_value() {
  Heltec.display -> clear();
  Heltec.display -> drawString(38, 0, "CEIT-FARM");
  Heltec.display -> drawString(0, 10, "time:");
  Heltec.display -> drawString(24, 10, get_time());
  if (isAutoFire)
    Heltec.display -> drawString(8, 20, "ON");
  else
    Heltec.display -> drawString(8, 20, "OFF");

  Heltec.display -> drawString(36, 20, time_fish_fire1 + " | " + time_fish_fire2);

  Heltec.display -> drawString(0, 30, "TAir:");
  Heltec.display -> drawString(36, 30, String(temperature));
  Heltec.display -> drawString(0, 40, "humid:");
  Heltec.display -> drawString(36, 40, String(humidity));
  Heltec.display -> drawString(0, 50, "light:");
  Heltec.display -> drawString(36, 50, String(light));

  Heltec.display -> drawString(68, 30, "TWater:");
  Heltec.display -> drawString(107, 30, "28");
  Heltec.display -> drawString(68, 40, "PH:");
  Heltec.display -> drawString(107, 40, "1.2");
  Heltec.display -> drawString(68, 50, "EC:");
  Heltec.display -> drawString(107, 50, "4");

  Heltec.display -> display();

  Serial.print("btn is auto = ");
  Serial.println(digitalRead(BUTTON_IS_AUTO));
  Serial.print("btn pump = ");
  Serial.println(digitalRead(BUTTON_PUMP));
  Serial.print("btn servo = ");
  Serial.println(digitalRead(BUTTON_SERVO));
  Serial.print("time fish = ");
  Serial.println(time_fish);
}

float RawToLux(float raw)
{
  float logLux = raw * logRange / rawRange;
  return pow(10, logLux);
}

void read_light(){
  float rawValue = analogRead(LIGHT_PIN);
  light = int(RawToLux(rawValue));

  Serial.print("Raw = ");
  Serial.print(rawValue);
  Serial.print(" - Lux = ");
  Serial.println(light);
}

String read_sensor() {
  read_light();

  float h;
  float t;
  do  {
    delay(100);
    h = dht.readHumidity();
    t = dht.readTemperature();
    delay(2100);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
    } else {
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.println("");
      delay(100);
      temperature = t;
      humidity = h;
    }

  } while (isnan(h) || isnan(t));

  values = "";
  values += String(get_time()) + "," + String(t) + "," + String(h) + "," + "4" + "," + "2" + "," + "25" + "," + String(light) + "," + String(counter);
  counter++;
  return values;

}
