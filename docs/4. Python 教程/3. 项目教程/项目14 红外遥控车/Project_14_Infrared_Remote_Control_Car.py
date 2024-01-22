#导入库
import utime as time
from machine import I2C, Pin, RTC, PWM 
from ht16k33matrix import HT16K33Matrix
from irrecvdata import irGetCMD 

recvPin = irGetCMD(19) #将红外解码器与引脚(19)关联.

#定义GPIO4的输出频率为50Hz，占空比为77，分配给PWM.
servoPin = Pin(4)
pwm = PWM(servoPin, freq=50)
pwm.duty(77)
time.sleep(1)

# 右轮
pin1=Pin(32,Pin.OUT)
pin2=PWM(Pin(25),freq=50,duty=0)
# 左轮
pin3=Pin(33,Pin.OUT)
pin4=PWM(Pin(26),freq=50,duty=0)

# 常量
DELAY = 0.01
PAUSE = 3

# 启动
if __name__ == '__main__':
    i2c = I2C(scl=Pin(22), sda=Pin(21))
    display = HT16K33Matrix(i2c)
    display.set_brightness(2)
    
# 作为小车前进的函数.
def car_forward(): 
  pin1.value(0)
  pin2.duty(1000) 
  pin3.value(0)
  pin4.duty(1000)  

# 作为小车后退的函数。
def car_back(): 
  pin1.value(1)
  pin2.duty(350) 
  pin3.value(1)
  pin4.duty(350)

# 作为小车向左行驶的函数.
def car_left(): 
  pin1.value(0)
  pin2.duty(1000) 
  pin3.value(1)
  pin4.duty(500)  

# 作为小车向右行驶的函数.
def car_right(): 
  pin1.value(1)
  pin2.duty(500) 
  pin3.value(0)
  pin4.duty(1000)

# 作为停车的函数.
def car_stop():
# pin2.deinit() # 如果使用microPython ESP32旧固件，就删除注释符号#
# pin4.deinit() # 或者有时候运行不了，就删除注释符号#，由于固件时不时会更新.
  pin1.value(0)
  pin2.duty(0) 
  pin3.value(0)
  pin4.duty(0)
  
def handleControl(value): 
    if value == '0xff629d':
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_forward() #小车前进
   # 点阵显示前进图案
        icon = b"\x18\x24\x42\x99\x24\x42\x81\x00"
        display.set_icon(icon).draw()
        display.set_angle(0).draw()
        time.sleep(PAUSE)
    elif value == '0xffa857':
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_back() # 小车后退
    # 点阵显示的后退图案
        icon = b"\x00\x81\x42\x24\x99\x42\x24\x18"
        display.set_icon(icon).draw()
        display.set_angle(0).draw()
        time.sleep(PAUSE)
    elif value == '0xff22dd': 
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_left() # 小车向左转
    # 点阵显示了向左图案
        icon = b"\x48\x24\x12\x09\x09\x12\x24\x48"
        display.set_icon(icon).draw()
        display.set_angle(0).draw()
        time.sleep(PAUSE)
    elif value == '0xffc23d': 
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_right() # 小车向右转
    # 点阵显示了向右图案
        icon = b"\x12\x24\x48\x90\x90\x48\x24\x12"
        display.set_icon(icon).draw()
        display.set_angle(0).draw()
        time.sleep(PAUSE)
    elif value == '0xff02fd': 
        car_stop() # 小车停止
    # 点阵显示停止图案
        icon = b"\x18\x18\x18\x18\x18\x00\x00\x18"
        display.set_icon(icon).draw()
        display.set_angle(0).draw()
        time.sleep(PAUSE)
        
def main():
    while True:
        irValue = recvPin.ir_read() #调用ir_read()来读取所按键的值并将其赋值给IRValue.
        if irValue:
            print(irValue)
            handleControl(irValue)
#if __name__ == "__main__":        
main()