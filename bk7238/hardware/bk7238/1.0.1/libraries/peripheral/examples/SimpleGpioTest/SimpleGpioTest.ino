int TestPin = 6;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(TestPin,OUTPUT);
    digitalWrite(TestPin,LOW);
    Serial.println("GPIO TEST START.");
}

void loop() {

  digitalWrite(TestPin,HIGH);
  Serial.println("Test pin set high");
  delay(200);
  digitalWrite(TestPin,LOW);
  Serial.println("Test pin set low");
  delay(200);
}
