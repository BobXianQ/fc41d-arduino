void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(0);
  analogReadResolution(10);
  Serial.print("adv_val:");
  Serial.println(val);
}
