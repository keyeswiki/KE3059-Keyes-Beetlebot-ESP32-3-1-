from irrecvdata import irGetCMD #导入红外解码器.

recvPin = irGetCMD(19) #将红外解码器与引脚(19)关联.

#当获得红外键值时，在“Shell”中重复打印.
try:
    while True:
        irValue = recvPin.ir_read() #调用ir_read()来读取所按键的值并将其赋值给IRValue.
        if irValue:
           print(irValue) # 发送红外接收机的irValue并打印结果.
except:
    pass