#include<Servo.h>
Servo ESC1, ESC2;

void setup() {
  ESC1.attach(2);  
  ESC2.attach(3);

  ESC1.writeMicroseconds(1550);
  ESC2.writeMicroseconds(1550);
  //delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

}
