# 项目15: WIFI 工作模式

在如今科技高速发展的时代，人们的生活质量越来越好，生活节奏越来越快，开始有人觉得复杂多样的智能化设备控制起来十分麻烦，通过手机统一控制智能化设备这种方法逐渐得到了人们的青睐。这种方法是利用单片机通过wifi模块和Internet网络建立手机和智能化设备之间的连接以此来实现对智能化设备的远程控制。在本章中，我们将重点关注ESP32的WiFi基础设施。ESP32有3种不同的WiFi工作模式：Station模式、AP模式和AP+Station模式。所有WiFi编程项目在使用WiFi前必须配置WiFi运行模式，否则无法使用WiFi。

## 项目15.1: WIFI Station 模式

### 1.实验元件：
|![Img](/media/img-20230330141831.png)|![Img](/media/img-20230329114812.png)|
| :--: | :--: |
| USB 线 x1|ESP32x1|

## 2.实验接线：
使用USB线将ESP32主板连接到电脑上的USB口。
![Img](/media/img-20230330142518.png)

## 3.元件知识： 
**Station 模式：** 当ESP32选择Station模式时，它作为一个WiFi客户端。它可以连接路由器网络，通过WiFi连接与路由器上的其他设备通信。如下图所示，PC和路由器已经连接，ESP32如果要与PC通信，需要将PC和路由器连接起来。
![Img](/media/img-20230330143626.png)

## 4.实验代码：
本教程中使用的代码保存在：“**..\Keyes Beetlebot ESP32 3合1 智能小车\4. Python 教程\3. 项目教程**” 的路径中。你可以把代码移到任何地方。例如，我们将代码保存在**D盘**中，<span style="color: rgb(0, 209, 0);">路径为D:\3. 项目教程</span>。
![Img](/media/img-20230331170623.png)

打开“**Thonny**”软件，点击“**此电脑**”→“**D:**”→“**3. 项目教程**”→“**项目15.1 WIFI Station 模式**”，然后鼠标左键双击“**Project_15.1_WiFi_Station_Mode.py**”。
![Img](/media/img-20230403150329.png)

由于各地的WiFi名称和密码是不同，所以在程序代码运行之前，用户需要在下图所示的框中输入正确的WiFi名称和密码。
![Img](/media/img-20230403144254.png)

```
import time
import network #导入网络模块

#请输入正确的路由器名称和密码
ssidRouter     = '********' #输入路由器名称
passwordRouter = '********' #输入路由器密码

def STA_Setup(ssidRouter,passwordRouter):
    print("Setup start")
    sta_if = network.WLAN(network.STA_IF) #设置ESP32为“Station 模式”
    if not sta_if.isconnected():
        print('connecting to',ssidRouter)
#激活ESP32的Station模式，向路由器发起连接请求，并输入连接密码.
        sta_if.active(True)
        sta_if.connect(ssidRouter,passwordRouter)
#等待ESP32与路由器连接成功.
        while not sta_if.isconnected():
            pass
#在“Shell”中打印分配给ESP32的IP地址.
    print('Connected, IP address:', sta_if.ifconfig())
    print("Setup End")

try:
    STA_Setup(ssidRouter,passwordRouter)
except:
    sta_if.disconnect()

```
## 5.实验现象：
由于各个地方的路由器名称和密码不同，所以在运行代码之前，用户需要在上图所示的红色框中输入正确的路由器名称和密码。
在确认正确输入路由器名和密码后，单击“![Img](/media/img-20230403103831.png)”，代码开始执行，等待ESP32连接到你的路由器，在“Shell”窗口中打印路由器分配给ESP32的IP地址。
![Img](/media/img-20230403154026.png)
![Img](/media/img-20230403145918.png)


## 项目15.2：WIFI AP 模式

## 1.实验元件&接线：
实验元件和实验接线与**项目15.1: WIFI Station 模式**相同。

## 2.元件知识：
**AP模式：** 当ESP32选择AP模式时，会创建一个独立于Internet的热点网络，等待其他WiFi设备连接。如下图所示，以ESP32为热点。如果手机或PC需要与ESP32通信，则必须连接到ESP32的热点。只有通过ESP32建立连接后才能进行通信。
![Img](/media/img-20230330144821.png)

## 3.实验代码：
本教程中使用的代码保存在：“**..\Keyes Beetlebot ESP32 3合1 智能小车\4. Python 教程\3. 项目教程**” 的路径中。你可以把代码移到任何地方。例如，我们将代码保存在**D盘**中，<span style="color: rgb(0, 209, 0);">路径为D:\3. 项目教程</span>。
![Img](/media/img-20230331170623.png)

打开“**Thonny**”软件，点击“**此电脑**”→“**D:**”→“**3. 项目教程**”→“**项目15.2 WIFI AP 模式**”，然后鼠标左键双击“**Project_15.2_WiFi_AP_Mode.py**”。
![Img](/media/img-20230403150152.png)

在程序代码运行之前，你可以像下图所示对框中ESP32的AP名称和密码进行任何更改。当然，在默认情况下，你也可以不修改它。
![Img](/media/img-20230403150528.png)

