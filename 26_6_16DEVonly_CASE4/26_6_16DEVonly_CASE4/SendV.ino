// 定期的にコマンド送信
void SendV() {
  static unsigned long prevMillis = 0;
  
  // 300msごとに自動でESP-NOW送信
  if (millis() - prevMillis > 300) {
    prevMillis = millis();
    
    // 常に最新の受信電圧を送信データに反映
    sendData.newvoltage = recvData.voltage;
    
    // ESP-NOWで構造体データを一斉送信
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(sendData));
    
    // シリアル確認用ログ
    Serial.print("Send -> Voltage: ");
    Serial.print(sendData.newvoltage, 2);
    //Serial.print(" | Train1_Cmd: ");
    //Serial.println(sendData.trains[0].cmd); // 1番列車の現在の状態を表示
  }
}
  // シリアル入力によるコマンド個別設定（ここでは例として1番列車を変更）
/*
  if (Serial.available() > 0) {
    char input = Serial.read();
    switch(input) {
      case 's':
        sendData.trains[0].cmd = 0; // 1番列車: 停止
        sendData.trains[0].speed = 0;
        Serial.println("【COMMAND】Train1 -> STOP");
        break;
      case 'w':
        sendData.trains[0].cmd = 1; // 1番列車: 前進
        sendData.trains[0].speed = 150; // 例としてスピードを150に設定
        Serial.println("【COMMAND】Train1 -> FORWARD");
        break;
      case 'x':
        sendData.trains[0].cmd = 2; // 1番列車: 後退
        sendData.trains[0].speed = 150;
        Serial.println("【COMMAND】Train1 -> BACK");
        break;
      default:
        // 改行コードなどを弾くため、無効メッセージは必要な場合のみ
        break;
        */
    
