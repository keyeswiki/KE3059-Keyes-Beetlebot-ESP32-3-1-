import machine
import time

# 在引脚4上创建一个PWM（脉冲宽度调制）对象
servo = machine.PWM(machine.Pin(4))

# 将PWM信号的频率设置为50hz，用于伺服系统
servo.freq(50)

# 为区间映射定义一个函数
def interval_mapping(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

# 定义一个函数来写入一个角度到伺服
def servo_write(pin, angle):

    pulse_width = interval_mapping(angle, 0, 180, 0.5, 2.5) # 计算脉冲宽度
    duty = int(interval_mapping(pulse_width, 0, 20, 0, 1023))     # 计算占空比
    pin.duty(duty) # 设置PWM信号的占空比

# 创建一个无限循环
while True:
    # 从0到180度的角度进行循环
    for angle in range(180):
        servo_write(servo, angle)
        time.sleep_ms(20)

    # 从180度到0度反向循环
    for angle in range(180, -1, -1):
        servo_write(servo, angle)
        time.sleep_ms(20)