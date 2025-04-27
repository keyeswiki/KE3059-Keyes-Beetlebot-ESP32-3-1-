#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel ledStrip(4, 14, NEO_GRB + NEO_KHZ800);
#include <ESP32Servo.h>

#include <HT16K33_Lib_For_ESP32.h>
//定义引脚为GPIO21和GPIO22
#define SDA 21
#define SCL 22
ESP32_HT16K33 matrix = ESP32_HT16K33();
//数组，用于存储模式数据，可以自己计算或从触摸工具获得
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

const int buzzerPin = 2; //蜂鸣器引脚
#define tracking_left  17  //左巡线传感器引脚 gpio17
#define tracking_right  16  //右巡线传感器引脚 gpio16
#define light_L_Pin  34   //左光敏电阻引脚 gpio34
#define light_R_Pin  35   //右光敏电阻引脚 gpio35
#define left_ctrl  33  //左电机的方向控制引脚 gpio33
#define left_pwm  26   //左电机的PWM控制引脚 gpio26.
#define right_ctrl  32 //右电机的方向控制引脚 gpio32.
#define right_pwm  25  //右电机的PWM控制引脚 gpio25

//替换为您的网络凭据（输入您的SSID和密码）
const char* ssid = "ChinaNet-2.4G-0DF0"; //此处输入SSID
const char* password = "ChinaNet@233"; //此处输入密码

// 端口80的TCP服务器将响应HTTP请求
WiFiServer server(80);

//超声波传感器
#define TRIG_PIN 5 // 定义超声波传感器的信号输入为 gpio5
#define ECHO_PIN 18 //定义超声波传感器的信号输入为 gpio18
long distance,a1,a2; //定义三个距离变量

//servo
const int servoPin = 4;//设置舵机引脚为gpio4
Servo myservo;  // 创建舵机对象来控制舵机

int flag_neo = 0;
int flag_matrix = 0;
int speed_L = 127;
int speed_R = 127;

bool carFollowKey = false;
bool carAvoidKey = false;
bool carLineTrackingKey = false;
bool carLightFindingKey = false;
bool stopKey = false;

char buzzerKey = 2;
char musicKey = 2;

char ledKey = 4;
char faceKey = 4;

bool upKey = false;
bool downKey = false;
bool leftKey = false;
bool rightKey = false;
bool isNewKey = false;

String req;
String M_req;
int len_val;
String s_M_req;


WiFiClient client;
void TaskWifi(void *pvParameters);

void setup() {
    Serial.begin(115200);
    
    pinMode(left_ctrl,OUTPUT); //将左电机方向控制引脚设为 OUTPUT
    ledcAttach(left_pwm, 1200, 8); //设置left_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。
    pinMode(right_ctrl,OUTPUT);//将右电机方向控制引脚设为 OUTPUT..
    ledcAttach(right_pwm, 1200, 8); //设置right_pwm引脚的频率为1200，PWM分辨率为8，占空比为256。

    pinMode(TRIG_PIN,OUTPUT);// 设置 TRIG_PIN 为 OUTPUT.
    pinMode(ECHO_PIN,INPUT);// 设置 ECHO_PIN 为 INPUT.
    pinMode(tracking_left, INPUT); //右传感器设置为 INPUT
    pinMode(tracking_right, INPUT); //左传感器设置为 INPUT
    pinMode(light_L_Pin, INPUT); //右传感器设置为 INPUT
    pinMode(light_R_Pin, INPUT); //左传感器设置为 INPUT

    myservo.setPeriodHertz(50);           // 标准50赫兹舵机
    myservo.attach(servoPin, 500, 2500);  // 将servoPin上的舵机附加到舵机对象上
    myservo.write(90);  // 设置舵机初始角度为 90° .
    delay(500);
    
    pinMode(buzzerPin,OUTPUT);
    //ledcAttach(buzzerPin, 2000, 8); // 设置PWM引脚
    ledcAttach(buzzerPin, 0, 0);
    ledcWriteTone(buzzerPin, 0);
    
    matrix.init(0x70, SDA, SCL);// 点阵初始化
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

  // 设置两个独立运行的任务
  uint32_t delayT = 100;  // LED引脚改变状态之间的延迟
  xTaskCreate(
    TaskWifi, "Task Wifi"  // 一个人类专属的名字
    ,
    2048  // 堆栈大小可以通过调用 `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,
    (void *)&delayT  // 任务参数，可以修改任务行为。这必须作为指向void的指针传递。
    ,
    2  // 优先级
    ,
    NULL  // 这里不使用任务处理 - 只需传递NULL
  );

}

