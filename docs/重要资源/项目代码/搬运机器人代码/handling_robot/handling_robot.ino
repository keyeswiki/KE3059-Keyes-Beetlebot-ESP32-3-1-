#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>

#include <ESP32Servo.h>
//电机
#define left_ctrl  33  //左电机的方向控制引脚 gpio33
#define left_pwm  26   //左电机的PWM控制引脚 gpio26
#define right_ctrl  32 //右电机的方向控制引脚 gpio32
#define right_pwm  25  //右电机的PWM控制引脚 gpio25

/*替换为您的网络凭据（输入您的SSID和密码）*/
const char* ssid = "REPLACE_WITH_YOUR_SSID"; //此处输入SSID
const char* password = "REPLACE_WITH_YOUR_PASSWORD"; //此处输入密码
// 端口80的TCP服务器将响应HTTP请求
WiFiServer server(80);

//舵机
const int servoPin = 23;//设置舵机引脚为gpio23
Servo myservo;  // 创建舵机对象来控制舵机

void setup(void)
{
    Serial.begin(115200);
    pinMode(left_ctrl,OUTPUT); //将左电机控制引脚设置为OUTPUT
    ledcAttach(left_pwm, 1200, 8); //设置left_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
    pinMode(right_ctrl,OUTPUT);//设置右侧电机方向控制引脚为OUTPUT
    ledcAttach(right_pwm, 1200, 8); //设置right_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
  
    myservo.setPeriodHertz(50);           // 标准50赫兹舵机
    myservo.attach(servoPin, 500, 2500);  // 将servoPin上的舵机附加到舵机对象上
    myservo.write(180);  // 舵机的初始角度设置为180°
    delay(300);

    // 连接WiFi网络
    WiFi.begin(ssid, password);
    Serial.println("");

    // 等待连接
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // 设置mDNS响应器：
    // - 在本例中，第一个参数是域名
    //   全限定域名是“esp32.local”
    // - 第二个参数是要发布的IP地址
    //   我们通过WiFi网络发送我们的IP地址
    if (!MDNS.begin("esp32")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");

    // 启动TCP （HTTP）服务器
    server.begin();
    Serial.println("TCP server started");

    // 向mmds - sd添加业务
    MDNS.addService("http", "tcp", 80);
}

void loop(void)
{
    // 检查客户端是否已连接
    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    //Serial.println("");
    //Serial.println("New client");

    // 等待来自客户端的数据变为可用
    while(client.connected() && !client.available()){
        delay(1);
    }

    // 读取HTTP请求的第一行
    String req = client.readStringUntil('\r');
    //Serial.println(req);
    // HTTP请求的第一行看起来像“GET /path HTTP/1.1”
    // 通过查找空格来检索“/path”部分
    int addr_start = req.indexOf(' ');
    int addr_end = req.indexOf(' ', addr_start + 1);
    if (addr_start == -1 || addr_end == -1) {
        //Serial.print("Invalid request: ");
        //Serial.println(req);
        return;
    }
    req = req.substring(addr_start + 1, addr_end);
    //Serial.print("Request: ");
    Serial.println(req);
    String s;
    if (req == "/")
    {
        IPAddress ip = WiFi.localIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP32 at ";
        s += ipStr;
        s += "</html>\r\n\r\n";
        Serial.println("Sending 200");
        client.println(WiFi.localIP());
    }
    else if(req == "/btn/F")
    {
      digitalWrite(left_ctrl,LOW); //将左电机方向控制引脚设为LOW。
      ledcWrite(left_pwm, 150); //左电机输出PWM 150
      digitalWrite(right_ctrl,LOW); //将右电机的控制引脚设置为LOW。
      ledcWrite(right_pwm, 150); //右电机输出PWM 150
    }
    else if(req == "/btn/B")
    {
      digitalWrite(left_ctrl, HIGH);
      ledcWrite(left_pwm, 150);
      digitalWrite(right_ctrl, HIGH);
      ledcWrite(right_pwm, 150);
    }
    else if(req == "/btn/L")
    {
      digitalWrite(left_ctrl,HIGH); 
      ledcWrite(left_pwm, 150); 
      digitalWrite(right_ctrl,LOW); 
      ledcWrite(right_pwm, 150);
    }
    else if(req == "/btn/R")
    {
      digitalWrite(left_ctrl,LOW); 
      ledcWrite(left_pwm, 150); 
      digitalWrite(right_ctrl,HIGH); 
      ledcWrite(right_pwm, 150);
    }
    else if(req == "/btn/S")
    {
      digitalWrite(left_ctrl,LOW);
      ledcWrite(left_pwm, 0);
      digitalWrite(right_ctrl,LOW);
      ledcWrite(right_pwm, 0); 
    }
    else if(req == "/btn/p")
    {
      Serial.write('p');
      myservo.write(130);
      delay(200);
    }
    else if(req == "/btn/q")
    {
      Serial.write('q');
      client.println("q");
    }
    else if(req == "/btn/x")
    {
      myservo.write(180);
      delay(200);
    }
}

