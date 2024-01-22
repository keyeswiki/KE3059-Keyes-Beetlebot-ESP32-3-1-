from machine import ADC, Pin
import time
# 初始化GPIO34引脚的光阻(ADC函数)
adc = ADC(Pin(34))
adc.atten(ADC.ATTN_11DB)
adc.width(ADC.WIDTH_10BIT)

# 循环打印光敏传感器的adc值
try:
    while True:
        adcValue = adc.read() # 读取光敏传感器的ADC值
        print("ADC Value:", adcValue) #发送光敏传感器ADC值并打印结果.
        time.sleep(0.5)
except:
    pass