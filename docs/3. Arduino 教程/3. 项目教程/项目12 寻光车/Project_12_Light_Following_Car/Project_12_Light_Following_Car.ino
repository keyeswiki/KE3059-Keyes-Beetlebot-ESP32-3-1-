//*************************************************************************************
/*
项目 12:寻光车
*/ 

//电机
#define left_ctrl  33  //定义左电机方向控制引脚gpio33.
#define left_pwm  26   //定义左电机PWM控制引脚gpio26.
#define right_ctrl  32 //定义右电机方向控制引脚gpio32.
#define right_pwm  25  //定义右电机PWM控制引脚gpio25.

//左右光敏传感器
#define light_L_Pin  34   //定义左光敏传感器引脚gpio34
#define light_R_Pin  35   //定义右光敏传感器引脚gpio35
int left_light; 
int right_light;

//舵机
int channel_PWM = 3; // 舵机通道
// 舵机频率，那么周期也就是1/50，也就是20ms ，PWM一共有16个通道，0-7位高速通道由80Mhz时钟驱动，后面8个为低速通道由1Mhz时钟驱动.
int freq_PWM = 50;
// PWM分辨率，取值为 0-20 之间  ，这里填写为10，那么后面的ledcWrite 这个里面填写的pwm值就在 0 - 2的10次方 之间 也就是 0-1024 ，如果是要求不高的东西你可以直接拿1000去算了.
int resolution_PWM = 10;
// 绑定的IO，在下面的绑定函数里面会用到，绑定之后这个IO就会变成我们PWM的输出口.
const int servopin = 4;//定义舵机的IO脚位在gpio4.

void setup(){
  Serial.begin(115200); //设置波特率为115200.
  pinMode(light_L_Pin, INPUT); //设置左光敏传感器引脚为输入
  pinMode(light_R_Pin, INPUT); //设置右光敏传感器引脚为输入
  
  pinMode(left_ctrl,OUTPUT);//设置左电机方向控制引脚为输出.
  ledcSetup(0, 1200, 8);//设置LEDC通道1频率为1200，PWM分辨率为8，即占空比256.
  ledcAttachPin(26, 0);  //将LEDC通道1绑定到指定的左电机引脚gpio26上以实现输出.
  pinMode(right_ctrl,OUTPUT);//设置右电机方向控制引脚为输出.
  ledcSetup(1, 1200, 8);//设置LEDC通道2频率为1200，PWM分辨率为8，即占空比256.
  ledcAttachPin(25, 1);  //将LEDC通道2绑定到指定的右电机引脚gpio25上以实现输出.
  
  ledcSetup(3, 50, 10); // 设置舵机通道3频率为50,PWM分辨率为10.
  ledcAttachPin(4, 3);  //将 LEDC 通道绑定到指定 IO 口上以实现输出.
  ledcWrite(channel_PWM, 77);  //20ms高电平为1.5ms左右 ，也就是1.5/20*1024，设置舵机初始角度为90°.
  delay(300);
}

void loop(){
  left_light = analogRead(light_L_Pin);//读取左光敏传感器的值
  right_light = analogRead(light_R_Pin);//读取右光敏传感器的值
  Serial.print("left_light_value = ");
  Serial.println(left_light);
  Serial.print("right_light_value = ");
  Serial.println(right_light);
  if (left_light > 3000 && right_light > 3000) //左,右光敏传感器测到的范围值
  {  
    Car_front(); //小车前进
  } 
  else if (left_light > 3000 && right_light <= 3000)  //左,右光敏传感器测到的范围值
  {
    Car_left(); //小车左转
  } 
  else if (left_light <= 3000 && right_light > 3000) //左,右光敏传感器测到的范围值
  {
    Car_right(); //小车右转
  } 
  else  //除以上情况之外
  {
    Car_Stop(); //小车停止
  }
}

void Car_front()
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 200); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚低电平.
  ledcWrite(1, 200); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void Car_left()
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚高电平.
  ledcWrite(0, 200); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚低电平.
  ledcWrite(1, 200); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void Car_right()
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 200); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚高电平.
  ledcWrite(1, 200); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void Car_Stop()
{
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚低电平.
  ledcWrite(0, 0); //LEDC通道1绑定到指定的左电机输出PWM值为0.
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚低电平.
  ledcWrite(1, 0); //LEDC通道2绑定到指定的右电机输出PWM值为0.
}
//*************************************************************************************
