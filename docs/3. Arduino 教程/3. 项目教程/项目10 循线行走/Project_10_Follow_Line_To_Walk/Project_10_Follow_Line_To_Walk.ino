//*************************************************************************************
/*
项目 10: 循迹小车
*/

//电机
#define left_ctrl  33  //定义左电机方向控制引脚gpio33
#define left_pwm  26   //定义左电机PWM控制引脚gpio26
#define right_ctrl  32 //定义右电机方向控制引脚gpio32
#define right_pwm  25  //定义右电机PWM控制引脚gpio25

//红外循迹
#define tracking_left  17  //定义左红外循迹引脚gpio17
#define tracking_right  16  //定义右红外循迹引脚gpio16
int L_val,R_val;//定义左,右红外循迹两个变量

//舵机
int channel_PWM = 3; // 舵机通道
// 舵机频率，那么周期也就是1/50，也就是20ms ，PWM一共有16个通道，0-7位高速通道由80Mhz时钟驱动，后面8个为低速通道由1Mhz时钟驱动.
int freq_PWM = 50;
// PWM分辨率，取值为 0-20 之间  ，这里填写为10，那么后面的ledcWrite 这个里面填写的pwm值就在 0 - 2的10次方 之间 也就是 0-1024 ，如果是要求不高的东西你可以直接拿1000去算了.
int resolution_PWM = 10;
// 绑定的IO，在下面的绑定函数里面会用到，绑定之后这个IO就会变成我们PWM的输出口.
const int servopin = 4;//定义舵机的IO脚位在gpio4.

void setup() {
  pinMode(left_ctrl,OUTPUT);//设置左电机方向控制引脚为输出.
  ledcSetup(0, 1200, 8);//设置LEDC通道1频率为20，PWM分辨率为8，即占空比256.
  ledcAttachPin(26, 0);  //将LEDC通道1绑定到指定的左电机引脚gpio26上以实现输出.
  pinMode(right_ctrl,OUTPUT);//设置右电机方向控制引脚为输出.
  ledcSetup(1, 1200, 8);//设置LEDC通道为2频率为20，PWM分辨率为8，即占空比256.
  ledcAttachPin(25, 1);  //将LEDC通道2绑定到指定的右电机引脚gpio25上以实现输出.
  
  pinMode(tracking_left, INPUT); //设置左红外循迹引脚为输入
  pinMode(tracking_right, INPUT); //设置右红外循迹引脚为输入
 
  ledcSetup(3, 50, 10); // 设置舵机通道3频率为50,PWM分辨率为10.
  ledcAttachPin(4, 3);  //将 LEDC 通道绑定到指定 IO 口上以实现输出.
  ledcWrite(channel_PWM, 77);  //20ms高电平为1.5ms左右 ，也就是1.5/20*1024，此时舵机转动90°.
  delay(300);
}

void loop() 
{
  tracking(); //运行主程序
}

void tracking()
{
  L_val = digitalRead(tracking_left);//读取左红外循迹的值
  R_val = digitalRead(tracking_right);//读取右红外循迹的值
  if((L_val == 1)&&(R_val == 1))//如果左,右红外循迹都读到黑线
  {
    front();//小车前进
  }
  else if((L_val == 1)&&(R_val == 0))//否则如果左红外循迹读到黑线，右红外循迹没有
  {
    left();//小车左转
  }
  else if((L_val == 0)&&(R_val == 1))//如果右红外循迹读到黑线，左红外循迹没有
  {
    right();//小车右转
  }
  else//如果左,右红外循迹都没有读到黑线
  {
    Stop();//小车停止
   }
}

void front()//定义前进的状态
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 130); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚低电平.
  ledcWrite(1, 130); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void left()//定义左转的状态
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚高电平.
  ledcWrite(0, 135); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚低电平.
  ledcWrite(1, 80); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void right()//定义右转的状态
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 80); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚高电平.
  ledcWrite(1, 135); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void Stop()//定义停止的状态
{
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚低电平.
  ledcWrite(0, 0); //LEDC通道1绑定到指定的左电机输出PWM值为0.
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚低电平.
  ledcWrite(1, 0); //LEDC通道2绑定到指定的右电机输出PWM值为0.
}
//*************************************************************************************
