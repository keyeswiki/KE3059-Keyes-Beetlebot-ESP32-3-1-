#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>

#define INA 32
#define PWMA 25
#define INB 33
#define PWMB 26

const char* ssid = "ChinaNet-2.4G-0DF0";
const char* password = "ChinaNet@233";
// 端口80的TCP服务器将响应HTTP请求
WiFiServer server(80);

//舵机
int channel_PWM = 3; // 舵机通道
// 舵机频率，那么周期也就是1/50，也就是20ms ，PWM一共有16个通道，0-7位高速通道由80Mhz时钟驱动，后面8个为低速通道由1Mhz时钟驱动.
int freq_PWM = 50;
// PWM分辨率，取值为 0-20 之间  ，这里填写为10，那么后面的ledcWrite 这个里面填写的pwm值就在0-2的10次方之间，也就是0-1024，如果是要求不高的东西你可以直接拿1000去算了.
int resolution_PWM = 10;
// 绑定的IO，在下面的绑定函数里面会用到，绑定之后这个IO就会变成我们PWM的输出口.
const int servopin = 4;//定义舵机的IO脚位在gpio4.

void setup(void)
{
    Serial.begin(115200);
    pinMode(INA, OUTPUT);
    ledcAttachPin(PWMA, 0);
    ledcSetup(0, 50, 8);//设置LEDC通道1频率为1200，PWM分辨率为8，即占空比256.
    pinMode(INB, OUTPUT);
    ledcAttachPin(PWMB,1);
    ledcSetup(1, 50, 8);//设置LEDC通道为2频率为1200，PWM分辨率为8，即占空比256.
    
    ledcSetup(3, 50, 10); // 设置舵机通道3频率为50,PWM分辨率为10.
    ledcAttachPin(23, 3);  //将 LEDC 通道绑定到指定 IO 口上以实现输出.
    ledcWrite(channel_PWM, set_angle(0));
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

    // 设置mDNS响应器:
    // - 第一个参数是域名，在本例中，完全限定域名为"esp32.local"
    // - 第二个参数是广告的IP地址，我们在WiFi网络上发送我们的IP地址
    if (!MDNS.begin("esp32")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");

    // 启动TCP (HTTP)服务器
    server.begin();
    Serial.println("TCP server started");

    // 添加服务到MDNS-SD
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

    // 等待来自客户端的数据变得可用
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
      digitalWrite(INA, LOW);
      ledcWrite(0, 100);
      digitalWrite(INB, LOW);
      ledcWrite(1, 100);
    }
    else if(req == "/btn/B")
    {
      digitalWrite(INA, HIGH);
      ledcWrite(0, 155);
      digitalWrite(INB, HIGH);
      ledcWrite(1, 155);
    }
    else if(req == "/btn/L")
    {
      digitalWrite(INA, LOW);
      ledcWrite(0, 100);
      digitalWrite(INB, HIGH);
      ledcWrite(1, 155);
    }
    else if(req == "/btn/R")
    {
      digitalWrite(INA, HIGH);
      ledcWrite(0, 155);
      digitalWrite(INB, LOW);
      ledcWrite(1, 100);
    }
    else if(req == "/btn/S")
    {
      digitalWrite(INA, LOW);
      ledcWrite(0, 0);
      digitalWrite(INB, LOW);
      ledcWrite(1, 0);
    }
    else if(req == "/btn/p")
    {
      Serial.write('p');
      ledcWrite(channel_PWM, set_angle(55));
    }
    else if(req == "/btn/q")
    {
      Serial.write('q');
      client.println("q");
    }
    else if(req == "/btn/x")
    {
      ledcWrite(channel_PWM, set_angle(0));
    }

}

int set_angle(int angle)
{
  int servo_angle = map(angle, 0, 180, 25, 128);
  return servo_angle;
}
