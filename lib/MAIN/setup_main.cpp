#include "setup_main.h"
#include "lcdisplay.h"
#include "tasks.h"
#include "states.h"
#include "hardware.h"
#include "core.h"

void setup_main(){
    Serial.begin(115200);
    Serial.printf("\r\n****************\r\n---ON AIR---\r\n****************\r\n");
    
    init_gpios();
    init_ISRs();
    lcd.init_all();
    lcd.show_init_screen();
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    lcd.clear();

    TaskHandle_t* p_t_loop = get_task_handle(t_loop);

    xTaskCreate(mainLoopTask,
                "main driver loop",
                4096,
                NULL,
                1,
                p_t_loop);

    vTaskDelay(500 / portTICK_PERIOD_MS);  

        cmd_t c = {
        .origin = ORG_SW,
        .content = STATE_IDLE
    };       

    task_notify(*p_t_loop, c, false);
}
