# Beetlebot ESP32 3合1 智能小车初始形态安装步骤
![Img](/media/img-20230403160548.png)

 安装1

安装所需零件：
![Img](/media/img-20230403160628.png)

安装：
![Img](/media/img-20230403160639.png)

完成：
![Img](/media/img-20230403160651.png)

 安装2

安装所需零件：
![Img](/media/img-20230403160715.png)

安装：
![Img](/media/img-20230403160726.png)

完成：
![Img](/media/img-20230403160738.png)

 安装3

安装所需零件：
![Img](/media/img-20230403160945.png)

安装：
![Img](/media/img-20230403160957.png)

完成：
![Img](/media/img-20230403161008.png)

 安装4

安装所需零件：
![Img](/media/img-20230403161032.png)

安装：
![Img](/media/img-20230403161044.png)

完成：
![Img](/media/img-20230403161056.png)

 安装5

安装所需零件：
![Img](/media/img-20230403161121.png)

<span style="color: rgb(255, 76, 65);">先将电机、8*8点阵屏和上下pcb板的连接线都接上。</span>

|左电机|右电机|
| :--: | :--: |
|**L**|**R**|

![Img](/media/img-20230508105400.png)

|8*8点阵|小车PCB板|
| :--: | :--: |
|G|G|
|5V|5V|
|SDA|SDA|
|SCL|SCL|
![Img](/media/img-20230508111302.png)
![Img](/media/img-20230508105414.png)

安装：
![Img](/media/img-20230508112106.png)

完成：
![Img](/media/img-20230403161648.png)

 安装6

安装所需零件：
![Img](/media/img-20230403161716.png)

安 装：
![Img](/media/img-20230403161733.png)

完成：
![Img](/media/img-20230403161752.png)

 安装7

安装所需零件：
![Img](/media/img-20230403161815.png)

安装：
![Img](/media/img-20230403162348.png)

完成：
![Img](/media/img-20230403162403.png)

 安装8

安装所需零件：
![Img](/media/img-20230403162701.png)

安装：
![Img](/media/img-20230403162930.png)

完成：
![Img](/media/img-20230403162721.png)

 安装9

安装所需零件：
![Img](/media/img-20230403163656.png)

<span style="color: rgb(255, 76, 65);">安装前需要调节舵机角度为90°。</span>

|舵机|小车PCB板|
| :--: | :--: |
|棕线| G |
| 红线 | 5V |
| 橙黄线 | S1（GPIO4）|
![Img](/media/img-20230508091835.png)

```
include <Arduino.h>
// 舵机通道 
int channel_PWM = 3;  
// 舵机频率，那么周期也就是1/50，也就是20ms ，PWM一共有16个通道，0-7位高速通道由80Mhz时钟驱动，后面8个为低速通道由1Mhz时钟驱动.
int freq_PWM = 50;   
// PWM分辨率，取值为 0-20 之间  ，这里填写为10，那么后面的ledcWrite这个里面填写的pwm值就在0 - 2的10次方 之间 也就是 0-1024 ，如果是要求不高的东西你可以直接拿1000去算了.
int resolution_PWM = 10;   
// 绑定的IO，在下面的绑定函数里面会用到，绑定之后这个IO就会变成我们PWM的输出口.
const int PWM_Pin = 4;  //指定pwm绑定到GPIO4输出.
void setup() {
  ledcSetup(channel_PWM, freq_PWM, resolution_PWM); // 设置舵机通道,舵机频率,PWM分辨率.
  ledcAttachPin(PWM_Pin, channel_PWM);  //将LEDC通道绑定到指定 IO 口上以实现输出
}
void loop() {
  ledcWrite(channel_PWM, 77);  //20ms高电平为1.5ms左右 ，也就是1.5/20*1024，此时舵机角度为90°.
}

```
上面的Arduino代码在资料中有提供，打开舵机的调整代码并烧录到Beetlebot的ESP32主板，完成。代码如下图位置：
![Img](/media/img-20230425083450.png)

调整之后，正向前安装：
![Img](/media/img-20230403164300.png)

完成：
![Img](/media/img-20230403164312.png)

 安装10

安装所需零件：
![Img](/media/img-20230403164357.png)

安装：
![Img](/media/img-20230403164606.png)

完成：
![Img](/media/img-20230403164628.png)

 安装11

安装所需零件：
![Img](/media/img-20230403164712.png)

安装：
![Img](/media/img-20230403164750.png)

完成：
![Img](/media/img-20230403164808.png)

 初始形态接线图

超声波接线图：
|超声波传感器| 小车PCB板 |
| :--: | :--: |
|Vcc|5V|
|Trig|S2（GPIO5）|
|Echo|S1（GPIO18）|
| Gnd |G|
![Img](/media/img-20230508100004.png)
![Img](/media/img-20230505145136.jpg)

云台舵机接线图：
| 舵机 | 小车PCB板 |
| :--: | :--: |
| 棕线 | G |
| 红线 | 5V |
| 橙黄线 | S1（GPIO4） |
![Img](/media/img-20230508091835.png)
![Img](/media/img-20230505145537.jpg)

左边光敏接线图：
|左边光敏传感器| 小车PCB板 |
| :--: | :--: |
| G| G |
| V| V |
|S|S（GPIO34）|
![Img](/media/img-20230508101203.png)
![Img](/media/img-20230505150208.png)
![Img](/media/img-20230505150639.jpg)

右边光敏接线图：
|右边光敏传感器| 小车PCB板 |
| :--: | :--: |
| G| G |
| V| V |
|S|S（GPIO35）|
![Img](/media/img-20230508101729.png)
![Img](/media/img-20230505151028.jpg)
![Img](/media/img-20230505151526.jpg)

 原型态安装完成示意图：
![Img](/media/img-20230506135431.png)






