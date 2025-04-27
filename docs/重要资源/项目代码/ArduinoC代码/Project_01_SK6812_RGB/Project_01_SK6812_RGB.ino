//*************************************************************************************
/*
  Project 01 SK6812 RGB
  4个rgb用于各种照明效果。
*/
#include <Adafruit_NeoPixel.h>

#define PIN 14

//参数1 = strip中像素的个数
//参数2 = Arduino引脚编号（大多数是有效的）
//参数3 =像素类型标志，根据需要加在一起：
//   NEO_KHZ800  800 KHz 比特流（大多数带有WS2812 led的NeoPixel产品）
//   NEO_KHZ400  400 KHz （经典的“v1”（不是v2） FLORA像素，WS2811驱动）
//   NEO_GRB     像素是为GRB比特流连接的（大多数NeoPixel产品）
//   NEO_RGB     像素为RGB比特流连接（v1 FLORA像素，而不是v2）
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

//重要：为了减少NeoPixel耗尽风险，在像素电源引线上添加1000 uF电容器，在第一个像素的数据输入上添加300 - 500欧姆电阻，并尽量减少Arduino与第一个像素之间的距离。
//避免在带电电路上连接…如果必须，请先连接GND。

void setup() {
  strip.begin();
  strip.show(); // 将所有像素初始化为“off”
}

void loop() {
  // 一些演示如何显示像素的示例程序：
  colorWipe(strip.Color(255, 0, 0), 50); // 红
  colorWipe(strip.Color(0, 255, 0), 50); // 绿
  colorWipe(strip.Color(0, 0, 255), 50); // 蓝
  // 发送一个影院像素追逐…
  theaterChase(strip.Color(127, 127, 127), 50); // 白
  theaterChase(strip.Color(127,   0,   0), 50); // 红
  theaterChase(strip.Color(  0,   0, 127), 50); // 蓝

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// 用一种颜色一个接一个地填充这些点
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// 略有不同，这使得彩虹均匀分布在各处
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 车轮上所有颜色的5个循环
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//剧院式的爬行灯。
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //做10个循环的追逐
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //打开每三个像素
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //关闭每三个像素
      }
    }
  }
}

//带有彩虹效果的剧院式爬行灯
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // 在轮盘上循环所有256种颜色
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //打开每三个像素
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //关闭每三个像素
        }
    }
  }
}

// 输入0到255的值来获取颜色值。
// 颜色是一个过渡r - g - b -回到r。
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
//*************************************************************************************
