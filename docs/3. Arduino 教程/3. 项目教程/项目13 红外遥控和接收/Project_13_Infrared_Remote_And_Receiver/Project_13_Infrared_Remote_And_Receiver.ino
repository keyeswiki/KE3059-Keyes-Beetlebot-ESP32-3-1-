//**********************************************************************************
/*
项目13:红外遥控及红外接收
*/
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

const uint16_t recvPin = 19;  // 红外接收引脚
IRrecv irrecv(recvPin);  // 创建用于接收类的类对象
decode_results results;   // 创建解码结果类对象

void setup()
{
  Serial.begin(115200); // 初始化串口，波特率设置为115200
  irrecv.enableIRIn(); // 开始接收信号
}

void loop() {
  if (irrecv.decode(&results)) {
    serialPrintUint64(results.value, HEX);
    Serial.println("");
    irrecv.resume(); //释放IRremote并接收下一个值.
  }
  delay(100);
}
//**********************************************************************************
