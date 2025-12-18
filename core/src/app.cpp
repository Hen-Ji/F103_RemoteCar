#include "global.h"
#include "app.h"

#include "os.h"
#include "osTask.h"

#include "testTasks.h"
#include "oledTask.h"
#include "loraTasks.h"
#include "motorTasks.h"
#include "joystickTask.h"
#include "imuTask.h"
#include "pid.h"
#include "math.h"
#include "format.h"

OsGroup* globalGroup;
OsGroup* loraGroup;
OsAtomic<Vector<float, 2>> motorSpeeds;
OsAtomic<Vector<float, 2>> motorCurrentSpeeds;
OsAtomic<float> imuAngle;

void remoteTask(void* args) { //遥控器任务
	globalGroup = new OsGroup("global group", sizeof(size_t) * 8, 16); //创建群组
	loraGroup = new OsGroup("lora group", sizeof(uint8_t) * 64, 1);
	motorSpeeds.set(0);
	motorCurrentSpeeds.set(0);
	imuAngle.set(0);

	new OsTask(ledTask, "ledTask", 64, 3); //创建呼吸灯任务
	new OsTask(joystickTask, "joystickTask", 256, 2); //摇杆任务

	OsTask::removeCurrent(); //删除启动任务
}

void mainTask(void* args) { //小车任务
	globalGroup = new OsGroup("global group", sizeof(size_t) * 8, 16); //创建群组
	loraGroup = new OsGroup("lora group", sizeof(uint8_t) * 64, 1);
	motorSpeeds.set(0);
	motorCurrentSpeeds.set(0);
	imuAngle.set(0);

	new OsTask(ledTask, "ledTask", 64, 3); //呼吸灯任务
	new OsTask(imuTask, "imuTask", 196, 2); //imu任务
	new OsTask(loraReceiveTask, "loraRxTask", 128, 2); //lora模块接收任务
	new OsTask(motorTask, "motorTask", 128, 2); //电机任务

	new OsTask(oledTask, "oledTask", 128, 1); //显示任务

    float a = 0, s = 0;
    Vector<float, 2> vals = {0};
    PosPid<float> anglePid(0.011, 0, 0);
	OsGroupMember *loraMember = new OsGroupMember(loraGroup, "main");

	while(1) {
		if(loraMember->receive()) { //获取lora数据
			size_t idx = 0;
			char* str = (char*)loraMember->getMessage()->message; //获取字符串

			const char* const deviceStr = "motor"; //判断命令是否是motor
			for(idx = 0; str[idx] != 0 && deviceStr[idx] != 0; idx++) {
				if(str[idx] != deviceStr[idx]) break;
			}
			if(str[idx] == ' ' && deviceStr[idx] == 0) { //是
				size_t len = 0;

				idx++; //第一个参数
				float x = Format::toFloat(&str[idx], &len);
				if(x > -0.01 && x < 0.01) x = 0;
				x *= -1;

				idx += len + 1; //第二个参数
				float y = Format::toFloat(&str[idx], &len);
				if(y > -0.01 && y < 0.01) y = 0;
				y *= -1;

				idx += len + 1; //第三个参数
				s = Format::toFloat(&str[idx], &len); //速度
				if(s > -0.05 && s < 0.05) s = 0;
				s *= -1;

				//计算角度，范围在-180到180
				if((x != 0 || y != 0) && x*x + y*y > 0.25) a = atan2(y, x) / 3.1415926535 * 180;
			}
		}

		float angle = imuAngle.get(); //获取imu角度
		angle = angle - (int)angle / 360 * 360;
		if(angle > 180) angle -= 360; //限制范围在-180到180
		else if(angle < -180) angle += 360;

		float error = angle - a; //计算误差
		if(error > 180) error -= 360; //限制范围在-180到180
		else if(error < -180) error += 360;

		if(error > -2 && error < 2) {
			error = 0; //在范围内就当没有误差了
			anglePid.ei = 0; //清零积分
		}

		anglePid.ei += error;//积分
		anglePid.ed = error - anglePid.ep; //微分
		float out = anglePid.out(error); //pid控制
		if(out > 1) out = 1; //输出限幅
		else if(out < -1) out = -1;
		
		vals[0] = out*0.7 + s; //给电机速度
		vals[1] = out*0.7 - s;
		motorSpeeds.set(vals);

		Os::delay(40); //延时40ms
	}
}

void appMain(void) {
	//new OsTask(mainTask, "mainTask", 128, 2); //创建小车主任务
	new OsTask(remoteTask, "remoteTask", 128, 2); //创建遥控器主任务
	Os::start(); //启动OS
}