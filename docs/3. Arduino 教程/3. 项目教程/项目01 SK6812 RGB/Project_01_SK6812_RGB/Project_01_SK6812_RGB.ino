//*************************************************************************************
/*
  项目 01 SK6812 RGB
  4个rgb用于各种灯光效果
*/
#include <Adafruit_NeoPixel.h>

#define PIN 14

// 参数1 =条带像素数
// 参数2 = Arduino引脚号(大部分有效)
// 参数3 =像素类型标志，根据需要添加在一起:
//   NEO_KHZ800  800 KHz比特流(大多数NeoPixel产品带WS2812 led)
//   NEO_KHZ400  400 KHz(经典的“v1”(不是v2) FLORA像素，WS2811驱动器)
//   NEO_GRB     像素连接到GRB位流(大多数NeoPixel产品)
//   NEO_RGB     像素连接到RGB位流(v1 FLORA像素，而不是v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// 重要提示:为了降低NeoPixel烧毁风险，在像素电源引线上添加1000 uF电容，在第一个像素的数据输入上添加
// 300 - 500欧姆电阻，并最小化Arduino与第一个像素之间的距离。避免在带电电路上连接。如果必须，请先连接GND。

void setup() {
  strip.begin();
  strip.show(); // 将所有像素初始化为“off”
}

void loop() {
  // 一些示例程序显示如何显示到像素:
  colorWipe(strip.Color(255, 0, 0), 50); // 红
  colorWipe(strip.Color(0, 255, 0), 50); // 绿
  colorWipe(strip.Color(0, 0, 255), 50); // 蓝
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // 白
  theaterChase(strip.Color(127,   0,   0), 50); // 红
  theaterChase(strip.Color(  0,   0, 127), 50); // 蓝

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// 用颜色一个接一个地填充圆点
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

// 稍有不同的是，这使得彩虹均匀地分布在各处
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5轮所有颜色
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//剧院风格的爬行灯.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //做10个循环的追逐
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //打开每三个像素
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //关闭每3个像素
      }
    }
  }
}

//带有彩虹效果的剧院式爬行灯
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // 来回循环所有256种颜色
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //打开每三个像素
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //关闭每3个像素
        }
    }
  }
}

// 输入一个0到255的值来获得颜色值.
// 这些颜色是r - g - b -回到r.
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
