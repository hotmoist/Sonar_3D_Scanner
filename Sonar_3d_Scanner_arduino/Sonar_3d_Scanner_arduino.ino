#include <AFMotor.h>

AF_Stepper stepper1(200, 1);
AF_Stepper stepper2(200, 2);

int ex;

int echoPin = 10;
int trigPin = 13;

float angle = 0;
float distance = 0;
float height = 0;
unsigned long count = 0;
int TF = 0;
int flag = 0;

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
  
  flag++;
  if (flag > 2) {
    Serial.println(distance);
    Serial.println(angle);
    Serial.println(height);
    Serial.println(TF);

    // move table 1.8 degree
    stepper1.step(1, FORWARD, SINGLE);
    angle += 1.8;
    count++;

    if (distance < 50) {
      TF++;
    }

    if (count % 200 == 0) { //360 = 1.8*200
      if (TF > 0) {
        stepper2.step(1, BACKWARD, SINGLE);
        height += 0.07;
      }
      else {
        //Serial.println("end");
        Serial.println(0); //거리
        Serial.println(0); //각도
        Serial.println(1000);//높이
        while (true);
      }
    }
  }
  delay(250);

}
