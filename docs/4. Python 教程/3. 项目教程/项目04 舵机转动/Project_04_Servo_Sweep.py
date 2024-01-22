from machine import Pin, PWM
import time
 
 
p2 = PWM(Pin(4))  # 从引脚4创建PWM对象
p2.freq(50)  # 设置PWM频率从1Hz到40MHz
p2.duty(256)  # 将占空比从0到1023设置为占空比/1023，(现在是25%)
 
# 0度   P2.duty_u16(1638) #设置占空比从0到65535作为duty_u16/65535的比值
# 90度  p2.duty_u16(4915)
# 180度 p2.duty_u16(8192)
 
def main(): 
    while True:
        p2.duty_u16(1638)  # 0度
        time.sleep(1)
        p2.duty_u16(4915)  # 90度
        time.sleep(1)
        p2.duty_u16(8100)  # 180度  # 真实舵机MG90S可以设为8192
        time.sleep(1)
         
        for i in range(1638, 8100, 10):
            p2.duty_u16(i)
            time.sleep_ms(10)
            
        p2.duty_u16(1638)
        time.sleep(1)
#if __name__ == "__main__":        
main() 
    