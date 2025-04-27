from machine import Pin
import time

# 定义超声波模块的控制引脚 
trigPin=Pin(5,Pin.OUT,0)
echoPin=Pin(18,Pin.IN,0)
# 设置声速
soundVelocity=340
distance=0

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
    return int(distance)

# 延时2秒，等待超声波模块稳定；
# 打印每500毫秒从超声波模块获得的数据。
time.sleep_ms(2000)
while True:
    time.sleep_ms(500)
    print('Distance: ',getSonar(),'cm' ) 