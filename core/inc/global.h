#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "osGroup.h"
#include "vector.h"
#include "osAtomic.h"

extern OsGroup* globalGroup;
extern OsGroup* loraGroup;
extern OsAtomic<Vector<float, 2>> motorSpeeds;
extern OsAtomic<Vector<float, 2>> motorCurrentSpeeds;
extern OsAtomic<float> imuAngle;


#endif