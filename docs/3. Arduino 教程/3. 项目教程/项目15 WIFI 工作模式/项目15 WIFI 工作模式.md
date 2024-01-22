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
由于各地的WiFi名称和密码是不同，所以在程序代码运行之前，用户需要在下图所示的框中输入正确的WiFi名称和密码。
![Img](/media/img-20230330144059.png)

```
//**********************************************************************************
/*
  项目 15.1: WiFi Station模式
  说明:使用ESP32连接到路由器
*/
#include <WiFi.h>
  
const char *ssid_Router     = "********"; //输入路由器名称
const char *password_Router = "********"; //输入路由器密码 

void setup(){
  Serial.begin(115200);
  delay(2000);
  Serial.println("Setup start");
  WiFi.begin(ssid_Router, password_Router);
  Serial.println(String("Connecting to ")+ssid_Router);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setup End");
}
 
void loop() {
}
//**********************************************************************************
```
## 5.实验现象：
编译并上传代码到ESP32主板上（<span style="color: rgb(255, 76, 65);">如果上传代码不成功，可以在点击![Img](/media/img-20230330092521.png)后用手按住ESP32主板上的Boot键，出现上传进度百分比再松开Boot键![Img](/media/img-20230331144331.png)</span>），利用USB线上电后，再打开串口监视器![Img](/media/img-20230330103640.png)，设置波特率为115200，当ESP32成功连接到ssid_WiFi时，串行监视器将打印出WiFi分配给ESP32的IP地址。然后串口监视器窗口将显示如下：
![Img](/media/img-20230413083930.png)


## 项目15.2：WIFI AP 模式

## 1.实验元件&接线：
实验元件和实验接线与**项目15.1: WIFI Station 模式**相同。

## 2.元件知识：
**AP模式：** 当ESP32选择AP模式时，会创建一个独立于Internet的热点网络，等待其他WiFi设备连接。如下图所示，以ESP32为热点。如果手机或PC需要与ESP32通信，则必须连接到ESP32的热点。只有通过ESP32建立连接后才能进行通信。
![Img](/media/img-20230330144821.png)

## 3.实验代码：
在程序代码运行之前，你可以像下图所示对框中ESP32的AP名称和密码进行任何更改。当然，在默认情况下，你也可以不修改它。
![Img](/media/img-20230330145132.png)

```
//*************************************************************************************
/*
  项目 15.2: WiFi AP模式
  说明:设置ESP32为打开接入点
*/
#include <WiFi.h>

const char *ssid_AP     = "ESP32_Wifi"; //输入AP名称
const char *password_AP = "12345678"; //输入AP密码

IPAddress local_IP(192,168,1,100);//设置ESP32本身的IP地址
IPAddress gateway(192,168,1,10);   //设置ESP32本身的网关
IPAddress subnet(255,255,255,0);  //设置ESP32本身的子网掩码

void setup(){
  Serial.begin(115200);
  delay(2000);
  Serial.println("Setting soft-AP configuration ... ");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.println("Setting soft-AP ... ");
  boolean result = WiFi.softAP(ssid_AP, password_AP);
  if(result){
    Serial.println("Ready");
    Serial.println(String("Soft-AP IP address = ") + WiFi.softAPIP().toString());
    Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
  }else{
    Serial.println("Failed!");
  }
  Serial.println("Setup End");
}
 
void loop() {
}
//*************************************************************************************
```
## 4.实验现象：
编译并上传代码到ESP32主板上（<span style="color: rgb(255, 76, 65);">如果上传代码不成功，可以在点击![Img](/media/img-20230330092521.png)后用手按住ESP32主板上的Boot键，出现上传进度百分比再松开Boot键![Img](/media/img-20230331144331.png)</span>），利用USB线上电后，再打开串口监视器![Img](/media/img-20230330103640.png)，设置波特率为115200，然后它将显示如下：
![Img](/media/img-20230413084002.png)

观察串口监视器打印信息时，打开手机WiFi扫描功能，可以看到ESP32上的ssid_AP，在本程序代码中称为“ESP32_Wifi”。你可以输入密码“12345678”连接它，也可以通过修改程序代码来修改它的AP名称和密码。
![Img](/media/img-20230330145323.png)


## 项目15.3：WIFI AP+Station模式

## 1.实验元件&接线：
实验元件和实验接线与**项目15.1: WIFI Station 模式**和**项目15.2: WIFI AP 模式**相同。
## 2.元件知识：

**AP+Station模式：** ESP32除AP模式和Station模式外，还可以同时使用AP模式和Station模式。此模式包含前两种模式的功能。打开ESP32的Station模式，将其连接到路由器网络，它可以通过路由器与Internet通信。同时开启其AP模式，创建热点网络。其他WiFi设备可以选择连接路由器网络或热点网络与ESP32通信。

## 3.实验代码：
类似于**项目15.1: WIFI Station 模式**和**项目15.2: WIFI AP 模式**，在运行程序代码之前，你需要修改下图框中显示的ssid_Router、password_Router、ssid_AP和password_AP。
![Img](/media/img-20230330145758.png)

```
//*************************************************************************************
/*
  项目 15.3: WiFi AP+Station模式
  描述:ESP32连接到用户的路由器，打开一个接入点
*/
#include <WiFi.h>
 
const char *ssid_Router     =  "********";  //输入路由器名称
const char *password_Router =  "********";  //输入路由器密码
const char *ssid_AP         =  "ESP32_Wifi"; //输入AP名称
const char *password_AP     =  "12345678";  //输入AP密码

void setup(){
  Serial.begin(115200);
  Serial.println("Setting soft-AP configuration ... ");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  Serial.println("Setting soft-AP ... ");
  boolean result = WiFi.softAP(ssid_AP, password_AP);
  if(result){
    Serial.println("Ready");
    Serial.println(String("Soft-AP IP address = ") + WiFi.softAPIP().toString());
    Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
  }else{
    Serial.println("Failed!");
  }
  
  Serial.println("\nSetting Station configuration ... ");
  WiFi.begin(ssid_Router, password_Router);
  Serial.println(String("Connecting to ")+ ssid_Router);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setup End");
}

void loop() {
}
//*************************************************************************************
```
## 4.实验现象：
编译并上传代码到ESP32主板上（<span style="color: rgb(255, 76, 65);">如果上传代码不成功，可以在点击![Img](/media/img-20230330092521.png)后用手按住ESP32主板上的Boot键，出现上传进度百分比再松开Boot键![Img](/media/img-20230331144331.png)</span>），利用USB线上电后，再打开串口监视器![Img](/media/img-20230330103640.png)，设置波特率为115200，然后串口监视器窗口将显示如下：
![Img](/media/img-20230413084129.png)
(<span style="color: rgb(255, 76, 65);">如果打开串口监视器且设置波特率为115200之后，串口监视器窗口没有显示如下信息，可以按下ESP32主板上的复位键.</span>

在观察串口显示器打印信息时，打开手机WiFi扫描功能，可以在ESP32上看到ssid_AP。
![Img](/media/img-20230330150315.png)











