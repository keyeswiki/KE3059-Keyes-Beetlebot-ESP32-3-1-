from machine import Pin
import time

#定义超声波测距模块的控制引脚.  
trigPin=Pin(5,Pin.OUT,0)
echoPin=Pin(18,Pin.IN,0)
#设置声速.
soundVelocity=340
distance=0

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
    return int(distance)

#延迟2秒，等待超声模块稳定，每500毫秒打印一次超声模块的数据.
time.sleep_ms(2000)
while True:
    time.sleep_ms(500)
    print('Distance: ',getSonar(),'cm' ) 