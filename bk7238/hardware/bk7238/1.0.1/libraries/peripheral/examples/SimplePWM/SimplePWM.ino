/*
 * Only pins 6, 7, 8, 9, 24, 26 are mapped to PWM 
 */
#define analogPin 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(analogPin,OUTPUT);
  Serial.println("PWM Test Start.");
}

void loop() {
  // put your main code here, to run repeatedly:
  //set the resolution ,all pwm share one resolutiondefault is 8 bits, it can be set 8-12
  analogWriteResolution(8);
  //the default max_val is 255 
  analogWrite(analogPin,204);
}
