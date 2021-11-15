#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "rq_handler.h"
#include "callbacks.h"
#include "hardware.h"
#include "literals.h"

// remove this before release
#define IN_DEV

#ifdef IN_DEV
  #define DEBUG_ONLY(statement) {statement}
#else
  #define DEBUG_ONLY(statement) // Nothing
#endif

/*******************************************************************************
 * GLOBAL VARS
*******************************************************************************/
// this is the mac-address of the receiver, in this case the controler inside
uint8_t dest_addr[] = {0xC8, 0xC9, 0xA3, 0xD2, 0x31, 0x00};
char inc_cmd[5];
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool bell_ringing = false;

/*******************************************************************************
 * SETUP
*******************************************************************************/
void setup() {


  DEBUG_ONLY(Serial.begin(115200);\
             Serial.printf("****************\nIDENTIFIER: SLAVE, ON AIR SIGN\n****************\n");)
  
  pinMode(BUTTON_IN_PIN, INPUT);
  pinMode(BUTTON_LED_PIN, OUTPUT);
  pinMode(LED_ARRAY, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    DEBUG_ONLY(Serial.println("Error initializing ESP-NOW");)
    while(1)
    {
      digitalWrite(LED_ARRAY, HIGH);
      delay(500);
      digitalWrite(LED_ARRAY, LOW);
      delay(500);
    }
  }

  lcd.init();
  lcd.backlight();

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, dest_addr, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;    
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    DEBUG_ONLY(Serial.println("Failed to add peer");)
    digitalWrite(LED_BUILTIN, HIGH);
    return;
  }
}

/*******************************************************************************
 * LOOP
*******************************************************************************/
void loop() {

  digitalWrite(BUTTON_LED_PIN, LOW);
  while(digitalRead(BUTTON_IN_PIN) == HIGH);
  bell_ringing = true;
  
  uint8_t data[] = ID_BELL_INT;
  esp_err_t err = esp_now_send(dest_addr, (uint8_t *) data, sizeof(data));
  
  DEBUG_ONLY(                                   \
  if (err == ESP_OK) {                          \
    Serial.println("Sent with success");        \
  }                                             \
  else {                                        \
    Serial.println("Error sending the data");   \
  })

  lcd.clear();
  delay(20);
  lcd.print(TXT_RINGING);
  while(bell_ringing)
  {
    digitalWrite(BUTTON_LED_PIN, HIGH);
    delay(100);
    digitalWrite(BUTTON_LED_PIN, LOW);
    delay(100);
    digitalWrite(BUTTON_LED_PIN, HIGH);
    delay(100);
    digitalWrite(BUTTON_LED_PIN, LOW);
    delay(1000);
  }
}


/*******************************************************************************
 * CALLBACK FUNCTIONS
*******************************************************************************/
// data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  DEBUG_ONLY(                                                                             \
  Serial.print("\r\nLast Packet Send Status:\t");                                         \
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");  \
)}

// data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(inc_cmd, incomingData, sizeof(inc_cmd));
  DEBUG_ONLY(Serial.println(inc_cmd);)
  lcd.printf(inc_cmd);
  int instruction = handle_rq(inc_cmd);
  lcd.clear();
  delay(20);
  bell_ringing = false;
  
  switch(instruction)
  {
    case 0: digitalWrite(LED_ARRAY, HIGH);
            lcd.print(TXT_WORKING_EXCUSE);
            break;

    case 1: digitalWrite(LED_ARRAY, HIGH);
            lcd.print(TXT_MEETING_EXCUSE);
            break;
    
    case 2: digitalWrite(LED_ARRAY, HIGH);
            lcd.print(TXT_RECORDING_EXCUSE);
            break;

    case 3: digitalWrite(LED_ARRAY, HIGH);
            lcd.print(TXT_WAIT_EXCUSE);
            break;
    
    case 4: digitalWrite(LED_ARRAY, LOW);
            lcd.print(TXT_WELCOME);
            break;

    default: lcd.print(TXT_UNKNOWN);        
  }

  lcd.setCursor(0, 0);

}