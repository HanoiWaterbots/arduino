#include <SimpleTimer.h>

SimpleTimer timer;
int timerId;

void setup() {
  Serial.begin(9600);
  timerId = timer.setInterval(5000, sendData);
}

void loop() {
  timer.run();
}

void sendData() {
  Serial.println("start");
  Serial.println("lat:123");
  Serial.println("lng:456");
  Serial.println("pH:7");
  Serial.println("Temperature:30");
  Serial.println("DO:5");
  Serial.println("ORP:5");
  Serial.println("Conductivity:5");
  Serial.println("end");
}
