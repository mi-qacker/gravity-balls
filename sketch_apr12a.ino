#include <Servo.h>
Servo myservo;
String x;
int i;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1);
  myservo.attach(8);
}

void loop() {
  while (!Serial.available());
  x = Serial.readString();
  i = x.toInt();
  delay(500);
  Serial.print(i);
  myservo.write(i);
  delay(5000);
//  myservo.write(0);
}
