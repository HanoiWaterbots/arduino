#include<Servo.h>
Servo ESC1, ESC2;

void setup() {
  ESC1.attach(10);  
  ESC2.attach(11);

 

  ESC1.writeMicroseconds(2000);
  ESC2.writeMicroseconds(2000);
  delay(500);

  ESC1.writeMicroseconds(2000);
  ESC2.writeMicroseconds(2000);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

}
