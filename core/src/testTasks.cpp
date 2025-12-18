#include "testTasks.h"
#include "io.h"

void ledTask(void* args) {
	Io io(PC13, Io::Output | Io::PushPull);

	while(1) {
        io.toggle();
        Os::delay(500);
	}
}