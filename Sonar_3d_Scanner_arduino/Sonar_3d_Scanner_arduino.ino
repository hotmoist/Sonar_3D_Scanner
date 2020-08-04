#include <Kalman.h>
#include <AFMotor.h>


int END_DISTANCE = 15;

AF_Stepper stepper1(200, 1);
AF_Stepper stepper2(200, 2);

double filteredDistance;
Kalman myFilter(0.1, 32, 1023, 0);

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

  delay(50);

  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);

  double distance = ((double)(340 * duration) / 10000) / 2;

  delay(10);

  filteredDistance = myFilter.getFilteredValue(distance);

  flag++;

  if (flag <= 5) {
    stepper2.step(1, BACKWARD, SINGLE);
  }




  if (flag > 50) {

    /*
      Serial.print(distance);
      Serial.print(",");
    */
    Serial.println(filteredDistance);
    Serial.println(angle);
    Serial.println(height);
    //   Serial.println(TF);

    // move table 1.8 degree
    stepper1.step(1, FORWARD, SINGLE);
    angle += 1.8;
    count++;

    if (filteredDistance < END_DISTANCE) {
      TF++;
    }

    if (count % 200 == 0) { //360 = 1.8*200
      if (TF > 0) {
        stepper2.step(1, BACKWARD, SINGLE);
        height += 0.07;
        TF = 0;
      }
      else {
        //Serial.println("end");
        Serial.println(0); //거리
        Serial.println(0); //각도
        Serial.println(1000);//높이
        while (true);
      }
    }

    delay(10);
  }
}
