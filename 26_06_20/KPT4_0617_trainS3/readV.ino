#include "constant.h"

// ===== 電圧読み取り =====
float readBattery() {
  int adc = analogRead(3); // GPIO3
  float v = (adc / 4095.0) * 1.7 * 5.0; // 分圧×3.3V基準
  return v;
}

// 時間管理変数
unsigned long lastVoltageTime = 0;
const unsigned long voltageInterval = 3000; // 💡 500ms(0.5秒)ごとに細かくステーションに報告

void readV(){
  if (millis() - lastVoltageTime > voltageInterval) {
    lastVoltageTime = millis();
    
    // 💡 共通の送信構造体の「voltage」部分だけに測定値を詰め込みます
    sendData.voltage = readBattery();
    
    // 他の制御エリアは空（0）のまま、24バイトの形でステーションへ送信
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(CombinedPayload));
    
    if(result == ESP_OK){
      Serial.print("Send Voltage: ");
      Serial.println(sendData.voltage, 2);
    } else {
      Serial.println("送信エラー❣");
    }
  }
}