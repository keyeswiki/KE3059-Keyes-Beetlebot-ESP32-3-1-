from machine import ADC, Pin
import time
# 将光阻初始化为引脚34 （ADC函数）
adc = ADC(Pin(34))
adc.atten(ADC.ATTN_11DB)
adc.width(ADC.WIDTH_10BIT)

# 循环打印光阻的当前adc值
try:
    while True:
        adcValue = adc.read() # 读取光阻的ADC值
        print("ADC Value:", adcValue) # 发送光阻ADC值并打印结果
        time.sleep(0.5)
except:
    pass