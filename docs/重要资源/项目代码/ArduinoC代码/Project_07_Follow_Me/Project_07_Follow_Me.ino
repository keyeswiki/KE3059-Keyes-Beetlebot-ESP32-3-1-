//*************************************************************************************
/*
Project 07: follow me
小车跟随物体
*/ 
#include <ESP32Servo.h>
//电机
#define left_ctrl  33  //定义左电机的方向控制引脚为gpio33
#define left_pwm  26   //定义左电机的PWM控制引脚为gpio26.
#define right_ctrl  32 //定义右电机的方向控制引脚为gpio32.
#define right_pwm  25  //定义右电机的PWM控制引脚为gpio25

//超声波传感器
#define TRIG_PIN 5 // 将超声波传感器的信号输入设置为gpio5.
#define ECHO_PIN 18 //将超声波传感器的信号输出设置为gpio18.
long distance; //定义距离变量

//舵机
const int servoPin = 4;//将舵机引脚设为gpio4.
Servo myservo;  //创建舵机对象来控制舵机

void setup() {
  pinMode(left_ctrl,OUTPUT); //将左电机控制引脚设为 OUTPUT
  ledcAttach(left_pwm, 1200, 8); //设置left_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  pinMode(right_ctrl,OUTPUT);//将右电机控制引脚设为 OUTPUT..
  ledcAttach(right_pwm, 1200, 8); //设置right_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  
  pinMode(TRIG_PIN,OUTPUT);//设置TRIG_PIN为OUTPUT.
  pinMode(ECHO_PIN,INPUT);//设置ECHO_PIN为INPUT.
  
  myservo.setPeriodHertz(50);           // 标准50赫兹舵机
  myservo.attach(servoPin, 500, 2500);  // 将舵机附加到舵机对象servoPin上。
  myservo.write(90);  // 舵机器的初始角度为90°。
  delay(300);
}

void loop() {
  distance = checkdistance();//得到超声波距离的值
  Serial.print(distance);//发送脉冲，以厘米为单位计算距离并打印结果。
  Serial.println("cm");
  if(distance<8)//distance<8
  {
    back();//后退
  }
  else if((distance>=8)&&(distance<13))//8≤distance<13
  {
    Stop();//停止
  }
  else if((distance>=13)&&(distance<35))//13≤distance<35.
  {
    front();//跟随
  }
  else//
  {
    Stop();//停止
  }
}

float checkdistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float distance = pulseIn(ECHO_PIN, HIGH) / 58.00;
  delay(10);
  return distance;
}
   
void front()//定义状态前进
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚为LOW.
  ledcWrite(left_pwm, 150); //左电机输出PWM 150
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚为LOW.
  ledcWrite(right_pwm, 150); //右电机输出PWM 150
}
void back()//定义状态后退
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚为HIGH..
  ledcWrite(left_pwm, 100); //左电机输出PWM 100
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚为HIGH..
  ledcWrite(right_pwm, 100); //右电机输出PWM 100
}
void Stop()//停止
{
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚为LOW.
  ledcWrite(left_pwm, 0); //左电机输出PWM 0.
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚为LOW.
  ledcWrite(right_pwm, 0); //右电机输出PWM 0
}
//*************************************************************************************
