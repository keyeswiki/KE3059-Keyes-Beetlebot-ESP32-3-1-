//**********************************************************************************
/*
项目 06 超声波测距
超声波探测物体距离
*/
#define trigPin 5 // 定义trigPin为gpio5.
#define echoPin 18 // 定义 echoPin为gpio18.
#define MAX_DISTANCE 700 // 最大传感器距离额定为400-500cm。
//timeOut= 2*MAX_DISTANCE /100 /340 *1000000 = MAX_DISTANCE*58.8
float timeOut = MAX_DISTANCE * 60; 
int soundVelocity = 340; // 定义声速=340m/s
void setup() {
 pinMode(trigPin,OUTPUT);// 设置trigPin为输出模式
 pinMode(echoPin,INPUT); // 设置echoPin为输入模式
 Serial.begin(115200); // 打开串口监视器，设置波特率为115200.
}
void loop() {
 delay(100); // 延时 100ms . 
 Serial.printf("Distance: ");
 Serial.print(getSonar()); // 以厘米为单位获取距离并打印结果
 Serial.println("cm");
}
float getSonar() {
 unsigned long pingTime;
 float distance;
 // 使trigPin输出高电平持续10us触发HC_SR04
 digitalWrite(trigPin, HIGH); 
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 // 等待HC-SR04返回高电平，并测量此等待时间
 pingTime = pulseIn(echoPin, HIGH, timeOut); 
 // 根据时间计算距离
 distance = (float)pingTime * soundVelocity / 2 / 10000; 
 return distance; // 返回距离值
}
//**********************************************************************************
