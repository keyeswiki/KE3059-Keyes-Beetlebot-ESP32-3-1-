from machine import Pin, PWM
import time

#定义GPIO4的输出频率为50Hz，占空比为77，分配给PWM
servoPin = Pin(4)
pwm = PWM(servoPin, freq=50)
pwm.duty(77)
time.sleep(1)