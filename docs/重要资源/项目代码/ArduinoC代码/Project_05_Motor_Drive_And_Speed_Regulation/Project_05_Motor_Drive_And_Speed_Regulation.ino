//*************************************************************************************
/*
 Project 05 Motor drive and speed regulation
 电机向前、向后、向左、向右移动
*/ 
#define left_ctrl  33  //定义左电机方向控制引脚gpio33
#define left_pwm  26   //定义左电机的PWM控制引脚gpio26
#define right_ctrl  32 //定义右电机方向控制引脚gpio32
#define right_pwm  25  //定义右电机的PWM控制引脚为gpio25

void setup()
{
  pinMode(left_ctrl,OUTPUT);     //左电机方向控制引脚 OUTPUT
  ledcAttach(left_pwm, 1200, 8); //left_pwm引脚频率为1200, PWM分辨率为8，即占空比为256。
  pinMode(right_ctrl,OUTPUT);//右电机方向控制引脚 OUTPUT.
  ledcAttach(right_pwm, 1200, 8); //right_pwm引脚频率为1200, PWM分辨率为8，即占空比为256。
}

void loop()
{ 
  //前进
  digitalWrite(left_ctrl,LOW); // 左电机方向控制引脚 LOW.
  ledcWrite(left_pwm, 200); // 左电机输出 PWM 200
  digitalWrite(right_ctrl,LOW); // 右电机方向控制引脚 LOW.
  ledcWrite(right_pwm, 200); // 右电机输出 PWM 200
  delay(2000);//延时 2s
  
  //后退
  digitalWrite(left_ctrl,HIGH); // 左电机方向控制引脚 HIGH.
  ledcWrite(left_pwm, 55); // 左电机输出 PWM 55
  digitalWrite(right_ctrl,HIGH); // 右电机方向控制引脚 HIGH.
  ledcWrite(right_pwm, 55); // 右电机输出 PWM 55
  delay(2000);//延时 2s
  
  //左转
  digitalWrite(left_ctrl,HIGH); // 左电机方向控制引脚 HIGH..
  ledcWrite(left_pwm, 55); // 左电机输出 PWM 55.
  digitalWrite(right_ctrl,LOW); // 右电机方向控制引脚 LOW.
  ledcWrite(right_pwm, 200); // 右电机输出 PWM 200.
  delay(2000);//延时 2s
  
  //右转
  digitalWrite(left_ctrl,LOW); // 左电机方向控制引脚 LOW.
  ledcWrite(left_pwm, 200); // 左电机输出 PWM 200.
  digitalWrite(right_ctrl,HIGH); // 右电机方向控制引脚 HIGH..
  ledcWrite(right_pwm, 55); // 右电机输出 PWM 55
  delay(2000);//延时 2s
  
  //停止
  digitalWrite(left_ctrl,LOW);// 左电机方向控制引脚 LOW.
  ledcWrite(left_pwm, 0); // 左电机输出 PWM 0.
  digitalWrite(right_ctrl,LOW);// 右电机方向控制引脚 LOW.
  ledcWrite(right_pwm, 0); // 右电机输出 PWM 0
  delay(2000);//延时 2s
}
//*************************************************************************************
