from machine import Pin,PWM
import time

# 右轮
pin1=Pin(32,Pin.OUT)
pin2=PWM(Pin(25),freq=12500)

# 左轮
pin3=Pin(33,Pin.OUT)
pin4=PWM(Pin(26),freq=12500)

# 作为小车前进的函数.
def car_forward(): 
  pin1.value(0)
  pin2.duty(1000) 
  pin3.value(0)
  pin4.duty(1000)  

# 作为小车后退的函数.
def car_back(): 
  pin1.value(1)
  pin2.duty(250) 
  pin3.value(1)
  pin4.duty(250)

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
  pin1.value(0)
  pin2.duty(0) 
  pin3.value(0)
  pin4.duty(0)   
try:
    while True:
        car_forward() #小车前进
        time.sleep(2) # 延时2s
        car_back() # 小车后退
        time.sleep(2)    
        car_left() # 小车向左行驶
        time.sleep(2)    
        car_right() # 小车向右行驶
        time.sleep(2)    
        car_stop() # 小车停止
        time.sleep(2)     
except:
    pass