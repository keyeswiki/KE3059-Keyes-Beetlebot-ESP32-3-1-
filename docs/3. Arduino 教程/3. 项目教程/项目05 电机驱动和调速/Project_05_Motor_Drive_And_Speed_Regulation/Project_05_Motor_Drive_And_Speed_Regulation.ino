//*************************************************************************************
/*
 项目 05 电机驱动和调速
 电机向前、向后、向左、向右转动
*/ 
#define left_ctrl  33  //定义左电机方向控制引脚gpio33
#define left_pwm  26   //定义左电机PWM控制引脚gpio26
#define right_ctrl  32 //定义右电机方向控制引脚gpio32
#define right_pwm  25  //定义右电机PWM控制引脚gpio25

void setup()
{
  pinMode(left_ctrl,OUTPUT);//设置左电机方向控制引脚为输出.
  ledcSetup(0, 1200, 8);//设置LEDC通道1频率为1200，PWM分辨率为8，即占空比256.
  ledcAttachPin(26, 0);  //将LEDC通道1绑定到指定的左电机引脚gpio26上以实现输出.
  pinMode(right_ctrl,OUTPUT);//设置右电机方向控制引脚为输出.
  ledcSetup(1, 1200, 8);//设置LEDC通道为2频率为1200，PWM分辨率为8，即占空比256.
  ledcAttachPin(25, 1);  //将LEDC通道2绑定到指定的右电机引脚gpio25上以实现输出.
}

void loop()
{ 
  //前进
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 200); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,LOW);  //右电机方向控制引脚低电平.
  ledcWrite(1, 200);  //LEDC通道2绑定到指定的右电机输出PWM值为200.
  delay(2000);//延时2秒
  
  //后退
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚高电平.
  ledcWrite(0, 50); //LEDC通道1绑定到指定的左电机输出PWM值为50.
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚高电平.
  ledcWrite(1, 50); //LEDC通道2绑定到指定的右电机输出PWM值为50.
  delay(2000);//延时2秒.
  
  //左转
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚高电平.
  ledcWrite(0, 55); //LEDC通道1绑定到指定的左电机输出PWM值为55.
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚低电平.
  ledcWrite(1, 200); //LEDC通道2绑定到指定的右电机输出PWM值为200.
  delay(2000);//延时2秒.
  
  //右转
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 200); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚高电平.
  ledcWrite(1, 55); //LEDC通道2绑定到指定的右电机输出PWM值为55.
  delay(2000);//延时2秒.
  
  //停止
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚低电平.
  ledcWrite(0, 0); //LEDC通道1绑定到指定的左电机输出PWM值为0.
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚低电平.
  ledcWrite(1, 0); //LEDC通道2绑定到指定的右电机输出PWM值为0.
  delay(2000);//延时2秒.
}
//*************************************************************************************
