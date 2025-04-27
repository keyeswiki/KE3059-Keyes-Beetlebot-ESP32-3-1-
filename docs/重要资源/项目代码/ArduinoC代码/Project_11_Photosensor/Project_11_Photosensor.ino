//*************************************************************************************
/*
Project 11:Photosensor
*/
#define PHOTOSENSITIVE_PIN 34 //定义ESP32读取的引脚光敏
int photosensitiveADC;        //定义一个变量来存储ADC值

void setup()
{
  pinMode(PHOTOSENSITIVE_PIN, INPUT);//配置输入模式的引脚
  Serial.begin(115200);          //波特率 115200
}

void loop()
{
  photosensitiveADC = analogRead(PHOTOSENSITIVE_PIN);//读取光敏电阻值
  Serial.print("photosensitiveADC: ");
  Serial.println(photosensitiveADC);         //打印光敏电阻值
  delay(500);
}
//*************************************************************************************
