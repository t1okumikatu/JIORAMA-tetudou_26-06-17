#include <esp_now.h>
#include <WiFi.h>
#include "constant.h"
// ===== 受信コールバック =====
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&recvData, incomingData, sizeof(recvData));
  Serial.println("recv.newvoltage");
  Serial.println(recvData.newvoltage);
  switch (recvData.Send1) {
    case 0: Serial.println("停止");
    brake(); break;
    case 1: Serial.println("前進"); 
    forward(170);break;
    case 2: Serial.println("後退"); 
    reverse(150); break;
    default: break;
  }
}
// ===== 送信完了コールバック =====
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  //Serial.print("Send Status: ");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}
void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(in1, OUTPUT);  // IN1
  pinMode(in2, OUTPUT);  // IN2
  ledcAttachChannel(in1, freq, resolution,ch1);
  ledcAttachChannel(in2, freq, resolution,ch2);
  APSTA();
  StatiNowpeer();
  }
void loop() {
  //forward(180);
  readV();
  delay(2000);
}

