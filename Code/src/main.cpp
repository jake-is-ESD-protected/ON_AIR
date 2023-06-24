#include "lcd.h"
#include "tasks.h"
#include "states.h"
#include "hardware.h"
#include "mywebserver.h"


void setup() {

  Serial.begin(115200);
  Serial.printf("\r\n****************\r\n---ON AIR---\r\n****************\r\n");
  
  init_gpios();
  init_ISRs();
  lcd.init_all();
  lcd.show_init_screen();
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  lcd.clear();

  mywebserver_init();

  xTaskCreate(mainLoopTask,
              "main driver loop",
              4096,
              NULL,
              1,
              &tLoop);

  vTaskDelay(3000 / portTICK_PERIOD_MS);  

    cmd_t c = {
    .origin = ORG_SW,
    .content = STATE_IDLE
  };       

  task_notify(tLoop, c, false);
}

void loop() {

}