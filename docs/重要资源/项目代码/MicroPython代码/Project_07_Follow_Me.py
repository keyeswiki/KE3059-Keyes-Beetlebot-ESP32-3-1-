from machine import Pin, PWM
import time

# 定义GPIO4的输出频率为50Hz，占空比为77，并分配给PWM
servoPin = Pin(4)
pwm = PWM(servoPin, freq=50)
pwm.duty(77)
time.sleep(1)

# 设置超声波传感器的引脚和声速
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

# 控制小车前进函数
def car_forward(): 
  pin1.value(0)
  pin2.duty(800) 
  pin3.value(0)
  pin4.duty(800)  

# 控制小车后退函数
def car_back(): 
  pin1.value(1)
  pin2.duty(550) 
  pin3.value(1)
  pin4.duty(550)
  
# 控制小车停下函数  
def car_stop():
  pin2.deinit()
  pin4.deinit()
  pin1.value(0)
 # pin2.duty(0) 
  pin3.value(0)
 # pin4.duty(0)

# 子函数 getSonar() 用于启动超声波模块开始测量，然后返回测量到的距离，单位是厘米。
# 在这个函数中，首先让trigPin发送10us高电平启动超声波模块。
# 然后 pulseIn() 读取超声波模块并返回高电平持续时间。
# 最后根据时间计算出测量到的距离。
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

try:
    while True:
        Distance=getSonar() # 用超声波测量距离。
        print(Distance) # 发送一个脉冲来计算以厘米为单位的距离，并打印结果。
        if Distance<8: # 如果距离小于8
            pin2=PWM(Pin(25),freq=50)
            pin4=PWM(Pin(26),freq=50)
            car_back() # 小车后退
        elif Distance>=8 and Distance<13: # 如果这个距离大于等于8，小于13。
            car_stop() # 小车停下
        elif Distance>=13 and Distance<35: # 如果这个距离大于等于13，小于35。
            pin2=PWM(Pin(25),freq=50)
            pin4=PWM(Pin(26),freq=50)
            car_forward() # 小车前进
        else:
           car_stop() 
except:
    pass     
