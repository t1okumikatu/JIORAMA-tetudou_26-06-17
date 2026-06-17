#include <esp_now.h>
#include <WiFi.h>
#include "constant.h"

// 変数の実体をここで定義
struct_message recvData;
StationPayload sendData;

// === 受信コールバック ===
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  // 列車側から送られてきた電圧データを受け取る
  if (len >= sizeof(recvData)) {
    memcpy(&recvData, incomingData, sizeof(recvData));
    Serial.print("recvVoltage: ");
    Serial.println(recvData.voltage, 2);
    
    // 送信データ側の電圧値も更新しておく
    sendData.newvoltage = recvData.voltage;
  }
}

// === 送信完了コールバック ===
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  // 必要に応じてログを出力
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  // 初期データの準備（例: すべての列車を停止状態に）
  sendData.newvoltage = 0.0;
  for(int i = 0; i < 5; i++) {
    sendData.trains[i].speed = 0;
    sendData.trains[i].cmd = 0; // 停止
  }

  
  TrainNowpeer();
}

void loop() {
  SendV();
  delay(100);
}