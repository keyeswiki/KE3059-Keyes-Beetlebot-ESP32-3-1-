//*************************************************************************************
#include <Arduino.h>
#include <ESP32Servo.h>

Servo myservo;  // 创建舵机对象来控制舵机

int servoPin = 23; // 舵机引脚

void setup() {
  myservo.setPeriodHertz(50);           // 标准50赫兹舵机
  myservo.attach(servoPin, 500, 2500);  // 将servoPin上的舵机附加到舵机对象上

  myservo.write(180);  // 舵机角度为 180°.
  delay(1000);
  myservo.write(90);  // 舵机角度为 90°.
  delay(1000);
  myservo.write(180);  // 舵机角度为 180°.
  delay(1000);
}
void loop() {

} 
//*************************************************************************************