void loop(){  
  if(ledKey == 1){
    Serial.println("RGB led mode--1");
    flag_neo = 0;
    while(flag_neo <= 7){
      flag_neo = flag_neo + 1;
      switch(flag_neo){
        case 1: ledStrip.fill(0xff0000, 0, 3);ledStrip.show(); break;
        case 2: ledStrip.fill(0xe6da59, 0, 3);ledStrip.show(); break;
        case 3: ledStrip.fill(0x4cff00, 0, 3);ledStrip.show(); break;
        case 4: ledStrip.fill(0x0077ff, 0, 3);ledStrip.show(); break;
        case 5: ledStrip.fill(0xff10df, 0, 3);ledStrip.show(); break;
        case 6: ledStrip.fill(0x75ffff, 0, 3);ledStrip.show(); break;
        case 7: ledStrip.fill(0xffffff, 0, 3);ledStrip.show(); break;
      }
      delay(500);
    }
  }else if(ledKey == 0){
    Serial.println("Turn off RGB led--1");
    ledStrip.clear();
    ledStrip.show();
  }else if(ledKey == 2){
    Serial.println("RGB led mode--2");
    flag_neo = 7;
    while(flag_neo > 0){
      switch(flag_neo){
        case 1: ledStrip.fill(0xff0000, 0, 3);ledStrip.show(); break;
        case 2: ledStrip.fill(0xe6da59, 0, 3);ledStrip.show(); break;
        case 3: ledStrip.fill(0x4cff00, 0, 3);ledStrip.show(); break;
        case 4: ledStrip.fill(0x0077ff, 0, 3);ledStrip.show(); break;
        case 5: ledStrip.fill(0xff10df, 0, 3);ledStrip.show(); break;
        case 6: ledStrip.fill(0x75ffff, 0, 3);ledStrip.show(); break;
        case 7: ledStrip.fill(0xffffff, 0, 3);ledStrip.show(); break;
      }
      delay(500);
      flag_neo = flag_neo - 1;
    }
  }else if(ledKey == 3){
    //Serial.println("Turn off RGB led--2");
    //ledKey = 4;
    //ledStrip.clear();
  }


  if(faceKey == 1){
    Serial.println("display face--1");
    flag_matrix = 0;
    while(flag_matrix <= 10){
      flag_matrix = flag_matrix + 1;
      switch(flag_matrix){
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
      delay(250);
    }
  }else if(faceKey == 2){
    Serial.println("turn off display");
    matrix.clear();
    matrix.show();
  }else if(faceKey == 3){
    Serial.println("display face--2");
    flag_matrix = 10;
    while(flag_matrix > 0){
      switch(flag_matrix){
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
      delay(250);
      flag_matrix = flag_matrix - 1;
    }
  }else if(faceKey == 0){
    faceKey = 4;
    //Serial.println("faceKey release");
  }
  

  if(buzzerKey == 1){
    ledcAttach(buzzerPin, 2000, 8);
    ledcWriteTone(buzzerPin, 262);
    delay(300);
    ledcAttach(buzzerPin, 0, 0);
    ledcWriteTone(buzzerPin, 0);
    Serial.println("turn on buzzer");
  }else if(buzzerKey == 0) {
    buzzerKey = 2;
    ledcAttach(buzzerPin, 0, 0);
    ledcWriteTone(buzzerPin, 0);
    Serial.println("turn off buzzer");
  }

  if(musicKey == 1){
    Serial.println("turn on music");
    ledcAttach(buzzerPin, 2000, 8);
    ledcWriteTone(buzzerPin, 262);
    delay(200);
    ledcWriteTone(buzzerPin, 294);
    delay(200);
    ledcWriteTone(buzzerPin, 330);
    delay(200);
    ledcWriteTone(buzzerPin, 349);
    delay(200);
    ledcWriteTone(buzzerPin, 392);
    delay(200);
    ledcWriteTone(buzzerPin, 440);
    delay(200);
    ledcWriteTone(buzzerPin, 494);
    delay(200);
    ledcAttach(buzzerPin, 0, 0);
    ledcWriteTone(buzzerPin, 0);
  }else if(musicKey == 0){
    musicKey = 2;
    Serial.println("turn off music");
    ledcAttach(buzzerPin, 0, 0);
    ledcWriteTone(buzzerPin, 0);
  }

  if(upKey){
    car_front();
    Serial.println("car run front");
  }else if(downKey){
    car_back();
    Serial.println("car turn back");
  }else if(leftKey){
    car_left();
    Serial.println("car turn left");
  }else if(rightKey){
    car_right();
    Serial.println("car turn right");
  }
  if(stopKey){
    upKey = false;
    downKey = false;
    leftKey = false;
    rightKey = false;
    stopKey = false;
    car_Stop();
    Serial.println("car stop");
  }

  if(carLineTrackingKey){
    car_tracking();
  }else if(carAvoidKey){
    car_avoid();
  }else if(carLightFindingKey){
    car_lighting();
  }else if(carFollowKey){
    car_follow();
  }

  //Serial.println("In loop function");
  delay(100);
}

void TaskWifi(void *pvParameters) {  // 一个任务
  uint32_t delayT = *((uint32_t *)pvParameters);

  //设置mDNS响应器
  // - 本例中第一个参数为域名
  //   完整域名是“esp32.local”
  // - 第二个参数是要发布的IP地址
  //   我们通过WiFi网络发送我们的IP地址
  if (!MDNS.begin("esp32")) {
      Serial.println("Error setting up MDNS responder!");
      while(1) {
          delay(1000);
      }
  }
  Serial.println("mDNS responder started");

  // 启动TCP （HTTP）服务器
  server.begin();
  Serial.println("TCP server started");

  // 向mmds - sd添加业务
  MDNS.addService("http", "tcp", 80);

  for (;;) {                          // 任务不会返回或退出
	  // 检查客户端是否连接
    client = server.available();
  
    // 等待来自客户端的数据变为可用
    while(client.connected() && !client.available()){
      delay(10);
    }

    client.setTimeout(50); // 设置超时时间为 50 毫秒
    // 读取HTTP请求的第一行
    req = client.readStringUntil('\r');
    //Serial.println(req);
    // HTTP请求的第一行看起来像“GET /path HTTP/1.1”
    // 通过查找空格来检索“/path”部分
    int addr_start = req.indexOf(' ');
    int addr_end = req.indexOf(' ', addr_start + 1);
    if (addr_start == -1 || addr_end == -1) {
        //Serial.print("Invalid request: ");
        //Serial.println(req);
        //return;
    }
    req = req.substring(addr_start + 1, addr_end);
    //Serial.print("Request: ");
    //Serial.println(req);
    int len_val = String(req).length();
    M_req = String(req).substring(0,6);
    if(M_req != ""){
      Serial.println(M_req);
    }
    
    if(req == "/btn/c"){
      musicKey = 1;
    }else if(req == "/btn/d"){
      musicKey = 0;
    }

    if(req == "/btn/a"){
      buzzerKey = 1;
    }else if(req == "/btn/b"){
      buzzerKey = 0;
    }

    if(M_req == "/btn/u"){
      String s_M_req = String(req).substring(6,len_val);
      speed_L = s_M_req.toInt();
      Serial.print("Change left motot speed: ");
      Serial.println(speed_L);
    }
    if(M_req == "/btn/v"){
      String s_M_req = String(req).substring(6,len_val);
      speed_R = s_M_req.toInt();
      Serial.print("Change right motot speed: ");
      Serial.println(speed_R);
    }


    if(M_req == "/btn/F"){
      upKey = true;
    }else if(M_req == "/btn/B"){
      downKey = true;
    }else if(M_req == "/btn/L"){
      leftKey = true;
    }else if(M_req == "/btn/R"){
      rightKey = true;
    }


    if(req == "/btn/e"){
      ledKey = 1;
    }else if(req == "/btn/g"){
      ledKey = 2;
    }else if(req == "/btn/f"){
      ledKey = 0;
    }else if(req == "/btn/z"){
      ledKey = 3;
    }


    if(req == "/btn/i"){
      faceKey = 1;
    }else if(req == "/btn/j"){
      faceKey = 2;
    }else if(req == "/btn/k"){
      faceKey = 3;
    }else if(req == "/btn/y"){
      faceKey = 0;
    }


    if(M_req == "/btn/l"){
      carLineTrackingKey = true;
    }else if(M_req == "/btn/m"){
      carAvoidKey = true;
    }else if(M_req == "/btn/n"){
      carLightFindingKey = true;
    }else if(M_req == "/btn/o"){
      carFollowKey = true;
    }else if(M_req == "/btn/S"){
      stopKey = true;
    }

    //client.print(s);
    //client.stop();
    //Serial.println("Done with client");
    delay(20);
  }
}

void car_tracking(){
  while (1){
    stopKey = false;
    Serial.println("car_traking: ");

    myservo.write(90);  // 设置舵机初始角度为 90° .
    delay(300);
    int L_val = digitalRead(tracking_left);//读取左边巡线传感器的值
    int R_val = digitalRead(tracking_right);//读取右边巡线传感器的值
    if((L_val == 1)&&(R_val == 1))//如果两个传感器都检测到黑线
    {
       ledcAttach(left_pwm, 1200, 8);
       ledcAttach(right_pwm, 1200, 8);
       digitalWrite(left_ctrl,LOW); //将左电机方向控制引脚设为 LOW.
       ledcWrite(left_pwm, 120); //左电机输出 PWM 120
       digitalWrite(right_ctrl,LOW); //将右电机方向控制引脚设为 LOW.
       ledcWrite(right_pwm, 120); //右电机输出 PWM 120
    }
    else if((L_val == 1)&&(R_val == 0))//如果只有左侧传感器检测到黑线
    {
       ledcAttach(left_pwm, 1200, 8);
       ledcAttach(right_pwm, 1200, 8);
       digitalWrite(left_ctrl,HIGH); //将左电机方向控制引脚设为 HIGH..
       ledcWrite(left_pwm, 100); //左电机输出 PWM 100
       digitalWrite(right_ctrl,LOW); //将右电机方向控制引脚设为 LOW.
       ledcWrite(right_pwm, 150); //右电机输出 PWM 150
    }
    else if((L_val == 0)&&(R_val == 1))//如果只有右侧传感器检测到黑线
    {
       ledcAttach(left_pwm, 1200, 8);
       ledcAttach(right_pwm, 1200, 8);
       digitalWrite(left_ctrl,LOW); //将左电机方向控制引脚设为 LOW.
       ledcWrite(left_pwm, 150); //左电机输出 PWM 150
       digitalWrite(right_ctrl,HIGH); //将右电机方向控制引脚设为 HIGH..
       ledcWrite(right_pwm, 100); //右电机输出 PWM 100
    }
    else//如果没有一个传感器检测到黑线
    {
      car_Stop();//停止
    }

		//delay(1000);
    if(stopKey){
      stopKey = false;
      carLineTrackingKey = false;
      car_Stop();//停止
      return;
    }
  }
}

void car_avoid(){
  Serial.println("car_avoid: ");
  myservo.attach(servoPin, 500, 2500);
  myservo.write(90);  // 设置舵机初始角度为 90° .
  while (1){
    stopKey = false;
    Serial.println("car_avoid: ");
    delay(300);
    distance = checkdistance(); //得到超声波距离的值
    if((distance < 10)&&(distance != 0))// 0<距离值<10
    {
      car_Stop();//停止
      delay(100);
      myservo.write(175);  //舵机转到 175°. 
      delay(500);
      a1=checkdistance();//测距
      delay(100);
      myservo.write(5); //舵机转到 5°. 
      delay(500);
      a2=checkdistance();//测距
      delay(100);

      if(a1 > a2)//
      {
        car_left();//左转
        delay(100);
        myservo.write(90); //设置舵机初始角度为 90° .
        delay(500);
      }
      else//左边距离 < 右边距离
      {
        car_right();//右转
        delay(100);
        myservo.write(90); //设置舵机初始角度为 90° .
        delay(500);
      }
    }
    else//
    {
      car_front();//前进  
    }

		//delay(1000);
    if(stopKey){
      stopKey = false;
      carAvoidKey = false;
      car_Stop();//停止
      return;
    }
  }
}

void car_lighting(){
  myservo.attach(servoPin, 500, 2500);
  myservo.write(90);  // 设置舵机初始角度为 90° .
  while (1){
    stopKey = false;
    Serial.println("car_lighting: ");

    delay(300); 
    int left_light = analogRead(light_L_Pin);//读取左边光敏电阻的值
    int right_light = analogRead(light_R_Pin);//读取右边光敏电阻的值
    if (left_light > 3000 && right_light > 3000) //检测左、右光敏电阻范围
    {  
      car_front(); //前进
    } 
    else if (left_light > 3000 && right_light <= 3000)  //检测左、右光敏电阻范围
    {
      car_left(); //左转
    } 
    else if (left_light <= 3000 && right_light > 3000) //检测左、右光敏电阻范围
    {
      car_right(); //右转
    } 
    else  //
    {
      car_Stop(); //停止
    }

		delay(1000);
    if(stopKey){
      stopKey = false;
      carLightFindingKey = false;
      car_Stop();//停止
      return;
    }
  }
}

void car_follow(){
  myservo.attach(servoPin, 500, 2500);
  myservo.write(90);  // 设置舵机初始角度为 90° .
  while (1){
    stopKey = false;
    Serial.println("car_follow: ");
    long distance = checkdistance();//得到超声波传感器测得的距离
    if(distance < 8)//如果距离值小于 8
    {
      car_back();//后退
    }
    else if((distance >= 8)&&(distance < 13))//if 8≤distance<13
    {
      car_Stop();//停止
    }
    else if((distance >= 13)&&(distance < 35))//if 13≤distance<35.
    {
      car_front();//跟随
    }
    else//
    {
      car_Stop();//停止
    }

		//delay(1000);
    if(stopKey){
      stopKey = false;
      carFollowKey = false;
      car_Stop();//停止
      return;
    }
  }
}

float checkdistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float distance = pulseIn(ECHO_PIN, HIGH) / 58.00;
  delay(10);
  return distance;
}

void car_front() //定义状态：前进
{
  ledcAttach(left_pwm, 1200, 8);
  ledcAttach(right_pwm, 1200, 8);
  digitalWrite(left_ctrl,LOW); //将左电机方向控制引脚设为 LOW.
  ledcWrite(left_pwm, speed_L); //左电机输出 PWM 100
  digitalWrite(right_ctrl,LOW); //将右电机方向控制引脚设为 LOW.
  ledcWrite(right_pwm, speed_R); //右电机输出 PWM 100
}
void car_back()//定义状态：后退
{
  ledcAttach(left_pwm, 1200, 8);
  ledcAttach(right_pwm, 1200, 8);
  digitalWrite(left_ctrl,HIGH); //将左电机方向控制引脚设为 HIGH.
  ledcWrite(left_pwm, 255-speed_L); //左电机输出 PWM 100
  digitalWrite(right_ctrl,HIGH); //将右电机方向控制引脚设为 HIGH.
  ledcWrite(right_pwm, 255-speed_R); //右电机输出 PWM 100
}
void car_left()//定义状态：左转
{
  ledcAttach(left_pwm, 1200, 8);
  ledcAttach(right_pwm, 1200, 8);
  digitalWrite(left_ctrl,HIGH); //将左电机方向控制引脚设为 HIGH..
  ledcWrite(left_pwm, 255-speed_L); //左电机输出 PWM 100
  digitalWrite(right_ctrl,LOW); //将右电机方向控制引脚设为 LOW.
  ledcWrite(right_pwm, speed_R); //右电机输出 PWM 100
}
void car_right()//定义状态：右转
{
  ledcAttach(left_pwm, 1200, 8);
  ledcAttach(right_pwm, 1200, 8);
  digitalWrite(left_ctrl,LOW); //将左电机方向控制引脚设为 LOW.
  ledcWrite(left_pwm, speed_L); //左电机输出 PWM 100
  digitalWrite(right_ctrl,HIGH); //将右电机方向控制引脚设为 HIGH..
  ledcWrite(right_pwm, 255-speed_R); //右电机输出 PWM 100
}
void car_Stop()//定义状态：停止
{
  ledcAttach(left_pwm, 1200, 8);
  ledcAttach(right_pwm, 1200, 8);
  digitalWrite(left_ctrl,LOW);//将左电机方向控制引脚设为 LOW.
  ledcWrite(left_pwm, 0); //左电机输出 PWM 0 
  digitalWrite(right_ctrl,LOW);//将右电机方向控制引脚设为 LOW.
  ledcWrite(right_pwm, 0); //右电机输出 PWM 0
}

