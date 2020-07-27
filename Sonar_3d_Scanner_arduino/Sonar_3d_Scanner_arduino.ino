#include <AFMotor.h>


AF_Stepper stepper1(200, 1);
AF_Stepper stepper2(200, 2);

int echoPin = 10;
int trigPin = 13;

float angle = 0;
float distance = 0;
float height = 0;

boolean flag = false;

void setup() {
  Serial.begin(9600);
  stepper1.setSpeed(200);
  stepper2.setSpeed(200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);

  float distance = ((float)(340 * duration) / 10000) / 2;

  // move table 1.8 degree
  stepper1.step(1, FORWARD, SINGLE);
  angle += 1.8;``

  if (fmod(angle, (float)360) == 0) {
    stepper2.step(1, FORWARD, SINGLE);
    height += 1.8;
    flag = true;
  } else {
    flag = false;
  }

  Serial.println(distance);
  Serial.println((angle - 1.8));

  if (flag) {
    Serial.println( (height - 1.8));
  } else {
    Serial.println(height);

    delay(1000);
  }
}
