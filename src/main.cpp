#include <Arduino.h>

void myTask1(void* param){
	while(1){
		Serial.println("In task 1!");
	}
}

void myTask2(void* param){
	while(1){
		Serial.println("In task 2!");
	}
}

TaskHandle_t th_myTask1 = NULL;
TaskHandle_t th_myTask2 = NULL;

void setup(){
	Serial.begin(115200);
	xTaskCreate(myTask1, "description for T1", 2048, NULL, 1, &th_myTask1);
	xTaskCreate(myTask2, "description for T2", 2048, NULL, 1, &th_myTask2);
}

void loop(){

}