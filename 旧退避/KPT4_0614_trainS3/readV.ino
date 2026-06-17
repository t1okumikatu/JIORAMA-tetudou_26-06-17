// ===== 電圧読み取り =====
float readBattery() {
  int adc = analogRead(3); // GPIO3 [cite: 13, 14]
  float v = (adc / 4095.0) * 1.7 * 5.0;  // 分圧×3.3V基準 [cite: 14]
  return v; // [cite: 14]
}

// 🛠️ 【超重要】時間管理変数を関数の外側（グローバル）に引っ越しました！
unsigned long lastVoltageTime = 0;
const unsigned long voltageInterval = 2000; // 2秒ごとに送信

void readV(){
  // 前回の送信から2秒以上経過したか正しくチェック
  if (millis() - lastVoltageTime > voltageInterval) {
    lastVoltageTime = millis();
    
    sendData.voltage = readBattery();
    
    // ESP-NOWで送信
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(sendData));
    
    if(result == ESP_OK){
      Serial.print("Send Voltage: ");
      Serial.println(sendData.voltage, 2);
    } else {
      Serial.println("送信エラー❣");
    }
  }
}