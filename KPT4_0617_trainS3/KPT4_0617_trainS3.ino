#include <esp_now.h>
#include <WiFi.h>
#include "constant.h"

struct_message sendData;
struct_return recvData;

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

  // 安全に構造体へデータをコピー
  if (len >= sizeof(recvData)) {
    memcpy(&recvData, incomingData, sizeof(recvData));
  }
  
  // 画面確認用の文字表示
  Serial.print("Header (data[0]): ");
  Serial.println(recvData.header);
  Serial.print("Command (Send2/data[3]): ");
  Serial.println(recvData.Send2);
  
  // 💡 【やりたかった形に変形！】
  // ステーションから届いた「222」という数値をそのままPWMスピードとして渡します
  Serial.print("モーターをPWM出力で回転: ");
  Serial.println(recvData.Send2);
  
  forward(recvData.Send2); 
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
  StatiNowpeer();
  
  Serial.println("Train2 起動完了しました。");
}

void loop() {
  readV();
 // Serial.println(Send2);
}