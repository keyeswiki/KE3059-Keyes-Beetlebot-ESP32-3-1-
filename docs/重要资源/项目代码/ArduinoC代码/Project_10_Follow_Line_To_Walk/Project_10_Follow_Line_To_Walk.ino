//*************************************************************************************
/*
Project 10: Line Tracking
*/
#include <ESP32Servo.h>
//电机
#define left_ctrl  33  //定义左电机的方向控制引脚为gpio33
#define left_pwm  26   //定义左电机的PWM控制引脚为gpio26.
#define right_ctrl  32 //定义右电机的方向控制引脚为gpio32.
#define right_pwm  25  //定义右电机的PWM控制引脚为gpio25

//IR 巡线
#define tracking_left  17   //将左巡线传感器的引脚设置为gpio17
#define tracking_right  16  //将右巡线传感器的引脚设置为gpio16
int L_val,R_val;//定义变量

//舵机
const int servoPin = 4;//将舵机引脚设为gpio4.
Servo myservo;  //创建舵机对象来控制舵机

void setup() {
  pinMode(left_ctrl,OUTPUT); //将左电机控制引脚设为 OUTPUT
  ledcAttach(left_pwm, 1200, 8); //设置left_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  pinMode(right_ctrl,OUTPUT);//将右电机控制引脚设为 OUTPUT..
  ledcAttach(right_pwm, 1200, 8); //设置right_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  
  pinMode(tracking_left, INPUT); //设置左传感器的右引脚为输入
  pinMode(tracking_right, INPUT); //设置右传感器的右引脚为输入
 
  myservo.setPeriodHertz(50);           // 标准50赫兹舵机
  myservo.attach(servoPin, 500, 2500);  // 将servoPin上的舵机附加到舵机对象上。
  myservo.write(90);  // 舵机的初始角度设置为90°
  delay(300);
}

void loop() 
{
  tracking(); 
}

void tracking()
{
  L_val = digitalRead(tracking_left);//读取左巡线传感器的值
  R_val = digitalRead(tracking_right);//读取右巡线传感器的值
  if((L_val == 1)&&(R_val == 1))//如果两个传感器都检测到黑线
  {
    front();//前进
  }
  else if((L_val == 1)&&(R_val == 0))//如果只有左侧传感器检测到黑线
  {
    left();//左转
  }
  else if((L_val == 0)&&(R_val == 1))//如果只有右侧传感器检测到黑线
  {
    right();//右转
  }
  else//如果没有传感器检测到黑线
  {
    Stop();//停
   }
}

void front()//定义状态前进
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚为 LOW.
  ledcWrite(left_pwm, 130); //左电机输出 PWM 130
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 130); //右电机输出 PWM 130
}
void left()//定义状态左转
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚为 HIGH..
  ledcWrite(left_pwm, 135); //左电机输出 PWM 135
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 80); //右电机输出 PWM 80
}
void right()//定义状态右转
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚为 LOW.
  ledcWrite(left_pwm, 80); //左电机输出 PWM 80
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚为 HIGH..
  ledcWrite(right_pwm, 135); //右电机输出 PWM 135
}
void Stop()//定义状态停止
{
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚为 LOW.
  ledcWrite(left_pwm, 0); //左电机输出 PWM 0
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 0); //右电机输出 PWM 0
}
//*************************************************************************************
