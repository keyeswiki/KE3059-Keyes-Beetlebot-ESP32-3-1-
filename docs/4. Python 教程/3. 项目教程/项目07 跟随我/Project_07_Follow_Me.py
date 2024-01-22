from machine import Pin, PWM
import time

#定义GPIO4的输出频率为50Hz，占空比为77，分配给PWM
servoPin = Pin(4)
pwm = PWM(servoPin, freq=50)
pwm.duty(77)
time.sleep(1)

#设置超声波传感器的引脚和声速
trigPin=Pin(5,Pin.OUT,0)
echoPin=Pin(18,Pin.IN,0)
soundVelocity=340
distance=0
# 右轮
pin1=Pin(32,Pin.OUT)
pin2=PWM(Pin(25),freq=50,duty=0)
# 左轮
pin3=Pin(33,Pin.OUT)
pin4=PWM(Pin(26),freq=50,duty=0)

# 作为小车前进的函数
def car_forward(): 
  pin1.value(0)
  pin2.duty(400) 
  pin3.value(0)
  pin4.duty(400)  

# 作为小车后退的函数.
def car_back(): 
  pin1.value(1)
  pin2.duty(550) 
  pin3.value(1)
  pin4.duty(550)
  
# 作为停车的函数.  
def car_stop():
  pin1.value(0)
  pin2.duty(0) 
  pin3.value(0)
  pin4.duty(0)
# pin2.deinit() # 如果使用microPython ESP32旧固件，就删除注释符号#
# pin4.deinit() # 或者有时候运行不了，就删除注释符号#，由于固件时不时会更新.

#子函数getSonar()用于启动超声波模块以开始测量，并返回以厘米为单位的测量距离.
#在这个功能中，首先让trigPin发送10us高电平来启动超声波模块.
#然后使用pulseIn()读取超声波模块，返回高电平的持续时间.
#最后，根据时间计算被测距离.
def getSonar(): 
    trigPin.value(1)
    time.sleep_us(10)
    trigPin.value(0)
    while not echoPin.value():
        pass
    pingStart=time.ticks_us()
    while echoPin.value():
        pass
    pingStop=time.ticks_us()
    pingTime=time.ticks_diff(pingStop,pingStart)
    distance=pingTime*soundVelocity//2//10000
    time.sleep_ms(10)
    return int(distance)


while True:
    Distance=getSonar() #用超声波测量距离.
    print(Distance) #发送一个脉冲以厘米为单位计算距离并打印结果.
    if Distance<8: #如果距离小于8，则
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_back() #小车后退
    elif Distance>=8 and Distance<13: #如果距离大于或等于8小于13，则
        car_stop() # 小车停止
    elif Distance>=13 and Distance<35: #如果距离大于或等于13小于35，则
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_forward() #小车前进
    else:
        car_stop()

