from machine import Pin, PWM
import time

#定义GPIO4的输出频率为50Hz，占空比为77，分配给PWM.
servoPin = Pin(4)
pwm = PWM(servoPin, freq=50)
pwm.duty(77)
time.sleep(1)

#设置循迹传感器的引脚
tracking_left = Pin(17, Pin.IN)
tracking_right = Pin(16, Pin.IN)
# 右轮
pin1=Pin(32,Pin.OUT)
pin2=PWM(Pin(25),freq=50,duty=0)
# 左轮
pin3=Pin(33,Pin.OUT)
pin4=PWM(Pin(26),freq=50,duty=0)

# 作为小车前进的函数.
def car_forward(): 
  pin1.value(0)
  pin2.duty(200) 
  pin3.value(0)
  pin4.duty(200)  

# 作为小车向左行驶的函数.
def car_left(): 
  pin1.value(0)
  pin2.duty(180) 
  pin3.value(1)
  pin4.duty(1023)  

# 作为小车向右行驶的函数.
def car_right(): 
  pin1.value(1)
  pin2.duty(1023) 
  pin3.value(0)
  pin4.duty(220)

# 作为停车的函数.
def car_stop():
# pin2.deinit() # 如果使用microPython ESP32旧固件，就删除注释符号#
# pin4.deinit() # 或者有时候运行不了，就删除注释符号#，由于固件时不时会更新.
  pin1.value(0)
  pin2.duty(0) 
  pin3.value(0)
  pin4.duty(0)

while True:
    L_value = tracking_left.value() #左红外循迹值赋给变量L_value.
    R_value = tracking_right.value() #右红外循迹值赋给变量R_value.
    if L_value == 1 and R_value == 1: #左右红外循迹传感器均检测到黑线.
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_forward() #小车前进
    elif L_value == 1 and R_value == 0:
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_left()
    elif L_value == 0 and R_value == 1:
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_right()
    else:
        car_stop() 