//*************************************************************************************
/*
Project 12:Light Following Car
*/ 
#include <ESP32Servo.h>
//电机
#define left_ctrl  33  //定义左电机的方向控制引脚为gpio33
#define left_pwm  26   //定义左电机的PWM控制引脚为gpio26.
#define right_ctrl  32 //定义右电机的方向控制引脚为gpio32.
#define right_pwm  25  //定义右电机的PWM控制引脚为gpio25

//光敏电阻
#define light_L_Pin  34   //定义左边光敏电阻的引脚为gpio34
#define light_R_Pin  35   //定义右边光敏电阻的引脚为gpio35
int left_light; 
int right_light;

//舵机
const int servoPin = 4;//将舵机引脚设为gpio4.
Servo myservo;  //创建舵机对象来控制舵机

void setup(){
  Serial.begin(115200); //波特率 115200.
  pinMode(light_L_Pin, INPUT); //将左侧传感器的引脚设置为INPUT
  pinMode(light_R_Pin, INPUT); //将右侧传感器的引脚设置为INPUT
  
  pinMode(left_ctrl,OUTPUT); //将左电机控制引脚设为 OUTPUT
  ledcAttach(left_pwm, 1200, 8); //设置left_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  pinMode(right_ctrl,OUTPUT);//将右电机控制引脚设为 OUTPUT..
  ledcAttach(right_pwm, 1200, 8); //设置right_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  
  myservo.setPeriodHertz(50);           // 标准50赫兹舵机
  myservo.attach(servoPin, 500, 2500);  // 将servoPin上的舵机附加到舵机对象上。
  myservo.write(90);  // 舵机的初始角度设置为90°
  delay(300);
}

void loop(){
  left_light = analogRead(light_L_Pin);//读取左边光敏电阻的值
  right_light = analogRead(light_R_Pin);//读取右边光敏电阻的值
  Serial.print("left_light_value = ");
  Serial.println(left_light);
  Serial.print("right_light_value = ");
  Serial.println(right_light);
  if (left_light > 3000 && right_light > 3000) //检测左、右光敏电阻范围
  {  
    Car_front(); //前进
  } 
  else if (left_light > 3000 && right_light <= 3000)  //检测左、右光敏电阻范围
  {
    Car_left(); //左转
  } 
  else if (left_light <= 3000 && right_light > 3000) //检测左、右光敏电阻范围
  {
    Car_right(); //右转
  } 
  else  //
  {
    Car_Stop(); //停下
  }
}

void Car_front()
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚为 LOW.
  ledcWrite(left_pwm, 150); //左电机输出 PWM 150
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 150); //右电机输出 PWM 150
}
void Car_left()
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚为 HIGH..
  ledcWrite(left_pwm, 150); //左电机输出 PWM 150
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 150); //右电机输出 PWM 150
}
void Car_right()
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚为 LOW.
  ledcWrite(left_pwm, 150); //左电机输出 PWM 150
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚为 HIGH..
  ledcWrite(right_pwm, 150); //右电机输出 PWM 150
}
void Car_Stop()
{
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚为 LOW.
  ledcWrite(left_pwm, 0); //左电机输出 PWM 0
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 0); //右电机输出 PWM 0
}
//*************************************************************************************
