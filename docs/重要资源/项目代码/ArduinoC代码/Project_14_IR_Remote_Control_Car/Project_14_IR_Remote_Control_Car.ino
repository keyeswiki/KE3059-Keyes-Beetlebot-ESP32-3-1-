//*************************************************************************************
/*
Project 14：IR Remote Control Car
*/ 
#include <IRremote.hpp>
#include <ESP32Servo.h>

//IR 接收
const uint16_t RECV_PIN = 19;  //红外接收引脚
IRrecv irrecv(RECV_PIN); //初始化红外接收器引脚
decode_results results; //创建一个解码的结果类对象

//电机
#define left_ctrl  33  //定义左电机的方向控制引脚为gpio33
#define left_pwm  26   //定义左电机的PWM控制引脚为gpio26.
#define right_ctrl  32 //定义右电机的方向控制引脚为gpio32.
#define right_pwm  25  //定义右电机的PWM控制引脚为gpio25

//舵机
const int servoPin = 4;//将舵机引脚设为gpio4.
Servo myservo;  //创建舵机对象来控制舵机

void setup() {
  Serial.begin(115200);//波特率 115200
  pinMode(left_ctrl,OUTPUT); //将左电机控制引脚设为 OUTPUT
  ledcAttach(left_pwm, 1200, 8); //设置left_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  pinMode(right_ctrl,OUTPUT);//将右电机控制引脚设为 OUTPUT..
  ledcAttach(right_pwm, 1200, 8); //设置right_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  
  // 如果中断驱动程序在安装时崩溃，给用户提示发生了什么。

  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); // 开始接收
  
  myservo.setPeriodHertz(50);           // 标准50赫兹舵机
  myservo.attach(servoPin, 500, 2500);  // 将servoPin上的舵机附加到舵机对象上。
  myservo.write(90);  // 舵机的初始角度设置为90°
  delay(300);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);  // 打印原始数据
    handleControl(IrReceiver.decodedIRData.decodedRawData); //处理来自远程控制的命令 
    IrReceiver.resume();  // 接收下一个值
  }
}

void handleControl(unsigned long irr_val){
  switch(irr_val)
  {
    case 0xB946FF00 : 
      car_front();  
      break;
    case 0xEA15FF00 : 
      car_back(); 
      break;
    case 0xBB44FF00 : 
      car_left(); 
      break; 
    case 0xBC43FF00 : 
      car_right();
      break;
    case 0xBF40FF00 : 
      car_Stop();
      break;
  }  
}

void car_front()//前进
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚为 LOW.
  ledcWrite(left_pwm, 150); //左电机输出 PWM 150
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 150); //右电机输出 PWM 150
}
void car_back()//后退
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚为 HIGH..
  ledcWrite(left_pwm, 150); //左电机输出 PWM 150
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚为 HIGH..
  ledcWrite(right_pwm, 150); //右电机输出 PWM 150
}
void car_left()//左转
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚为 HIGH..
  ledcWrite(left_pwm, 150); //左电机输出 PWM 150
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 150); //右电机输出 PWM 150
}
void car_right()//右转
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚为 LOW.
  ledcWrite(left_pwm, 150); //左电机输出 PWM 150
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚为 HIGH..
  ledcWrite(right_pwm, 150); //右电机输出 PWM 150
}
void car_Stop()//停止
{
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚为 LOW.
  ledcWrite(left_pwm, 0); //左电机输出 PWM 0
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 0); //右电机输出 PWM 0
}
//*************************************************************************************
