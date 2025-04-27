#include <Arduino.h>
#include <ESP32Servo.h>

Servo myservo;  // 创建伺服对象来控制伺服

int servoPin = 4; // 舵机引脚

void setup() {
  myservo.setPeriodHertz(50);           // 标准50赫兹伺服
  myservo.attach(servoPin, 500, 2500);  // 将servoPin上的伺服附加到伺服对象上

  myservo.write(90);  // 舵机的角度是90°.
  delay(1000);
  myservo.write(0);  // 舵机的角度是0°.
  delay(1000);
  myservo.write(90);  // 舵机的角度是90°.
  delay(1000);
}
void loop() {

}
