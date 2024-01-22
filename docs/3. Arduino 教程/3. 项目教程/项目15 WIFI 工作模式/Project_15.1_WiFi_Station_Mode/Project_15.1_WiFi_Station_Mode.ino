//**********************************************************************************
/*
  项目 15.1: WiFi站模式
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
