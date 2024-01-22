//*************************************************************************************
/*
项目 08: 躲避障碍物
*/  
#include "HT16K33_Lib_For_ESP32.h"  //定义8*8点阵模块函数库.

//点阵
//定义点阵的引脚在GPIO21,GPIO22
#define SDA 21
#define SCL 22
ESP32_HT16K33 matrix = ESP32_HT16K33();
//数组，用于储存图案的数据，可以自己算也可以从取摸工具中得到
byte front[8]={0x12,0x24,0x48,0x90,0x90,0x48,0x24,0x12};
byte back[8]={0x48,0x24,0x12,0x09,0x09,0x12,0x24,0x48};
byte left[8]={0x18,0x24,0x42,0x99,0x24,0x42,0x81,0x00};
byte right[8]={0x00,0x81,0x42,0x24,0x99,0x42,0x24,0x18};
byte stop1[8]={0x00,0x00,0x00,0xfd,0xfd,0x00,0x00,0x00};
byte result[8][8];

//电机
#define left_ctrl  33  //定义左电机方向控制引脚gpio33.
#define left_pwm  26   //定义左电机PWM控制引脚gpio26.
#define right_ctrl  32 //定义右电机方向控制引脚gpio32.
#define right_pwm  25  //定义右电机PWM控制引脚gpio25.

//超声波传感器
#define TRIG_PIN 5 // 定义超声波的信号输入在gpio5.
#define ECHO_PIN 18 //定义超声波的信号输出在gpio18.
long distance,a1,a2;//定义三个距离变量

//舵机
int channel_PWM = 3; // 舵机通道
// 舵机频率，那么周期也就是1/50，也就是20ms ，PWM一共有16个通道，0-7位高速通道由80Mhz时钟驱动，后面8个为低速通道由1Mhz时钟驱动.
int freq_PWM = 50;
// PWM分辨率，取值为 0-20 之间  ，这里填写为10，那么后面的ledcWrite 这个里面填写的pwm值就在 0 - 2的10次方 之间 也就是 0-1024 ，如果是要求不高的东西你可以直接拿1000去算了.
int resolution_PWM = 10;
// 绑定的IO，在下面的绑定函数里面会用到，绑定之后这个IO就会变成我们PWM的输出口.
const int servopin = 4;//定义舵机的IO脚位在gpio4.

void setup() {
  Serial.begin(9600);
  pinMode(left_ctrl,OUTPUT);//设置左电机方向控制引脚为输出.
  ledcSetup(0, 1200, 8);//设置LEDC通道0频率为1200，PWM分辨率为8，即占空比256.
  ledcAttachPin(26, 0);  //将LEDC通道0绑定到指定的左电机引脚gpio26上以实现输出.
  pinMode(right_ctrl,OUTPUT);//设置右电机方向控制引脚为输出.
  ledcSetup(1, 1200, 8);//设置LEDC通道为1频率为1200，PWM分辨率为8，即占空比256.
  ledcAttachPin(25, 1);  //将LEDC通道1绑定到指定的右电机引脚gpio25上以实现输出.
  
  pinMode(TRIG_PIN,OUTPUT);//设置TRIG_PIN为输出.
  pinMode(ECHO_PIN,INPUT);//设置ECHO_PIN为输入.
  
  ledcSetup(3, 50, 10); // 设置舵机通道3频率为50,PWM分辨率为10.
  ledcAttachPin(4, 3);  //将 LEDC 通道绑定到指定 IO 口上以实现输出.
  ledcWrite(channel_PWM, 77);  //20ms高电平为1.5ms左右 ，也就是1.5/20*1024，此时舵机转到90°.
  delay(300);
  
  matrix.init(0x70, SDA, SCL);//初始化点阵
  matrix.clear(); //点阵清屏
}
 
void loop()
 {
  avoid();//运行避障主程序
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
  distance = checkdistance(); //获取超声波距离的值
  Serial.println(distance);
  if((distance < 10)&&(distance != 0))//如果距离小于10且大于0
  {
    car_Stop();//停止
    matrix.clear();
    matrix.showLedMatrix(stop1,0,0);//点阵显示停止图案
    matrix.show();//点阵显示停止图案
    delay(200);
    ledcWrite(channel_PWM, 128);  //20ms高电平为2.5ms左右 ，也就是2.5/20*1024，此时舵机转到180°.
    delay(300);
    a1=checkdistance();//测量距离
    delay(200);
    ledcWrite(channel_PWM, 25);  //20ms高电平为0.5ms左右 ，也就是0.5/20*1024，此时舵机转到0°.
    delay(300);
    a2=checkdistance();//测量距离
    delay(200);
    if(a1 > a2)//比较距离，如果左边大于右边
    {
      car_left();//向左转
      matrix.clear();
      matrix.showLedMatrix(left,0,0);//点阵显示向左图案
      matrix.show();//点阵显示向左图案
      ledcWrite(channel_PWM, 77);  //20ms高电平为1.5ms左右 ，也就是1.5/20*1024，此时舵机转到90°.
      delay(300);
      matrix.clear();
      matrix.showLedMatrix(front,0,0);//点阵显示向前图案
      matrix.show();//点阵显示向前图案
    }
    else//如果右边大于左边
    {
      car_right();//向右转
      matrix.clear();
      matrix.showLedMatrix(right,0,0);//点阵显示向右图案
      matrix.show();//点阵显示向右图案
      ledcWrite(channel_PWM, 77);  //20ms高电平为1.5ms左右 ，也就是1.5/20*1024，此时舵机转到90°.
      delay(300);
      matrix.clear();
      matrix.showLedMatrix(front,0,0);//点阵显示前进图案
      matrix.show();//点阵显示前进图案
    }
  }
  else//如果以上都不是
  {
    car_front();//前进
    matrix.clear();
    matrix.showLedMatrix(front,0,0);//点阵显示前进图案
    matrix.show();//点阵显示前进图案
  }
}

void car_front()//定义前进的状态
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 200); //LEDC通道0绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚低电平.
  ledcWrite(1, 200); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void car_back()//定义后退的状态
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚高电平.
  ledcWrite(0, 50); //LEDC通道0绑定到指定的左电机输出PWM值为50.
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚高电平.
  ledcWrite(1, 50); //LEDC通道1绑定到指定的右电机输出PWM值为50.
}
void car_left()//定义左转的状态
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚高电平.
  ledcWrite(0, 200); //LEDC通道0绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚低电平.
  ledcWrite(1, 200); //LEDC通道1绑定到指定的右电机输出PWM值为200.
}
void car_right()//定义右转的状态
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 200); //LEDC通道0绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚高电平.
  ledcWrite(1, 200); //LEDC通道1绑定到指定的右电机输出PWM值为200.
}
void car_Stop()//定义停止的状态
{
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚低电平.
  ledcWrite(0, 0); //LEDC通道0绑定到指定的左电机输出PWM值为0.
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚低电平.
  ledcWrite(1, 0); //LEDC通道1绑定到指定的右电机输出PWM值为0.
}
//*************************************************************************************
