#include <Kalman.h>
#include <AFMotor.h>

/*
   CNU Bottom-up(2020) 공모전
   초음파 3D 스캐너 아두이노IDE 코드

   made by : team 음파음파 (hj-k66 , hotmosit)
*/

int END_DISTANCE = 15; // 센서와 완판 끝의 거리 

AF_Stepper stepper1(100, 1);
AF_Stepper stepper2(200, 2);

double filteredDistance; // 필터링된 거리 
Kalman myFilter(0.125, 32, 1023, 0);

// 초음파 핀
int echoPin = 10;
int trigPin = 13;

float angle = 0;  // 원판 각도
float height = 0; // 센서 높이
unsigned long count = 0; // 측정 횟수 

int TF = 0;    // 스캔 물체 미탐지 여부 확인 변수. 200일 경우 스캔 종료 
int flag = 0;  // 칼만 필터링을 위한 변수. 총 10번 같은 위치 측정
int flag2 = 0; // 센서 측정 오작동 예외 변수

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
    /*
     * state == 'f'인 경우 아두이노의 초음파 센서 조정
     * 최초 초음파 센서가 원판보다 위에 있는 경우 센서 위치 조정을 위한 코드
     * 원판을 인식할 때 까지 센서를 0.07cm 씩 아래로 내려가게 한다
     * 원판을 인식한 경우 state 값을 '0'으로 변경된다.
     */

    flag2++;  // 최초 측정시 1~3회 노이즈 발생, 예외 처리 변수


    if ( flag2 > 4) {
      
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
    /*
     * state == '0'인 경우 아두이노의 초음파 센서 조정
     * 원판을 인식하는 경우 모터를 한바퀴 (0.07cm)만큼 올림
     * 더 이상 원판을 인식 하지 않을때 까지 모터가 돌아감
     */
    flag2++;  // 측정시 최초 1~3회 노이즈 발생, 예외 처리 위함

    if (flag2 > 4) {

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
    /*
     * state == '1' 은 start scanning 버튼의 입력을 기다리는 상태
     * processing 에서 시리얼 통신을 통해 값을 받는다.
     * state의 값이 변경된다. 값이 '2'가 되는 경우 스캔
     */
     
    if (Serial.available() > 0) {
      state = Serial.read();
    }
  }

  if (state == '2' && distance <= 100 ) {
    /*
     * state == '2'는 측정을 하는 코드.
     * distance <= 100 조건문은 센서의 오작동시 발생하는 값이 1000 이상이므로 예외처리르 위함
     * 실 거리가 100 이하인 경우만 측정을 실시한다. 
     */

    
    // 카르만 필터를 이용하여 노이즈 제거
    if (flag > 0) { // 첫 측정시 노이즈 발생하여 flag가 1 이상일때부터 측정
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
        else if (height >= 4.8 || TF <= 0) {
          // 최대 높이에 도달하거나 더이상 스캔할 물체가 탐지 되지 않는 경우 종료
          // 종료 조건으로 processing에 0,0,1000,0 을 전달한다.

          Serial.println(0); //거리
          Serial.println(0); //각도
          Serial.println(1000);//높이
          Serial.println(0);

          while (true); // 더이상 진행을 멈추기 위한 코드
        }
      }

      // 다음 측정을 위해 flag 값을 0으로 변경
      flag = 0;
    }

    if (Serial.available() > 0) {
      // 스캔 도중의 'pause' 버튼이나 'exit' 버튼의 명령을 기다리기 위함
      state = Serial.read();
    }
  }

  delay(50);
}
