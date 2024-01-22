//*************************************************************************************
/*
项目 04.2 舵机转动
舵机将从0度旋转到180度，然后反转使它从180度旋转到0度的方向不断地重复这些动作.
*/
#include <ESP32Servo.h>

Servo myservo;  // 创建舵机对象来控制舵机

int posVal = 0;    // 创建变量来存储舵机位置
int servoPin = 4; // 舵机引脚

void setup() {
  myservo.setPeriodHertz(50);           // 标准50赫兹舵机
  myservo.attach(servoPin, 500, 2500);  // 将servoPin上的舵机附加到舵机对象上
}
void loop() {

  for (posVal = 0; posVal <= 180; posVal += 1) { // 从0°到180°
    // 以1度为阶梯
    myservo.write(posVal);       // 告诉舵机进入可变'pos'位置
    delay(15);                   // 等待15ms舵机到达位置
  }
  for (posVal = 180; posVal >= 0; posVal -= 1) { // 从180°到0°
    myservo.write(posVal);       // 告诉舵机进入可变'pos'位置
    delay(15);                   // 等待15ms舵机到达位置
  }
}
//*************************************************************************************
