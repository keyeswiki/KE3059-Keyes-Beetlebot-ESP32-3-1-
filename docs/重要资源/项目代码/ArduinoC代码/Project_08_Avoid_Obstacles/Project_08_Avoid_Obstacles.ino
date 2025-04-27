//*************************************************************************************
/*
Project 08: avoid obstacles
避障
*/  
#include <ESP32Servo.h>
//motor
#define left_ctrl  33  //定义左电机的方向控制引脚为gpio33
#define left_pwm  26   //定义左电机的PWM控制引脚为gpio26.
#define right_ctrl  32 //定义右电机的方向控制引脚为gpio32.
#define right_pwm  25  //定义右电机的PWM控制引脚为gpio25

//超声波传感器
#define TRIG_PIN 5 // 将超声波传感器的信号输入设置为gpio5.
#define ECHO_PIN 18 //将超声波传感器的信号输出设置为gpio18.
long distance,a1,a2;//定义距离变量

//舵机
const int servoPin = 4;//将舵机引脚设为gpio4.
Servo myservo;  //创建舵机对象来控制舵机

void setup() {
  pinMode(left_ctrl,OUTPUT); //将左电机控制引脚设为 OUTPUT
  ledcAttach(left_pwm, 1200, 8); //设置left_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  pinMode(right_ctrl,OUTPUT);//将右电机控制引脚设为 OUTPUT..
  ledcAttach(right_pwm, 1200, 8); //设置right_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  
  pinMode(TRIG_PIN,OUTPUT); //设置TRIG_PIN为OUTPUT.
  pinMode(ECHO_PIN,INPUT);//设置ECHO_PIN为INPUT.
  
  myservo.setPeriodHertz(50);           // 标准50赫兹舵机
  myservo.attach(servoPin, 500, 2500);  // 将舵机附加到舵机对象servoPin上。
  myservo.write(90);
  delay(500);
}
 
void loop()
 {
  avoid();//避障
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

void avoid()
{
  distance = checkdistance(); //得到超声波距离的值
  Serial.println(distance);
  if((distance < 10)&&(distance != 0))//0<distance<10
  {
    car_Stop();//停下
    delay(100);
    myservo.write(170);  //舵机转动到180°. 
    delay(500);
    a1=checkdistance();//测距
    delay(100);
    myservo.write(10); //舵机转动到0°. 
    delay(500);
    a2=checkdistance();//测距
    delay(100);

    if(a1 > a2)//
    {
      car_left();//左转
      delay(100);
      myservo.write(90); //舵机初始角度为90° .
      delay(500);
    }
    else//如果左距离<右距离
    {
      car_right();//右转
      delay(100);
      myservo.write(90); //舵机初始角度为90° .
      delay(500);
    }
  }
  else//
  {
    car_front();//前进  
  }
}

void car_front()//定义状态前进
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚为LOW.
  ledcWrite(left_pwm, 100); //左电机输出 PWM 100
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 100); //右电机输出 PWM 100
}
void car_back()//定义状态后退
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚为HIGH..
  ledcWrite(left_pwm, 100); //左电机输出 PWM 100
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚为 HIGH..
  ledcWrite(right_pwm, 100); //右电机输出 PWM 100
}
void car_left()//定义状态左转
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚为HIGH..
  ledcWrite(left_pwm, 100); //左电机输出 PWM 100
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 100); //右电机输出 PWM 100
}
void car_right()//定义状态右转
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚为LOW.
  ledcWrite(left_pwm, 100); //左电机输出 PWM 100
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚为 HIGH..
  ledcWrite(right_pwm, 100); //右电机输出 PWM 100
}
void car_Stop()//定义状态停止
{
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚为LOW.
  ledcWrite(left_pwm, 0); //左电机输出 PWM 0 
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚为 LOW.
  ledcWrite(right_pwm, 0); //右电机输出 PWM 0
}                                                                
//*************************************************************************************
