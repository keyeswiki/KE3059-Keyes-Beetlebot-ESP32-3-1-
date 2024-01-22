//*************************************************************************************
#include <Arduino.h>

// 舵机通道 
int channel_PWM = 4;  
// 舵机频率，那么周期也就是1/50，也就是20ms ，PWM一共有16个通道，0-7位高速通道由80Mhz时钟驱动，后面8个为低速通道由1Mhz时钟驱动.
int freq_PWM = 50;   
// PWM分辨率，取值为 0-20 之间  ，这里填写为10，那么后面的ledcWrite这个里面填写的pwm值就在0 - 2的10次方 之间 也就是 0-1024 ，如果是要求不高的东西你可以直接拿1000去算了.
int resolution_PWM = 10;   
// 绑定的IO，在下面的绑定函数里面会用到，绑定之后这个IO就会变成我们PWM的输出口.
const int PWM_Pin = 23;  //指定pwm绑定到GPIO23输出.

void setup() {
  ledcSetup(channel_PWM, freq_PWM, resolution_PWM); // 设置舵机通道,舵机频率,PWM分辨率.
  ledcAttachPin(PWM_Pin, channel_PWM);  //将LEDC通道绑定到指定 IO 口上以实现输出
}

void loop() {
  ledcWrite(channel_PWM, 25);  //20ms高电平为1.5ms左右 ，也就是0.5/20*1024，此时舵机角度为0°.
}
//*************************************************************************************
