from irrecvdata import irGetCMD #导入红外解码器库

recvPin = irGetCMD(19) #将红外解码器与引脚（19）关联

#得到红外键值后，在“Shell”中重复打印
try:
    while True:
        irValue = recvPin.ir_read() #调用 ir_read() 读取按下的键的值并将其赋值给IRValue
        if irValue:
           print(irValue) # 发送红外线接收器的irValue并打印
except:
    pass