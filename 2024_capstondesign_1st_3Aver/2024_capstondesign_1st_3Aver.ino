/*
이 코드는 시설 및 채소학 연구실에서 진행한 "2024 캡스톤디자인 1학기-작물 재배에 활용을 위한 스마트 센서 모듈" 프로젝트의 코드입니다.

재료
스위치 1, 2
릴레이 1, 2
온습도센서(DHT11) 1,2,3
아두이노 Uno (R3) 및 브레드보드 +-
온습도센서와 연결할 긴 신호선(프로젝트의 경우 UTP케이블을 사용)

*/

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD 설정
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 첫 번째 DHT 센서 타입 및 핀 설정
#define DHTPIN1 2     // 온습도센서 연결위치
#define DHTTYPE DHT11   
DHT dht1(DHTPIN1, DHTTYPE);  

// 두 번째 DHT 센서 타입 및 핀 설정
#define DHTPIN2 3     // 온습도센서 연결위치
#define DHTTYPE DHT11   
DHT dht2(DHTPIN2, DHTTYPE);

// 세 번째 DHT 센서 타입 및 핀 설정
#define DHTPIN3 4     // 온습도센서 연결위치
#define DHTTYPE DHT11   
DHT dht3(DHTPIN3, DHTTYPE);

// SSR 핀 설정
#define SSR_PIN1 8  // SSR + 연결위치
#define SSR_PIN2 9  // SSR + 연결위치

void setup() {
  // 시리얼 모니터 시작
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  dht3.begin();

  // LCD 초기화
  lcd.init();
  lcd.backlight();
  
  // SSR 핀 출력으로 설정
  pinMode(SSR_PIN1, OUTPUT);
  digitalWrite(SSR_PIN1, LOW); // 초기 상태는 꺼짐
  pinMode(SSR_PIN2, OUTPUT);
  digitalWrite(SSR_PIN2, LOW); // 초기 상태는 꺼짐
}

void loop() {
  // ssrPin의 상태를 읽어옵니다
  int ssrState1 = digitalRead(SSR_PIN1);
  int ssrState2 = digitalRead(SSR_PIN2);

  // 첫 번째 센서에서 온도 및 습도 읽기
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  
  // 두 번째 센서에서 온도 및 습도 읽기
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  
  // 세 번째 센서에서 온도 및 습도 읽기
  float h3 = dht3.readHumidity();
  float t3 = dht3.readTemperature();
  
  // 첫 번째 및 두 번째 센서의 값 출력
  Serial.print("Sensor 1 - Humidity: ");
  Serial.print(h1);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t1);
  Serial.println(" *C");

  Serial.print("Sensor 2 - Humidity: ");
  Serial.print(h2);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t2);
  Serial.println(" *C");
  
  // 세 번째 센서의 온도 및 습도 출력
  Serial.print("Sensor 3 - Humidity: ");
  Serial.print(h3);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t3);
  Serial.println(" *C");

  // 첫 번째와 두 번째 센서의 평균값 계산
  float avgHumidity = (h1 + h2 + h3) / 3.0;
  float avgTemperature = (t1 + t2 + t3) / 3.0;
  
  // 첫 번째 및 두 번째 센서의 평균값 출력
  Serial.print("Average Humidity: ");
  Serial.print(avgHumidity);
  Serial.print(" %\t");
  Serial.print("Average Temperature: ");
  Serial.print(avgTemperature);
  Serial.println(" *C");


//-------------------------------------------------------------------------------------------------

  // 센서의 평균값에 SSR_1 제어
//     육묘장 내 평균온습도 제어 
  if (avgHumidity <= 60 || avgTemperature >= 26.5) { // 온습도센서의 평균값을 통해 릴레이 켜기
    digitalWrite(SSR_PIN1, HIGH); // SSR1 켜기   

//           육묘장 내 평균습도  
  } else if (avgHumidity >= 70) { // 온습도센서의 평균값을 통해 릴레이 끄기
    digitalWrite(SSR_PIN1, LOW); // SSR1 끄기
  }
  

  // 센서의 평균값에 SSR2 제어
// 육묘장 내 평균온도  
  if (avgTemperature >= 26.5) { // 온습도센서의 평균값을 통해 릴레이 켜기 
    digitalWrite(SSR_PIN2, HIGH); // SSR2 켜기
//       육묘장 내 평균온습도  
  } else if (avgTemperature <= 24.5 || avgHumidity >= 70) { // 온습도센서의 평균값을 통해 릴레이 끄기 
    digitalWrite(SSR_PIN2, LOW); // SSR2 끄기
  }
  
//----------------------------------------------------------------------------------------------------------------------------------------------------------


  // SSR이 켜졌는지 확인합니다
  if (ssrState1 == HIGH) {
    Serial.println("SSR1 is ON");
    lcd.init();
    lcd.setCursor(0, 0);
    lcd.print("SSR1 is ON");
    delay(1000);
  } else {
    Serial.println("SSR1 is OFF");
  }

  if (ssrState2 == HIGH) {
    Serial.println("SSR2 is ON");
    lcd.init();
    lcd.setCursor(0, 1);    
    lcd.print("SSR2 is ON");
    delay(1000);    
  } else {
    Serial.println("SSR2 is OFF");
  }
  
  // 첫 번째 및 두 번째 센서의 값 출력
  lcd.setCursor(0, 0);
  lcd.print("1H:");
  lcd.print(h1);
  lcd.print("|1T:");
  lcd.print(t1);
  lcd.setCursor(0, 1);
  lcd.print("2H:");
  lcd.print(h2);
  lcd.print("|2T:");
  lcd.print(t2);  
  delay(2000);
  // 세 번째 센서의 온도 및 습도 출력
  lcd.setCursor(0, 0);
  lcd.print("3H:");
  lcd.print(h3);
  lcd.print("|3T:");
  lcd.print(t3);
  // 첫 번째 및 두 번째 센서의 평균값 출력
  lcd.setCursor(0, 1);
  lcd.print("AH:");
  lcd.print(avgHumidity);
  lcd.print("|AT:");
  lcd.print(avgTemperature);
  lcd.setCursor(0, 1);


  // 2초 대기
  delay(2000);
}
