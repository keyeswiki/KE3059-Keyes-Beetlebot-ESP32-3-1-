/*
Project 02 Buzzer
蜂鸣器播放多种音调
*/
const int buzzerPin = 2; //蜂鸣器引脚
const int resolution = 8; 

// 7个音符的频率（Hz）
int frequencies[] = {262, 294, 330, 349, 392, 440, 494};


// 在蜂鸣器上播放给定持续时间的频率
void playFrequency(int frequency, int duration) {
  ledcWriteTone(buzzerPin, frequency); // 开始播放
  delay(duration); // 等待指定的持续时间
  ledcWriteTone(buzzerPin, 0); // 停止蜂鸣器
}

void setup() {
  ledcAttach(buzzerPin, 2000, 8); // 设置PWM引脚
}

void loop() {
  for (int i = 0; i < 7; i++) {
    playFrequency(frequencies[i], 300); // 每个音符播放300毫秒
    delay(50); // 在音符之间加一个短暂的停顿
  }
  delay(1000); // 延时1秒
}
