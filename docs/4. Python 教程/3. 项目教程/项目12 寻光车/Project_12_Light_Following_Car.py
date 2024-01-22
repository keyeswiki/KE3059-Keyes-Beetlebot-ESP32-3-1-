from machine import Pin, ADC, PWM
import time

#定义GPIO4的输出频率为50Hz，占空比为77，分配给PWM.
servoPin = Pin(4)
pwm = PWM(servoPin, freq=50)
pwm.duty(77)
time.sleep(1)

# 初始化左光敏传感器GPIO34引脚(ADC函数)
adc1 = ADC(Pin(34))
adc1.atten(ADC.ATTN_11DB)
adc1.width(ADC.WIDTH_10BIT)
# 初始化右光敏传感器GPIO35引脚(ADC函数)
adc2 = ADC(Pin(35))
adc2.atten(ADC.ATTN_11DB)
adc2.width(ADC.WIDTH_10BIT)

# 右轮
pin1=Pin(32,Pin.OUT)
pin2=PWM(Pin(25),freq=50,duty=0)
# 左轮
pin3=Pin(33,Pin.OUT)
pin4=PWM(Pin(26),freq=50,duty=0)

# 作为小车前进的函数.
def car_forward(): 
  pin1.value(0)
  pin2.duty(300) 
  pin3.value(0)
  pin4.duty(300)  

# 作为小车后退的函数.
def car_back(): 
  pin1.value(1)
  pin2.duty(200) 
  pin3.value(1)
  pin4.duty(200)

# 作为小车向左行驶的函数.
def car_left(): 
  pin1.value(0)
  pin2.duty(200) 
  pin3.value(1)
  pin4.duty(900)  

# 作为小车向右行驶的函数.
def car_right(): 
  pin1.value(1)
  pin2.duty(900) 
  pin3.value(0)
  pin4.duty(200)

# 作为停车的函数.
def car_stop():
# pin2.deinit() # 如果使用microPython ESP32旧固件，就删除注释符号#
# pin4.deinit() # 或者有时候运行不了，就删除注释符号#，由于固件时不时会更新. 
  pin1.value(0)
  pin2.duty(0) 
  pin3.value(0)
  pin4.duty(0)   

while True:
    adcValue1 = adc1.read() # 读取左光敏传感器的ADC值.
    adcValue2 = adc2.read() # 读取右光敏传感器的ADC值.
    print("ADC Value1:", adcValue1 ,"ADC Value2:", adcValue2)
    time.sleep(0.5)
    if adcValue1 > 700 and adcValue2 > 700: #由左右光敏传感器测量的光线强度值.
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_forward() #小车前进
    elif adcValue1 > 700 and adcValue2 <= 700:
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50) 
        car_left()
    elif adcValue1 <= 700 and adcValue2 > 700:
        pin2=PWM(Pin(25),freq=50)
        pin4=PWM(Pin(26),freq=50)
        car_right()
    else:
        car_stop()