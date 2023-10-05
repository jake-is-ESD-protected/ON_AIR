## ESP32 as finite state machine
If you've got a background in engineering, you might have heard about the infamous **garage door** example, as it demonstrates the mechanisms and requirements of a finite state machine pretty well. If you're tired of it and just want to see code, skip to the next chapter.

Imagine that we want to construct an automatic garage door with a single **button**, a **motor** and a **position sensor**. The motor can turn both ways and the sensor only detects in binary, so `0` for *closed* and `1` for *open*. While the door is moving we know the direction of the motor and the previous state of the position sensor. The button will cause the motors to engage in the opposite direction of the last time it was pressed, but only if the motors are currently not active. If they are, the button should immediately stop the door halfway. After such a case, the motor should resume its task after the button has been pressed again.

You can already tell that all of these circumstances are binary variables, something is either "on" or "off", "closed" or "open". This might not always be the case in other finite state machines, but in this case we're only dealing with boolean flags. Code for opening the door might look like this:
```c
// assume a closed door

bool button_was_pressed; // set outside by button
bool door_is_closed; // set outside by sensor

if(button_was_pressed && door_is_closed){
    moving_up = true; // only possible moving direction
    button_was_pressed = false; // reset button state
    while(!button_was_pressed && door_is_closed){
        motor_keep_moving(); // assume small and quick incremental movements
    }
    moving_up = false;
}
```

