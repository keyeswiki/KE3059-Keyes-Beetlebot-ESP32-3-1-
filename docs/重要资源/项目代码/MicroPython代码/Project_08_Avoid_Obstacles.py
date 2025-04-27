import utime as time
from machine import I2C, Pin, RTC, PWM 
from ht16k33matrix import HT16K33Matrix

# 定义GPIO4的输出频率为50Hz，占空比为77，并分配给PWM
servoPin = Pin(4)
pwm = PWM(servoPin, freq=50)
pwm.duty(77)
time.sleep(1)

# 设置超声波传感器的引脚和声速。
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
  pin2.duty(200) 
  pin3.value(0)
  pin4.duty(250)  

# 控制小车左转函数
def car_left():
  pin1.value(0)
  pin2.duty(500) 
  pin3.value(1)
  pin4.duty(500)  

# 控制小车右转函数
def car_right():
  pin1.value(1)
  pin2.duty(500) 
  pin3.value(0)
  pin4.duty(500)

# 控制小车停下函数
def car_stop():
  pin2.deinit()
  pin4.deinit()
  pin1.value(0)
#  pin2.duty(0) 
  pin3.value(0)
#  pin4.duty(0)
  
  
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

# 常量
DELAY = 0.01
PAUSE = 3
# 初始化
if __name__ == '__main__':
    i2c = I2C(scl=Pin(22), sda=Pin(21))
    display = HT16K33Matrix(i2c)
    display.set_brightness(2)

try:
    while True:
        Distance=getSonar() # 用超声波测量距离。
        if Distance>0 and Distance<10: # 如果距离大于0，小于10。
            car_stop() # 小车停下
            # 在LED上绘制自定义图标
            icon = b"\x18\x18\x18\x18\x18\x00\x00\x18"
            display.set_icon(icon).draw()
            # 旋转图标
            display.set_angle(0).draw()
            time.sleep(0.3)
            pwm = PWM(servoPin, freq=50)
            pwm.duty(128)
            time.sleep(0.5)
            a1=getSonar()
            time.sleep(0.3)
            pwm = PWM(servoPin, freq=50)
            pwm.duty(25)
            time.sleep(0.5)
            a2=getSonar()
            time.sleep(0.2)
            if a1>a2:
                pin2=PWM(Pin(25),freq=50)
                pin4=PWM(Pin(26),freq=50)
                car_left()
                icon = b"\x48\x24\x12\x09\x09\x12\x24\x48"
                display.set_icon(icon).draw()
                display.set_angle(0).draw()
                pwm = PWM(servoPin, freq=50)
                pwm.duty(77)
                time.sleep(0.5)
                icon = b"\x18\x24\x42\x99\x24\x42\x81\x00"
                display.set_icon(icon).draw()
                display.set_angle(0).draw()
            else:
                pin2=PWM(Pin(25),freq=50)
                pin4=PWM(Pin(26),freq=50)
                car_right()
                icon = b"\x12\x24\x48\x90\x90\x48\x24\x12"
                display.set_icon(icon).draw()
                # 旋转图标
                display.set_angle(0).draw()
                pwm = PWM(servoPin, freq=50)
                pwm.duty(77)
                time.sleep(0.5)
                icon = b"\x18\x24\x42\x99\x24\x42\x81\x00"
                display.set_icon(icon).draw()
                display.set_angle(0).draw()
        else:
            pin2=PWM(Pin(25),freq=50)
            pin4=PWM(Pin(26),freq=50)
            car_forward() # 小车前进
            icon = b"\x18\x24\x42\x99\x24\x42\x81\x00"
            display.set_icon(icon).draw()
            display.set_angle(0).draw()
except:
    pass