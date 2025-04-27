#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <HT16K33_Lib_For_ESP32.h>

String item = "0";
/*替换为您的网络凭据（输入您的SSID和密码）*/
const char* ssid = "ChinaNet-2.4G-0DF0"; //在这里输入SSID
const char* password = "ChinaNet@233";  //此处输入密码
WiFiServer server(80);

//点阵引脚 GPIO21,GPIO22
#define SDA 21
#define SCL 22
ESP32_HT16K33 matrix = ESP32_HT16K33();
//数组，用于存储数据的模式，可以计算自己或从触摸工具检索
byte front[8]={0x12,0x24,0x48,0x90,0x90,0x48,0x24,0x12};
byte back[8]={0x48,0x24,0x12,0x09,0x09,0x12,0x24,0x48};
byte left[8]={0x18,0x24,0x42,0x99,0x24,0x42,0x81,0x00};
byte right[8]={0x00,0x81,0x42,0x24,0x99,0x42,0x24,0x18};
byte stop1[8]={0x00,0x00,0x00,0xfd,0xfd,0x00,0x00,0x00};

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("TCP server started");
  MDNS.addService("http", "tcp", 80);

  matrix.init(0x70, SDA, SCL);// 矩阵初始化
  matrix.clear(); //清除
  matrix.setBrightness(10);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
      return;
  }
  while(client.connected() && !client.available()){
      delay(1);
  }
  String req = client.readStringUntil('\r');
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
      Serial.print("Invalid request: ");
      Serial.println(req);
      return;
  }
  req = req.substring(addr_start + 1, addr_end);
  item=req;
  Serial.println(item);
  String s;
  if (req == "/")
  {
      IPAddress ip = WiFi.localIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP32 at ";
      s += ipStr;
      s += "</html>\r\n\r\n";
      Serial.println("Sending 200");
      client.println(s);
  }
  else if(req == "/btn/i")
  {
    Serial.write('i');
    client.println(F("i"));
    matrix.showLedMatrix(front,0,0);//显示前进
    matrix.show();
  }
  else if(req == "/btn/j")
  {
    Serial.write('j');
    client.println(F("j"));
    matrix.showLedMatrix(stop1,0,0);
    matrix.show();
  }
  else if(req == "/btn/k")
  {
    Serial.write('k');
    client.println(F("k"));
    matrix.showLedMatrix(back,0,0);
    matrix.show();
  }
  //client.print(s);
  client.stop();
}
