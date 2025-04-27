from machine import Pin, ADC, PWM
import time

# 定义GPIO4的输出频率为50Hz，占空比为77，并分配给PWM
servoPin = Pin(4)
pwm = PWM(servoPin, freq=50)
pwm.duty(77)
time.sleep(1)

# 初始化左光阻至引脚34 （ADC函数）
adc1 = ADC(Pin(34))
adc1.atten(ADC.ATTN_11DB)
adc1.width(ADC.WIDTH_10BIT)
# 初始化右光阻至引脚35 （ADC函数）
adc2 = ADC(Pin(35))
adc2.atten(ADC.ATTN_11DB)
adc2.width(ADC.WIDTH_10BIT)

# 右轮
pin1=Pin(32,Pin.OUT)
pin2=PWM(Pin(25),freq=50,duty=0)
# 左轮
pin3=Pin(33,Pin.OUT)
pin4=PWM(Pin(26),freq=50,duty=0)

# 控制小车前进函数
def car_forward(): 
  pin1.value(0)
  pin2.duty(300) 
  pin3.value(0)
  pin4.duty(300)  

# 控制小车后退函数
def car_back(): 
  pin1.value(1)
  pin2.duty(200) 
  pin3.value(1)
  pin4.duty(200)

# 控制小车左转函数
def car_left(): 
  pin1.value(0)
  pin2.duty(200) 
  pin3.value(1)
  pin4.duty(900)  

# 控制小车右转函数
def car_right(): 
  pin1.value(1)
  pin2.duty(900) 
  pin3.value(0)
  pin4.duty(200)

# 控制小车停下函数
def car_stop():
  pin2.deinit()
  pin4.deinit() 
  pin1.value(0)
#  pin2.duty(0) 
  pin3.value(0)
#  pin4.duty(0)   

try:
    while True:
        adcValue1 = adc1.read() # 读取左光敏电阻的ADC值。
        adcValue2 = adc2.read() # 读取右光敏电阻的ADC值。
        print("ADC Value1:", adcValue1 ,"ADC Value2:", adcValue2)
        time.sleep(0.5)
        if adcValue1 > 700 and adcValue2 > 700: # 由左、右光阻测量的范围值。
            pin2=PWM(Pin(25),freq=50)
            pin4=PWM(Pin(26),freq=50)
            car_forward() # 小车前进
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
except:
    pass