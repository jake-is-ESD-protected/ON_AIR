// Task spam example (this is bad on purpose!)
// If the creators of the Arduino framework didn't put in some brain juices behind
// the scenes, this code would crash, because two tasks access the `Serial` line
// at the same time. Thanks to them, this wonky example works.
#include <Arduino.h>

void myTask1(void* param){
	while(1){   // This is like your custom `loop()` function
		Serial.println("In task 1!");
	}
}

void myTask2(void* param){
	while(1){   // This is like your custom `loop()` function
		Serial.println("In task 2!");
	}
}

TaskHandle_t th_myTask1 = NULL;
TaskHandle_t th_myTask2 = NULL;

void setup_example_task_spam(){
	Serial.begin(115200);
	xTaskCreate(myTask1, "description for T1", 2048, NULL, 1, &th_myTask1);
	xTaskCreate(myTask2, "description for T2", 2048, NULL, 1, &th_myTask2);
}
