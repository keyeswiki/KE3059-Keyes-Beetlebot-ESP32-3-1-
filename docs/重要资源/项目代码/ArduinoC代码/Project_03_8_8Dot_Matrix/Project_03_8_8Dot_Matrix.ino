//*************************************************************************************
/*
 Project 03 8*8 Dot Matrix
8*8点阵屏幕显示图案
*/
#include "HT16K33_Lib_For_ESP32.h"
#define SDA 21
#define SCL 22
ESP32_HT16K33 matrix = ESP32_HT16K33();
byte test1[8] = {0x70, 0x88, 0x84, 0x42, 0x42, 0x84, 0x88, 0x70};

void setup()
{
  matrix.init(0x70, SDA, SCL);//初始化点阵
  matrix.showLedMatrix(test1,0,0);
  matrix.show();
}

void loop()
{
  for (int i = 0; i <= 7; i++)
  {
    matrix.setBrightness(i);
    delay(100);
  }
  for (int i = 7; i > 0; i--)
  {
    matrix.setBrightness(i);
    delay(100);
  }
}
//*************************************************************************************
