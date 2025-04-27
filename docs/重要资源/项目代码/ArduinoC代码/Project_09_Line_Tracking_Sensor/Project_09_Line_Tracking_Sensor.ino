//*************************************************************************************
/*
Project 09: Line Tracking Sensor
*/ 
#define tracking_left  17  //定义左边传感器的引脚为 gpio17
#define tracking_right  16  //定义右侧传感器的引脚为 gpio16
int L_val,R_val; //定义两个传感器的两个变量

void setup() {
 Serial.begin(115200); //波特率设置为 115200
 pinMode(tracking_left, INPUT); //将左侧传感器的引脚设置为 INPUT
 pinMode(tracking_right, INPUT); //将右侧传感器的引脚设置为 INPUT
}

void loop() {
 L_val = digitalRead(tracking_left); //读取左边的值
 R_val = digitalRead(tracking_right); //读取右边的值
 Serial.print("L_val: "); //串口打印 L_val
 Serial.print(L_val); //串口打印s L_val
 Serial.print("    "); //串口打印s space key
 Serial.print("R_val: "); //串口打印s R_val
 Serial.println(R_val); //串口打印s the R_val
 delay(300); //延时 0.3s
}
//*************************************************************************************
