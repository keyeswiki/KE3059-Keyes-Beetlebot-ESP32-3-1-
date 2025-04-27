from machine import Pin,PWM
import time

# 右轮
pin1=Pin(32,Pin.OUT)
pin2=PWM(Pin(25),freq=12500)

# 左轮
pin3=Pin(33,Pin.OUT)
pin4=PWM(Pin(26),freq=12500)

# 用来控制小车前进的函数 
def car_forward(): 
  pin1.value(0)
  pin2.duty(1000) 
  pin3.value(0)
  pin4.duty(1000)  

# 用来控制小车后退的函数
def car_back(): 
  pin1.value(1)
  pin2.duty(250) 
  pin3.value(1)
  pin4.duty(250)

# 用来控制小车左转的函数
def car_left(): 
  pin1.value(0)
  pin2.duty(1000) 
  pin3.value(1)
  pin4.duty(500)  

# 用来控制小车右转的函数 
def car_right(): 
  pin1.value(1)
  pin2.duty(500) 
  pin3.value(0)
  pin4.duty(1000)

# 用来控制小车停止的函数
def car_stop(): 
  pin1.value(0)
  pin2.duty(0) 
  pin3.value(0)
  pin4.duty(0)   
try:
    while True:
        car_forward() #前进
        time.sleep(2) # 延时 2s
        car_back() # 后退
        time.sleep(2)    
        car_left() # 左转
        time.sleep(2)    
        car_right() # 右转
        time.sleep(2)    
        car_stop() # 停止
        time.sleep(2)     
except:
    pass  