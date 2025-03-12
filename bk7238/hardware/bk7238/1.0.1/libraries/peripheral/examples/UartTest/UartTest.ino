
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
}

//demo for read
//read one byte once time
//exmaple use sscom  send 0x01 will get hello world
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    String rev_str;
    while(Serial.available())
    {
      rev_str += (char)Serial.read();
    }
  
    Serial.print("Received : ");
    Serial.println(rev_str);

    Serial.write("hello world");
    Serial.flush();
    
  }
} 
