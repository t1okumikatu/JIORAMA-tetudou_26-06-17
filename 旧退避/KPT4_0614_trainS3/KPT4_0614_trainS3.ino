#include <esp_now.h>
#include <WiFi.h>
#include "constant.h"

// 構造体の実体をここで宣言
struct_message sendData;
struct_return recvData;

// ===== 受信コールバック =====
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  // 届いたデータを構造体にコピー
  memcpy(&recvData, incomingData, sizeof(recvData));
  
  Serial.print("recv.newvoltage: ");
  Serial.println(recvData.newvoltage);
  Serial.print("Command (Send2): ");
  Serial.println(recvData.Send2);
  
  // Stationからの指令(Send2)で動く
  switch (recvData.Send2) {
    case 0:  Serial.println("停止"); brake(); break;
    case 1:  Serial.println("前進"); forward(170); break;
    case 2:  Serial.println("後退"); reverse(150); break;
    default: break;
  }
}

// ===== 送信完了コールバック =====
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  // 必要に応じてログを出せます
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(in1, OUTPUT);  
  pinMode(in2, OUTPUT);  
  
  ledcAttachChannel(in1, freq, resolution, ch1);
  ledcAttachChannel(in2, freq, resolution, ch2);
  
  WiFi.mode(WIFI_STA);
  StatiNowpeer(); // ESP-NOWの初期化とペアリング
  
  Serial.println("Train2 起動完了しました。");
}

void loop() {
  // 💡 readV()関数内のタイマーが正しく動くので、loop内では常に呼び出すだけで安全です
  readV();
}