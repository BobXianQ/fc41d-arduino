#include "Arduino.h"
#include "ql_hal.h"

#define ARDUINO_THREAD_SIZE					(1024*4)
#define ARDUINO_THREAD_PRIORITY				(THD_EXTENDED_APP_PRIORITY)

ql_task_t arduino_thread_ctx = NULL;

__weak void setup(void){}
__weak void loop(void){}

static void arduino_thread(void* args) {
	ql_rtos_task_sleep_ms(3000);
	setup();
	while (1) {
		ql_rtos_task_sleep_ms(2);
		loop();
	}
}

void arduino_thread_creat(void) {
	int ret = ql_rtos_task_create(&arduino_thread_ctx,
		(unsigned short)ARDUINO_THREAD_SIZE,
		ARDUINO_THREAD_PRIORITY,
		"arduino",
		arduino_thread,
		NULL);

	if (ret != 0) {
		QL_HAL_LOG(LOG_ERROR, "Failed to create arduino thread: %d\r\n", ret);
	}
}

void quec_app_main(void* argv) {
	arduino_thread_creat();
}
