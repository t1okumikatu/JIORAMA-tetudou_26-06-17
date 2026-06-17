// ===== 電圧読み取り =====
float readBattery() {
  int adc = analogRead(3); // GPIO3 (例)
  float v = (adc / 4095.0) * 1.7 * 5.0;  // 分圧×3.3V基準
  return v;
}
void readV(){
unsigned long lastVoltageTime = 0;
const unsigned long voltageInterval = 1000; // 3秒ごと


// 電圧を3秒ごとに送信
  if (millis() - lastVoltageTime > voltageInterval) {
    lastVoltageTime = millis();
    sendData.voltage = readBattery();
    esp_err_t result = esp_now_send(stationAddress, (uint8_t *)&sendData, sizeof(sendData));
    if(result==ESP_OK){
    Serial.println("Send Voltage ");
    Serial.println(sendData.voltage, 2);
    //Serial.println(" V");
  } else {
    Serial.println("送信エラー❣");
  }
  }
}