# 项目16 WIFI控制点阵屏

## 1.实验简介：
在前面的实验中，我们已经读取了APP按钮字符串。那么在本实验中，我们将使用ESP32的WIFI Station模式通过APP连接WIFI来控制小车上8×8点阵屏。

## 2.实验代码：
WiFi控制点阵屏显示图案
![Img](/media/img-20230331105323.png)
<span style="color: rgb(255, 76, 65);">特别注意：</span>需要先将实验代码![Img](/media/img-20230331103843.png)中的用户Wifi名称和用户Wifi密码改成你们自己的Wifi名称和Wifi密码。
## 3.实验APP操作及现象：
点击![Img](/media/img-20230331104105.png)将代码上传到ESP32主板上（<span style="color: rgb(255, 76, 65);">如果上传代码不成功，可以在点击![Img](/media/img-20230331104441.png)后用手按住ESP32主板上的Boot键，出现上传进度百分比再松开Boot键![Img](/media/img-20230331144331.png)</span>），小车安上电池，并且将电源开关拨到ON端，上电后，打开APP，连接上WiFi，点击![Img](/media/img-20230330154408.png)按钮，小车前面的8×8点阵显示“向前”图案；点击![Img](/media/img-20230330154416.png)按钮，小车前面的8×8点阵显示“停止”图案；点击![Img](/media/img-20230330154423.png)按钮，小车前面的8×8点阵显示“向后”图案。

















