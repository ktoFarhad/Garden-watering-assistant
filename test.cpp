#define SOIL1 4
#define SOIL2 5
#define SOIL3 6

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("ESP32-C3 Soil Sensor Test");
}

void loop() {
  int s1 = analogRead(SOIL1);
  int s2 = analogRead(SOIL2);
  int s3 = analogRead(SOIL3);

  Serial.print("Soil 1: ");
  Serial.print(s1);

  Serial.print(" | Soil 2: ");
  Serial.print(s2);

  Serial.print(" | Soil 3: ");
  Serial.println(s3);

  delay(1000);
}