Assume that both boolean variables are set somewhere outside of this scope (keep that in mind, we'll come back to that assumption). If the button is pressed and the door is closed, the only logical conclusion is to open the door. For that we set an internal state `moving_up = true` and reset the button-flag. Then the motor increments in tiny steps as long as neither the emergency button press nor the detection of the door at the top of the garage are triggered. Remember, the sensor can only sense absolute states, everything in between is just the previous detected state. Eventually we reach the top and `door_is_closed` becomes `false`. We exit the motor job and reset the `moving_up` flag. I think you get the point.

## Interrupts on the ESP32
To implement code that executes based on asynchronous events like a button press without constantly polling for it (btw. polling sucks, stop doing it) external interrupts are best suited. 

```C
// this shit here:
while(1){
	if(digitalRead(INPUT_PIN) == HIGH){
		Serial.println("Polling is shit!");
	}
	else{
		goToSleep();
	}
}
```

Remember how I told you that some flags are set "outside" the scope? That's how we set them: **Interrupts**. An interrupt is the detection of a physical voltage change in a binary context, meaning a switch from **Vcc** to **GND**, also called *falling edge* or vice versa, called a *rising edge*. Since these are physical, you can use physical devices like buttons and switches to trigger interrupts and you can view the signals on the oscilloscope or multimeter. The ESP32 supports detection of such changes on almost every pin and we can assign them like this:

```C
#define INPUT_PIN 26

void interruptHandler(void){
	// do FAST stuff here:
	// set a flag or turn on a LED
}

void setup{
	pinMode(INPUT_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(INPUT_PIN), interruptHandler, FALLING);
}

void loop{

}
```

We define a function that will be run when the interrupt is triggered. Only do quick stuff in here, no `Serial.print()`, or your processor will hate you and crash. In `setup()` we set the required pin as `INPUT` and attach the interrupt by providing the pin, handler and mode. `interruptHandler()` will get called when a voltage on pin 23 drops from Vcc to GND and **only** then. This way, if you provide a button with a pull-up resistor, you can detect user input without polling for it.

## Task scheduling with FreeRTOS
We can use **FreeRTOS** to make our controller do multiple things at once, well not quite. While the ESP32 does have two seperate cores, a task scheduler will work on a single core as well, giving each task a small time window during which it executes its code and then switching to the next one. This makes it feel like multiple things occur at once, even though they just get split into tiny parts which get executed in a round-robin fashion. Tasks can be started and terminated by other tasks and require dynamic memory, *quite a lot in fact*. 

A task is just a function that either runs forever or terminates itself when finished. In higher level languages and OOP this gave birth to terms like "workers", "factories" or "spawners". In this case we will chose a simple and constant method which is easiest to explain and handle. Each task gets started in the beginning and runs forever in a `while(1)`-loop and goes to sleep if certain flags indicate that it should not be active. This way the amount of allocated memory stays constant and we don't have to worry about our resources during runtime.

A FreeRTOS task can be any function, as long as its parameters are a single void pointer for optional arguments and it has no return value. We add an infinite loop in which we do whatever needs to be done. Of course, we need more than one task to profit from our multitasking abilities. In the setup, we create the tasks with their associated functions, a description, memory size, no additional arguments (this would be passed to the `param`-variable if specified), a priority level of 1 and the handle to the task itself, a sort of "leash" to the task which is used for control, we'll come back to that. When you flash this code, the controller will spam `In task 1!` and `In task 2!`, indicating that both are running at the same time. Notice also that the `loop()` is empty, yet things are happening.

```C
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
```

But that's dumb, isn't it? Now we just spam whatever is inside both tasks in an uncontrolled fashion. We somehow need to control the tasks. If we just add a flag to control them, then we will still just poll the `if` statement, which keeps the prints in check, but not the overall usage of the controller. That's why we need notifications.

```C
bool please_print = false;

void interruptHandler(void){ // interrupt handler from before
	please_print = true;
}

// Don't do this:
void myTask1(void* param){
	while(1){
		if(please_print){
			Serial.println("In task 1!");
			please_print = false;
		}
	}
}
```

> **Small sidenote:** the classic `delay()` will not be an option here, because that might interfere with the task schedulers base clock. Instead, if you want to delay something within a task, use `vTaskDelay(x / portTICK_PERIOD_MS)` where `x` is the amount of milliseconds and `portTICK_PERIOD_MS` is a conversion macro to convert the scheduler's ticks (minimal timesteps) to milliseconds.

A **notification** is a built in tool from FreeRTOS which kicks the target task into the shin and says **"hey, you've got mail!"**. The crucial part is: we can trigger such a notification from within an interrupt. This notification is a `uint32_t` variable by default, but we can put anything into these 4 bytes. In the following example, we will periodically write to serial in one task (*synchronous task*) and in another task we react to a notification from an interrupt (*asynchronous task*):

```C
#define INPUT_PIN 26
#define DO_STUFF 1

TaskHandle_t th_myTask1 = NULL;
TaskHandle_t th_myTask2 = NULL;

void interruptHandler(void){
	uint32_t msg = DO_STUFF;
	BaseType_t dummy = pdFALSE;
    xTaskNotifyFromISR(th_myTask2, msg, eSetValueWithOverwrite, &dummy);
}

void myTask1(void* param){
	while(1){
		Serial.println("In task 1!");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void myTask2(void* param){
	while(1){
		uint32_t msg = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if(msg == DO_STUFF){
			Serial.println("In task 2!");
		}
	}
}

void setup(){
	Serial.begin(115200);
	pinMode(INPUT_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(INPUT_PIN), interruptHandler, FALLING);
	xTaskCreate(myTask1, "description for T1", 2048, NULL, 1, &th_myTask1);
	xTaskCreate(myTask2, "description for T2", 2048, NULL, 1, &th_myTask2);
}

void loop(){

}
```
The FreeRTOS syntax can be a bit intimidating at first, but you should be fine by just copy-pasting most of it for now. Within the interrupt we notify task 2 by passing its handle which I mentioned before and the message itself. Remember, 4 byte is all you get with this method. The other parameters are not relevant for this simple tutorial. While task 1 will print every second, the `ulTaskNotifyTake()` will stall task 2 until it receives a notification. `portMAX_DELAY` indicates that we want to wait forever until the notification arrives, there is no comeback-interval. While the task is stalled, the scheduler knows not to waste any resources on it, as it is currently dormant.

Congrats, you now have some pretty powerful yet simple tools to build an efficient **finite state machine**. Now you can send all sorts of notifications around. For this i recommend `enums`, as the numeric value of your notification might not play a role itself:
```C
typedef enum ntf{
	idle,
	blink,
	print_stats,
	sleep,
	reboot
}ntf_t;

typedef enum state{
	init,
	idle,
	blinking,
	printing,
	sleeping,
	rebooting
}state_t;

state_t internal_state = init;

void parser(void* param){
	ntf_t msg = (ntf_t)ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	switch(msg){
		case idle:
			internal_state = idle;
			// switch your FSM to idle
		case blink:
			internal_state = blinking;
			// make your FSM blink
		case print_stats:
			internal_state = printing;
			// print something
		// ...
		default:
			internal_state = idle;
	}
}
```
With such a distinction, you can create a "parser"-task which interprets incoming commands and manipulates the hardware such as LEDs or an LCD while other tasks are running as well. This way you can handle multiple inputs, periodic tasks like sampling and listening to more inputs at once.
