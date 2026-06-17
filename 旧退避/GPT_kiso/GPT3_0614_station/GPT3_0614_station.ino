#include <esp_now.h>
#include <WiFi.h>
#include "constant.h"
// === 受信コールバック ===incomingDataをrecvDataに
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&recvData, incomingData, sizeof(recvData));
  Serial.println("recvVoltage  ");
  Serial.println(recvData.voltage, 2);
  sendData.newvoltage=recvData.voltage;
  //Serial.println(" V");;
}
// === 送信完了コールバック ===
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  //Serial.print("Send Status: ");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}
void setup() {
  Serial.begin(115200);
  delay(1000);
  APSTA();
  TrainNowpeer();
  }

void loop() {
  SendV();
  delay(100);
}
