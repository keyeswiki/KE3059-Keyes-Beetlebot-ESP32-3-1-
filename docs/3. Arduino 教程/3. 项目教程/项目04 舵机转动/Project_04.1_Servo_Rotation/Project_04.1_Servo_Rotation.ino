//*************************************************************************************
/*
项目 04.1 舵机转动
舵机塑料臂将以0°、45°、90°、135°、180°的角度反复旋转
*/
#include <Arduino.h>

// 舵机通道 
int channel_PWM = 3;  
// 舵机频率，那么周期也就是1/50，也就是20ms ，PWM一共有16个通道，0-7位高速通道由80Mhz时钟驱动，后面8个为低速通道由1Mhz时钟驱动.
int freq_PWM = 50;   
// PWM分辨率，取值为 0-20 之间  ，这里填写为10，那么后面的ledcWrite这个里面填写的pwm值就在0 - 2的10次方 之间 也就是 0-1024 ，如果是要求不高的东西你可以直接拿1000去算了.
int resolution_PWM = 10;   
// 绑定的IO，在下面的绑定函数里面会用到，绑定之后这个IO就会变成我们PWM的输出口.
const int PWM_Pin = 4;  //指定pwm绑定到GPIO4输出.

void setup() {
  Serial.begin(115200); //设置波特率为115200.
  ledcSetup(channel_PWM, freq_PWM, resolution_PWM); // 设置舵机通道,舵机频率,PWM分辨率.
  ledcAttachPin(PWM_Pin, channel_PWM);  //将LEDC通道绑定到指定 IO 口上以实现输出
}

void get_pwm_info()
{
  Serial.println("*******************************************************************");
  Serial.print("Reads the value of the specified channel duty cycle：");
  Serial.println(ledcRead(channel_PWM));  //读取指定通道占空比的值
  Serial.print("Reads the value of the specified channel frequency as：");
  Serial.println(ledcReadFreq(channel_PWM));  //返回指定通道当前频率（如果当前占空比为0 则该方法返回0).
}

void loop() {
  ledcWrite(channel_PWM, 25);  //20ms高电平为0.5ms左右 ，也就是0.5/20*1024，此时舵机角度为0°.
  get_pwm_info();  //打印信息，点击IDE右上角的串口查看器就可以看到打印的内容了
  delay(1000);
  ledcWrite(channel_PWM, 52);  //20ms高电平为1ms左右 ，也就是1/20*1024，此时舵机角度为45°.
  get_pwm_info();  //打印信息，点击IDE右上角的串口查看器就可以看到打印的内容了
  delay(1000);
  ledcWrite(channel_PWM, 77);  //20ms高电平为1.5ms左右 ，也就是1.5/20*1024，此时舵机角度为90°.
  get_pwm_info();
  delay(1000);
  ledcWrite(channel_PWM, 102);  //20ms高电平为2ms左右 ，也就是2/20*1024，此时舵机角度为135°.
  get_pwm_info();
  delay(1000);
  ledcWrite(channel_PWM, 128);  //20ms高电平为2.5ms左右 ，也就是2.5/20*1024，此时舵机角度为180°.
  get_pwm_info();
  delay(1000);
  //ledcDetachPin(PWM_Pin);  //这个是解除IO口的pwm输出功能模式.
}
//*************************************************************************************
