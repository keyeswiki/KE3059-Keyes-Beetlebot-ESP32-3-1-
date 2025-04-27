import network

# 请输入正确的AP名称和密码。
ssidAP         = 'ESP32_WiFi' # 输入AP名称
passwordAP     = '12345678'   # 输入AP密码

local_IP       = '192.168.1.149'
gateway        = '192.168.1.1'
subnet         = '255.255.255.0'
dns            = '8.8.8.8'

# 将ESP32设置为AP模式
ap_if = network.WLAN(network.AP_IF)

def AP_Setup(ssidAP,passwordAP):
    ap_if.ifconfig([local_IP,gateway,subnet,dns]) # 配置ESP32的IP地址、网关和子网掩码。
    print("Setting soft-AP  ... ")
# 打开ESP32中名称为ssid_AP、密码为password_AP的AP
    ap_if.config(essid=ssidAP,authmode=network.AUTH_WPA_WPA2_PSK, password=passwordAP)
    ap_if.active(True)
    print('Success, IP address:', ap_if.ifconfig())
    print("Setup End\n")

try:
    AP_Setup(ssidAP,passwordAP)
except:
    print("Failed, please disconnect the power and restart the operation.")
    ap_if.disconnect() # 如果程序运行异常，将调用AP断连功能。