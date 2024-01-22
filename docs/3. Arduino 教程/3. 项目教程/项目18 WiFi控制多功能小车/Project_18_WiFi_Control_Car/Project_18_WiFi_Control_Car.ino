//*************************************************************************************
/*
项目18: WiFi控制多功能小车
*/

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel ledStrip(4, 14, NEO_GRB + NEO_KHZ800);
#include <ESP32Tone.h>
#include <musicESP32.h>   
music Music(2);

#include <HT16K33_Lib_For_ESP32.h>
//定义点阵的引脚在GPIO21,GPIO22
#define SDA 21
#define SCL 22
ESP32_HT16K33 matrix = ESP32_HT16K33();
//数组，用于储存图案的数据，可以自己算也可以从取摸工具中得到
uint8_t matrix_smile[8]={0x60, 0x80, 0x64, 0x02, 0x02, 0x64, 0x80, 0x60};
uint8_t matrix_front[8]={0x12, 0x24, 0x48, 0x90, 0x90, 0x48, 0x24, 0x12};
uint8_t matrix_back[8]={0x48, 0x24, 0x12, 0x09, 0x09, 0x12, 0x24, 0x48};
uint8_t matrix_left[8]={0x18, 0x24, 0x42, 0x99, 0x24, 0x42, 0x81, 0x00};
uint8_t matrix_right[8]={0x00, 0x81, 0x42, 0x24, 0x99, 0x42, 0x24, 0x18};
uint8_t matrix_stop[8]={0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00};
uint8_t matrix_tsundere[8]={0x40, 0x40, 0x48, 0x10, 0x48, 0x44, 0x40, 0x00};
uint8_t matrix_squinting[8]={0x44, 0x28, 0x12, 0x02, 0x02, 0x12, 0x28, 0x44};
uint8_t matrix_despise1[8]={0x30, 0x10, 0x14, 0x04, 0x34, 0x10, 0x10, 0x00};
uint8_t matrix_speechless[8]={0x40, 0x40, 0x5c, 0x14, 0x5c, 0x40, 0x40, 0x40};
uint8_t matrix_heart[8]={0x30, 0x48, 0x44, 0x22, 0x22, 0x44, 0x48, 0x30};
uint8_t matrix_clear[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


#define INA 32
#define PWMA 25
#define INB 33
#define PWMB 26

const char* ssid = "ChinaNet-2.4G-0DF0";
const char* password = "ChinaNet@233";

// TCP server at port 80 will respond to HTTP requests
WiFiServer server(80);

//超声波传感器
#define TRIG_PIN 5 // 定义超声波的信号输入在gpio5.
#define ECHO_PIN 18 //定义超声波的信号输出在gpio18.
long distance; //定义距离变量

//舵机
int channel_PWM = 3; // 舵机通道
// 舵机频率，那么周期也就是1/50，也就是20ms ，PWM一共有16个通道，0-7位高速通道由80Mhz时钟驱动，后面8个为低速通道由1Mhz时钟驱动.
int freq_PWM = 50;
// PWM分辨率，取值为 0-20 之间  ，这里填写为10，那么后面的ledcWrite 这个里面填写的pwm值就在0-2的10次方之间，也就是0-1024，如果是要求不高的东西你可以直接拿1000去算了.
int resolution_PWM = 10;
// 绑定的IO，在下面的绑定函数里面会用到，绑定之后这个IO就会变成我们PWM的输出口.
const int servopin = 4;//定义舵机的IO脚位在gpio4.

int flag_neo = 0;
int flag_matrix = 0;
int speed_L = 200;
int speed_R = 200;

void setup(void)
{
    Serial.begin(115200);
    pinMode(INA, OUTPUT);
    ledcAttachPin(PWMA, 0);
    ledcSetup(0, 50, 8);//设置LEDC通道1频率为1200，PWM分辨率为8，即占空比256.
    pinMode(INB, OUTPUT);
    ledcAttachPin(PWMB,1);
    ledcSetup(1, 50, 8);//设置LEDC通道为2频率为1200，PWM分辨率为8，即占空比256.

    pinMode(TRIG_PIN,OUTPUT);//设置TRIG_PIN为输出.
    pinMode(ECHO_PIN,INPUT);//设置ECHO_PIN为输入.
    
    ledcSetup(3, 50, 10); // 设置舵机通道3频率为50,PWM分辨率为10.
    ledcAttachPin(4, 3);  //将 LEDC 通道绑定到指定 IO 口上以实现输出.
    ledcWrite(channel_PWM, 77);  //20ms高电平为1.5ms左右 ，也就是1.5/20*1024，设置舵机初始角度为90°.
    delay(300);

    pinMode(2, OUTPUT);

    matrix.init(0x70, SDA, SCL);// 矩阵初始化
    matrix.clear(); //清屏
    matrix.setBrightness(10);

    ledStrip.begin();

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
    // - 在本例中，第一个参数是域名
    //   完全限定域名为esp32.local
    // - 第二个参数是要发布的IP地址
    //   我们通过WiFi网络发送我们的IP地址
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
    int len_val = String(req).length();
    String M_req = String(req).substring(0,6);
    //Serial.println(M_req);
    if(M_req == "/btn/u")
    {
      String s_M_req = String(req).substring(6,len_val);
      speed_L = s_M_req.toInt();
      Serial.println(speed_L);
    }
    if(M_req == "/btn/v")
    {
      String s_M_req = String(req).substring(6,len_val);
      speed_R = s_M_req.toInt();
      Serial.println(speed_R);
    }

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
      ledcWrite(0, speed_L);
      digitalWrite(INB, LOW);
      ledcWrite(1, speed_R);
    }
    else if(req == "/btn/B")
    {
      digitalWrite(INA, HIGH);
      ledcWrite(0, (255-speed_L));
      digitalWrite(INB, HIGH);
      ledcWrite(1, (255-speed_R));
    }
    else if(req == "/btn/L")
    {
      digitalWrite(INA, LOW);
      ledcWrite(0, speed_L);
      digitalWrite(INB, HIGH);
      ledcWrite(1, (255-speed_R));
    }
    else if(req == "/btn/R")
    {
      digitalWrite(INA, HIGH);
      ledcWrite(0, (255-speed_L));
      digitalWrite(INB, LOW);
      ledcWrite(1, speed_R);
    }
    else if(req == "/btn/S")
    {
      digitalWrite(INA, LOW);
      ledcWrite(0, 0);
      digitalWrite(INB, LOW);
      ledcWrite(1, 0);
    }
    else if(req == "/btn/e")
    {
      flag_neo = flag_neo + 1;
      if(flag_neo >= 7)
      {
        flag_neo = 7;
      }
      Serial.println(flag_neo);
      switch(flag_neo)
      {
        case 1: ledStrip.fill(0xff0000, 0, 3);ledStrip.show(); break;
        case 2: ledStrip.fill(0xe6da59, 0, 3);ledStrip.show(); break;
        case 3: ledStrip.fill(0x4cff00, 0, 3);ledStrip.show(); break;
        case 4: ledStrip.fill(0x0077ff, 0, 3);ledStrip.show(); break;
        case 5: ledStrip.fill(0xff10df, 0, 3);ledStrip.show(); break;
        case 6: ledStrip.fill(0x75ffff, 0, 3);ledStrip.show(); break;
        case 7: ledStrip.fill(0xffffff, 0, 3);ledStrip.show(); break;
      }
    }
    else if(req == "/btn/f")
    {
      ledStrip.clear();
      ledStrip.show();
    }
    else if(req == "/btn/g")
    {
      flag_neo = flag_neo - 1;
      Serial.println(flag_neo);
      if(flag_neo <= 1)
      {
        flag_neo = 1;
      }
      switch(flag_neo)
      {
        case 1: ledStrip.fill(0xff0000, 0, 3);ledStrip.show(); break;
        case 2: ledStrip.fill(0xe6da59, 0, 3);ledStrip.show(); break;
        case 3: ledStrip.fill(0x4cff00, 0, 3);ledStrip.show(); break;
        case 4: ledStrip.fill(0x0077ff, 0, 3);ledStrip.show(); break;
        case 5: ledStrip.fill(0xff10df, 0, 3);ledStrip.show(); break;
        case 6: ledStrip.fill(0x75ffff, 0, 3);ledStrip.show(); break;
        case 7: ledStrip.fill(0xffffff, 0, 3);ledStrip.show(); break;
      }
    }
    else if(req == "/btn/z")
    {
      //ledStrip.clear();
    }
//    else if(req == "/btn/h")
//    {
//      Serial.write('z');
//      client.println(F("h"));
//    }
    else if(req == "/btn/i")
    {
      flag_matrix = flag_matrix + 1;
      if(flag_matrix >= 10)
      {
        flag_matrix = 10;
      }
      Serial.println(flag_matrix);
      switch(flag_matrix)
      {
        case 1: matrix.showLedMatrix(matrix_smile,0,0);matrix.show(); break;
        case 2: matrix.showLedMatrix(matrix_tsundere,0,0);matrix.show(); break;
        case 3: matrix.showLedMatrix(matrix_squinting,0,0);matrix.show(); break;
        case 4: matrix.showLedMatrix(matrix_despise1,0,0);matrix.show(); break;
        case 5: matrix.showLedMatrix(matrix_speechless,0,0);matrix.show(); break;
        case 6: matrix.showLedMatrix(matrix_heart,0,0);matrix.show(); break;
        case 7: matrix.showLedMatrix(matrix_front,0,0);matrix.show(); break;
        case 8: matrix.showLedMatrix(matrix_back,0,0);matrix.show(); break;
        case 9: matrix.showLedMatrix(matrix_left,0,0);matrix.show(); break;
        case 10: matrix.showLedMatrix(matrix_right,0,0);matrix.show(); break;
      }
    }
    else if(req == "/btn/j")
    {
      matrix.clear();
      matrix.show();//点阵显示向前图案 
    }
    else if(req == "/btn/k")
    {
      flag_matrix = flag_matrix - 1;
      if(flag_matrix <= 1)
      {
        flag_matrix = 1;
      }
      Serial.println(flag_matrix);
      switch(flag_matrix)
      {
        case 1: matrix.showLedMatrix(matrix_smile,0,0);matrix.show(); break;
        case 2: matrix.showLedMatrix(matrix_tsundere,0,0);matrix.show(); break;
        case 3: matrix.showLedMatrix(matrix_squinting,0,0);matrix.show(); break;
        case 4: matrix.showLedMatrix(matrix_despise1,0,0);matrix.show(); break;
        case 5: matrix.showLedMatrix(matrix_speechless,0,0);matrix.show(); break;
        case 6: matrix.showLedMatrix(matrix_heart,0,0);matrix.show(); break;
        case 7: matrix.showLedMatrix(matrix_front,0,0);matrix.show(); break;
        case 8: matrix.showLedMatrix(matrix_back,0,0);matrix.show(); break;
        case 9: matrix.showLedMatrix(matrix_left,0,0);matrix.show(); break;
        case 10: matrix.showLedMatrix(matrix_right,0,0);matrix.show(); break;
      }
    }
    else
    {
        s = "HTTP/1.1 404 Not Found\r\n\r\n";
        Serial.println("Sending 404");
    }
    //client.print(s);

    //client.stop();
    //Serial.println("Done with client");

}
//*************************************************************************************