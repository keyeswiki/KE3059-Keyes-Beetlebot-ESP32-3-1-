//**********************************************************************************
/*
Project 13 IR Remote Control
*/
#include <IRremote.hpp>

#define IR_RECEIVE_PIN   19   // 定义红外接收器模块引脚
IRrecv irrecv(IR_RECEIVE_PIN);     // 创建一个接收类的类对象
decode_results results;     // 创建一个解码的结果类对象

void setup() {
  Serial.begin(115200);       // 初始化串口，波特率设置为115200
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);  // 开始接收
}

void loop() {
  if (IrReceiver.decode()) {
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);  //打印原始数据
      //IrReceiver.printIRResultShort(&Serial); // 在一行中打印接收到的完整数据
      //IrReceiver.printIRSendUsage(&Serial);  // 打印发送此数据所需的语句
      IrReceiver.resume(); // 启用接收下一个值
  }
  delay(100);
}
//**********************************************************************************
