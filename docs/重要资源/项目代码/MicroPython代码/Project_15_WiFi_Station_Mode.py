import time
import network # 导入网络模块

# 请输入正确的路由器名称和密码
ssidRouter     = 'REPLACE_WITH_YOUR_SSID'      # 输入路由器名称
passwordRouter = 'REPLACE_WITH_YOUR_PASSWORD'  # 输入路由器密码

def STA_Setup(ssidRouter,passwordRouter):
    print("Setup start")
    sta_if = network.WLAN(network.STA_IF) # 将ESP32设置为“Station”模式
    if not sta_if.isconnected():
        print('connecting to',ssidRouter)
# 激活ESP32的Station模式，向路由器发起连接请求
# 然后输入密码进行连接
        sta_if.active(True)
        sta_if.connect(ssidRouter,passwordRouter)
# 等待ESP32连接到路由器，直到两者连接成功。
        while not sta_if.isconnected():
            pass
# 在“Shell”中打印分配给ESP32的IP地址。
    print('Connected, IP address:', sta_if.ifconfig())
    print("Setup End")

try:
    STA_Setup(ssidRouter,passwordRouter)
except:
    sta_if.disconnect()