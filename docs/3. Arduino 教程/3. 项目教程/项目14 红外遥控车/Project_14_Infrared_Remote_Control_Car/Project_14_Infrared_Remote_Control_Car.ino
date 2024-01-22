//*************************************************************************************
/*
项目 14:红外控制小车
*/ 
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <HT16K33_Lib_For_ESP32.h>

//红外接收
const uint16_t RECV_PIN = 19;  //红外接收引脚
IRrecv irrecv(RECV_PIN); //初始化红外接收引脚
unsigned long irr_val;
decode_results results; //创建一个解码结果类对象

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

//电机
#define left_ctrl  33  //定义左电机方向控制引脚gpio33.
#define left_pwm  26   //定义左电机PWM控制引脚gpio26.
#define right_ctrl  32 //定义右电机方向控制引脚gpio32.
#define right_pwm  25  //定义右电机PWM控制引脚gpio25.

//舵机
int channel_PWM = 3; // 舵机通道
// 舵机频率，那么周期也就是1/50，也就是20ms ，PWM一共有16个通道，0-7位高速通道由80Mhz时钟驱动，后面8个为低速通道由1Mhz时钟驱动.
int freq_PWM = 50;
// PWM分辨率，取值为 0-20 之间  ，这里填写为10，那么后面的ledcWrite 这个里面填写的pwm值就在 0 - 2的10次方之间 也就是 0-1024 ，如果是要求不高的东西你可以直接拿1000去算了.
int resolution_PWM = 10;
// 绑定的IO，在下面的绑定函数里面会用到，绑定之后这个IO就会变成我们PWM的输出口.
const int servopin = 4;//定义舵机的IO脚位在gpio4.

void setup() {
  Serial.begin(115200);//打开串口，波特率115200
  pinMode(left_ctrl,OUTPUT);//设置左电机方向控制引脚为输出.
  ledcSetup(0, 1200, 8);//设置LEDC通道1频率为1200，PWM分辨率为8，即占空比256.
  ledcAttachPin(26, 0);  //将LEDC通道1绑定到指定的左电机引脚gpio26上以实现输出.
  pinMode(right_ctrl,OUTPUT);//设置右电机方向控制引脚为输出.
  ledcSetup(1, 1200, 8);//设置LEDC通道为2频率为1200，PWM分辨率为8，即占空比256.
  ledcAttachPin(25, 1);  //将LEDC通道2绑定到指定的右电机引脚gpio25上以实现输出.
  
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  irrecv.enableIRIn(); // 开始接收信号
  Serial.print("Enabling IRin");
  Serial.println(RECV_PIN);   //print the infrared receiving pin
  
  ledcSetup(3, 50, 10); // 设置舵机通道3频率为50,PWM分辨率为10.
  ledcAttachPin(4, 3);  //将 LEDC 通道绑定到指定 IO 口上以实现输出.
  ledcWrite(channel_PWM, 77);  //20ms高电平为1.5ms左右 ，也就是1.5/20*1024，此时舵机转到90°.
  delay(300);
  matrix.init(0x70, SDA, SCL);//初始化点阵
  matrix.clear(); //点阵清屏
}

void loop() {
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    irr_val = results.value;
    serialPrintUint64(irr_val, HEX);
    Serial.println("");
    switch(irr_val)
    {
      case 0xFF629D : 
        car_front(); 
        matrix.clear();
        matrix.showLedMatrix(front,0,0);//点阵显示向前图案
        matrix.show();//点阵显示向前图案  
        break;
      case 0xFFA857 : 
        car_back(); 
        matrix.clear();
        matrix.showLedMatrix(back,0,0);//点阵显示向后图案
        matrix.show();//点阵显示向后图案
        break;
      case 0xFF22DD : 
        car_left(); 
        matrix.clear();
        matrix.showLedMatrix(left,0,0);//点阵显示向左图案
        matrix.show();//点阵显示向左图案
        break; 
      case 0xFFC23D : 
        car_right();
        matrix.clear();
        matrix.showLedMatrix(right,0,0);//点阵显示向右图案
        matrix.show();//点阵显示向右图案
        break;
      case 0xFF02FD : 
        car_Stop();
        matrix.clear();
        matrix.showLedMatrix(stop1,0,0);//点阵显示停止图案
        matrix.show();//点阵显示停止图案
        break;
    }
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}


void car_front()//定义前进的状态
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 200); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚低电平.
  ledcWrite(1, 200); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void car_back()//定义后退的状态
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚高电平.
  ledcWrite(0, 50); //LEDC通道1绑定到指定的左电机输出PWM值为50.
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚高电平.
  ledcWrite(1, 50); //LEDC通道2绑定到指定的右电机输出PWM值为50.
}
void car_left()//定义左转的状态
{
  digitalWrite(left_ctrl,HIGH); //左电机方向控制引脚高电平.
  ledcWrite(0, 200); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,LOW); //右电机方向控制引脚低电平.
  ledcWrite(1, 200); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void car_right()//定义右转的状态
{
  digitalWrite(left_ctrl,LOW); //左电机方向控制引脚低电平.
  ledcWrite(0, 200); //LEDC通道1绑定到指定的左电机输出PWM值为200.
  digitalWrite(right_ctrl,HIGH); //右电机方向控制引脚高电平.
  ledcWrite(1, 200); //LEDC通道2绑定到指定的右电机输出PWM值为200.
}
void car_Stop()//定义停止的状态
{
  digitalWrite(left_ctrl,LOW);//左电机方向控制引脚低电平.
  ledcWrite(0, 0); //LEDC通道1绑定到指定的左电机输出PWM值为0.
  digitalWrite(right_ctrl,LOW);//右电机方向控制引脚低电平.
  ledcWrite(1, 0); //LEDC通道2绑定到指定的右电机输出PWM值为0.
}
//*************************************************************************************
