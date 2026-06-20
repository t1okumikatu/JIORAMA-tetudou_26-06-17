#include <esp_now.h>
#include <WiFi.h>
#include "constant.h"

// ===== Stationから受信した生データをシリアル表示する関数 =====
void printReceivedStationData(const uint8_t *incomingData, int len) {
  String logLine = "【受信生データ配列】: ";
  for (int i = 0; i < len; i++) {
    logLine += "data[";
    logLine += String(i);
    logLine += "]:";
    logLine += String(incomingData[i]);
    if (i < len - 1) {
      logLine += ",  ";
    }
  }
  Serial.println(logLine);
}

// ===== 受信コールバック =====
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  // 生データのシリアル出力
  printReceivedStationData(incomingData, len);
  
  // 安全に構造体へデータをコピー（24バイトの共通構造体で受け取ります）
  if (len >= sizeof(CombinedPayload)) {
    memcpy(&recvData, incomingData, sizeof(CombinedPayload));
    
    Serial.print("Header (data[0]): ");
    Serial.println(recvData.header);
    
    // 💡 列車2なので、受け取る速度指示は train2_speed (旧Send2/data[3]) です
    Serial.print("列車2 速度指示(data[3]): ");
    Serial.println(recvData.train2_speed);
    
    // モーターをPWM出力で回転
    forward(recvData.train2_speed);
  }
}

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(in1, OUTPUT);  
  pinMode(in2, OUTPUT);  
  
  ledcAttachChannel(in1, freq, resolution, ch1);
  ledcAttachChannel(in2, freq, resolution, ch2);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  // ピア接続（送信先設定）
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0; // ステーション側のチャンネルと合わせてください(通常0)
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("peer失敗");
    return;
  }
  
  Serial.println("Train2 共通構造体仕様で起動完了しました。");
}

void loop() {
  // 電圧を定期測定してステーションへフィードバック
  readV();
}