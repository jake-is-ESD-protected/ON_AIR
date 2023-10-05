// Task control example with external interrupt
#include <Arduino.h>

#define INPUT_PIN 26
#define DO_STUFF 1

TaskHandle_t th_myTask1 = NULL;
TaskHandle_t th_myTask2 = NULL;

static void interruptHandler(void){
	uint32_t msg = DO_STUFF;
	BaseType_t dummy = pdFALSE;
    xTaskNotifyFromISR(th_myTask2, msg, eSetValueWithOverwrite, &dummy);
}

static void myTask1(void* param){
	while(1){
		Serial.println("In task 1!");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

static void myTask2(void* param){
	while(1){
		uint32_t msg = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if(msg == DO_STUFF){
			Serial.println("In task 2!");
		}
	}
}

void setup_example_task_control_IR(){
	Serial.begin(115200);
	pinMode(INPUT_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(INPUT_PIN), interruptHandler, FALLING);
	xTaskCreate(myTask1, "description for T1", 2048, NULL, 1, &th_myTask1);
	xTaskCreate(myTask2, "description for T2", 2048, NULL, 1, &th_myTask2);
}