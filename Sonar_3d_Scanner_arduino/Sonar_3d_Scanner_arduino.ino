#include <Kalman.h>
#include <AFMotor.h>


int END_DISTANCE = 15;

AF_Stepper stepper1(100, 1);
AF_Stepper stepper2(200, 2);

double filteredDistance;
Kalman myFilter(0.125, 32, 1023, 0);

int echoPin = 10;
int trigPin = 13;

float angle = 0;
//float distance = 0;
float height = 0;
unsigned long count = 0;

int TF = 0;
int flag = 0;
int flag2 = 0;

// 상태
char state = 'f';

void setup() {
  Serial.begin(9600);
  stepper1.setSpeed(200);
  stepper2.setSpeed(200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Serial.println(state);

  delay(10);

  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);

  double distance = ((double)(340 * duration) / 10000) / 2;

  if ((int)distance == 0) {
    // 거리 측정에 오류가 있는 경우 예외처리
    // 고민이 더 필요한 해봐야하는 코드
    distance = 100;
  }

  if (distance > 100) {
    distance = 100;
  }

  if (state == 'f') {

    flag2++;


    if ( flag2 > 4) {
      //  Serial.println(distance);
      if (distance > 15) {
        stepper2.step(1, FORWARD, SINGLE);
        Serial.println(0);
      } else {
        Serial.println(0);
        state = '0';
        flag2 = 0;
      }
    }
  }


  if (state == '0') {
    // state == '0'인 경우 아두이노의 초음파 센서 조정
    // 원판을 인식하는 경우 모터를 한바퀴 (0.07cm)만큼 올림
    // 더 이상 원판을 인식 하지 않을때 까지 모터가 돌아감
    flag2++;

    if (flag2 > 4) {
      //   Serial.println(distance);
      if (distance < 15) {
        // 원판을 인식하는 경우
        // 원판을 인식하였으므로 processing에 0을 전달함
        stepper2.step(1, BACKWARD, SINGLE);

        Serial.println(0);

      }  else {
        // 원판을 더이상 인식하지 않는 경우
        // 원판이 더이상 인식하지 않으므로 processing에 1을 전달함
        stepper2.step(1, BACKWARD, SINGLE);
        Serial.println(1);
        state = '1';
        flag2 = 0;
      }
    }
  }

  if (state == '1') {
    // start scanning 버튼의 입력을 기다리는 상태
    // processing 에서 시리얼 통신을 통해 값을 받는다.
    // state의 값이 변경된다. 값이 '2'가 되는 경우 스캔
    if (Serial.available() > 0) {
      state = Serial.read();
    }
  }

  if (state == '2' && distance <= 100 ) {

    // 카르만 필터를 이용하여 노이즈 제거

    if (flag > 0) {
      filteredDistance = myFilter.getFilteredValue(distance);
    }

    flag++;


    // 같은 위치를 10번 측정하여 필터링 된 값을 이용하여 스캔에 반영한다.
    if (flag > 10) {
      Serial.println(filteredDistance);
      Serial.println(angle);
      Serial.println(height);
      Serial.println(distance);

      // 원판 모터를 1 스텝마다 1.8도 돌아간다.
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
        else if (height >= 5.8 || TF <= 0) {
          // 최대 높이에 도달하거나 더이상 스캔할 물체가 탐지 되지 않는 경우 종료
          // 종료 조건으로 processing에 0,0,1000 을 전달한다.
          
          Serial.println(0); //거리
          Serial.println(0); //각도
          Serial.println(1000);//높이

          while (true); // 더이상 진행을 멈추기 위한 코드
        }
      }

      // 필터링 테스트
      flag = 0;
    }

    if (Serial.available() > 0) {
      state = Serial.read();
    }
  }

  delay(50);
}
