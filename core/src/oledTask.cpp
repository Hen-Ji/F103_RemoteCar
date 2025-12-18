#include "oledTask.h"

#include "oled.h"
#include "cstring.h"
#include "iicSoftware.h"

#include "joystick.h"
#include "exti.h"
#include "vector.h"

void oledTask(void* args) {
	IicBusSoftware* iicBus = new IicBusSoftware(PA11, PA12, 1); //初始化IIC, SCL -> PA11, SDA -> PA12
	Oled* oled = new Oled(iicBus); //创建OLED
	OsGroupMember* loraMember = new OsGroupMember(loraGroup, "oled"); //加入群组
	String* buffer = new String(128);
	int num = 0, loraNum = 0;
	Vector<float, 2> speeds = {0};
	float angle = 0;

	while (1) {
		while(loraMember->receive()) { //接收lora消息
			OsGroupMessage* msg = loraMember->getMessage();
			if((void*)msg->sender == (void*)"lora") {
				loraNum++; //记录一下
			}
		}

		angle = imuAngle.get(); //获取imu角度
		speeds = motorCurrentSpeeds.get(); //获取电机当前速度

		oled->clear(); //清屏
		oled->show(0, 0, buffer->format("%d, %d", num, loraNum));
		oled->show(0, 1, buffer->format("%.2f, %.2f", speeds[0], speeds[1]));
		oled->show(0, 2, buffer->format("%.2f", angle));
		oled->show(0, 3, buffer->format("%.1f, %d, %d", (float)OsTask::getIdleTask()->getUsageAvg() / 10, OsTask::current()->getIdleStack(), Os::getIdleHeap()));
		oled->update(); //刷新

		num++;
		Os::delay(100);
	}
}