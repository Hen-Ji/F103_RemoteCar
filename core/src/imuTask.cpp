#include "imuTask.h"
#include "imu.h"
#include "iicSoftware.h"
#include "osGroup.h"

void imuTask(void* args) {
    IicBusSoftware* iic = new IicBusSoftware(PB0, PB1, 1); //创建iic
    Imu* imu = new Imu(iic); //创建imu

    imu->initBias(40, 5);

    uint32_t tick = Os::getCurrentTick();
    while(1) {
        size_t cnt = 4;
        while(cnt--) { //计算
            imu->update();
            Os::delayTo(tick+5);

            tick = Os::getCurrentTick();
        }

        static float preAngle = imu->getEulerAngle().y;
        float currAngle = imu->getEulerAngle().y;
        static float angle = 0;

        angle += currAngle - preAngle; //计算角度
        if(preAngle >= 90 && currAngle >= -180 && currAngle <= -90) { //角度从180 -> -180
            angle += 360; //加上一圈
        }
        else if(preAngle <= -90 && currAngle >= 90 && currAngle <= 180) { //角度从-180 -> 180
            angle -= 360; //减去一圈
        }

        imuAngle.set(angle); //设置角度
        preAngle = currAngle;
    }
}