```
import network

#请输入正确的AP名称和密码.
ssidAP         = 'ESP32_WiFi' #输入AP名称
passwordAP     = '12345678'  #输入AP密码

local_IP       = '192.168.1.149'
gateway        = '192.168.1.1'
subnet         = '255.255.255.0'
dns            = '8.8.8.8'

#配置ESP32为AP模式.
ap_if = network.WLAN(network.AP_IF)

def AP_Setup(ssidAP,passwordAP):
    ap_if.ifconfig([local_IP,gateway,subnet,dns]) #配置ESP32的IP地址、网关和子网掩码.
    print("Setting soft-AP  ... ")
#在ESP32中打开AP，名称为ssid_AP，密码为password_AP
    ap_if.config(essid=ssidAP,authmode=network.AUTH_WPA_WPA2_PSK, password=passwordAP)
    ap_if.active(True)
    print('Success, IP address:', ap_if.ifconfig())
    print("Setup End\n")

try:
    AP_Setup(ssidAP,passwordAP)
except:
    print("Failed, please disconnect the power and restart the operation.")
    ap_if.disconnect() #如果程序运行不正常，将调用AP断开连接函数.

```
## 4.实验现象：
在程序代码运行之前，你可以像上图所示对红色框中ESP32的AP名称和密码进行任何更改。当然，在默认情况下，你也可以不修改它。
单击“![Img](/media/img-20230403103831.png)”，代码开始执行，打开ESP32的AP功能，“Shell”窗口中打印接入点信息。
![Img](/media/img-20230403151050.png)
![Img](/media/img-20230403151055.png)

观察串口监视器打印信息时，打开手机WiFi扫描功能，可以看到ESP32上的ssid_AP，在本程序代码中称为“ESP32_Wifi”。你可以输入密码“12345678”连接它，也可以通过修改程序代码来修改它的AP名称和密码。
![Img](/media/img-20230330145323.png)


## 项目15.3：WIFI AP+Station模式

## 1.实验元件&接线：
实验元件和实验接线与**项目15.1: WIFI Station 模式**和**项目15.2: WIFI AP 模式**相同。
## 2.元件知识：

**AP+Station模式：** ESP32除AP模式和Station模式外，还可以同时使用AP模式和Station模式。此模式包含前两种模式的功能。打开ESP32的Station模式，将其连接到路由器网络，它可以通过路由器与Internet通信。同时开启其AP模式，创建热点网络。其他WiFi设备可以选择连接路由器网络或热点网络与ESP32通信。

## 3.实验代码：
本教程中使用的代码保存在：“**..\Keyes Beetlebot ESP32 3合1 智能小车\4. Python 教程\3. 项目教程**” 的路径中。你可以把代码移到任何地方。例如，我们将代码保存在**D盘**中，<span style="color: rgb(0, 209, 0);">路径为D:\3. 项目教程</span>。
![Img](/media/img-20230331170623.png)

打开“**Thonny**”软件，点击“**此电脑**”→“**D:**”→“**3. 项目教程**”→“**项目15.2 WIFI AP 模式**”，然后鼠标左键双击“**Project_15.3_WiFi_AP_Station_Mode.py**”。
![Img](/media/img-20230403151746.png)

类似于**项目15.1: WIFI Station 模式**和**项目15.2: WIFI AP 模式**，在运行程序代码之前，你需要修改下图框中显示的ssid_Router、password_Router、ssid_AP和password_AP。如上图中的红色框所示：
![Img](/media/img-20230403152106.png)

```
import network

ssidRouter     = '********' #输入路由器名称
passwordRouter = '********' #输入路由器密码

ssidAP         = 'ESP32_WiFi'#输入AP名称
passwordAP     = '12345678' #输入AP密码

local_IP       = '192.168.1.149'
gateway        = '192.168.1.1'
subnet         = '255.255.255.0'
dns            = '8.8.8.8'

sta_if = network.WLAN(network.STA_IF)
ap_if = network.WLAN(network.AP_IF)
    
def STA_Setup(ssidRouter,passwordRouter):
    print("Setting soft-STA  ... ")
    if not sta_if.isconnected():
        print('connecting to',ssidRouter)
        sta_if.active(True)
        sta_if.connect(ssidRouter,passwordRouter)
        while not sta_if.isconnected():
            pass
    print('Connected, IP address:', sta_if.ifconfig())
    print("Setup End")
    
def AP_Setup(ssidAP,passwordAP):
    ap_if.ifconfig([local_IP,gateway,subnet,dns])
    print("Setting soft-AP  ... ")
    ap_if.config(essid=ssidAP,authmode=network.AUTH_WPA_WPA2_PSK, password=passwordAP)
    ap_if.active(True)
    print('Success, IP address:', ap_if.ifconfig())
    print("Setup End\n")

try:
    AP_Setup(ssidAP,passwordAP)    
    STA_Setup(ssidRouter,passwordRouter)
except:
    sta_if.disconnect()
    ap_if.idsconnect()

```
## 4.实验现象：
在确保正确修改代码后，单击“![Img](/media/img-20230403103831.png)”，代码开始执行，“Shell”窗口中将显示如下：
![Img](/media/img-20230403152902.png)
![Img](/media/img-20230403152319.png)

在观察串口显示器打印信息时，打开手机WiFi扫描功能，可以在ESP32上看到ssid_AP。
![Img](/media/img-20230330145323.png)











