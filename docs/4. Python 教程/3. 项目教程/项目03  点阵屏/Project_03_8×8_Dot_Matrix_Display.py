# 导入库文件
import utime as time
from machine import I2C, Pin, RTC
from ht16k33matrix import HT16K33Matrix

# 常量
DELAY = 0.01
PAUSE = 3

def main():
    # 开始/启动
    if __name__ == '__main__':
        i2c = I2C(scl=Pin(22), sda=Pin(21))
        display = HT16K33Matrix(i2c)
        display.set_brightness(2)

        # 在LED上绘制一个自定义图标
        icon = b"\x00\x66\x00\x00\x18\x42\x3c\x00"
        display.set_icon(icon).draw()
        # 旋转图标
        display.set_angle(0).draw()
        time.sleep(PAUSE)
#if __name__ == "__main__":        
main()