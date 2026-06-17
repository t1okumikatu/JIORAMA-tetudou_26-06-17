#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include "SPIFFS.h"
#include "constant.h"
#include "constantnew.h"

#define CHANNEL 0
#define PIN_PE7 25          // PIN_PE7 51 LoopFlag(時間)>>>mega21
#define NANO_reset  27
#define EN_reset    35
#define PIN_PE0 4           // Servo In/HIGH=>Sub  LOW=>Main
#define PIN_PE1 18          // Servo In
#define PIN_PE2 23         // Servo Out/HIGH=>Sub LOW=>Main
#define PIN_PE3 19          // Servo Out/Sub
#define StartSW 13
#define servoInt 15 

esp_now_peer_info_t slave;

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

// 💡 プロトタイプ宣言の重複を整理
void Receive();
void in_Servo_Contlor();
void out_Servo_Contlor();
void sankaku();
void startFlag_compete();
void compete_0();
void compete1_2();
void control_1();
void control_2();
void control_3();
void control_4();
void Serialprint();
void Serialprint2();
void setup_servo();

// 💡 20バイトの配列データを構造体にしっかり詰め込む関数（Broad関数を統合・最適化）
void PackStationData() {
  sendData.header       = 99;              // data[0]
  sendData.train1_speed = Send1 * 1.4;     // data[1]
  
  // 💡 構造体の中の正しい名前に合わせて修正しました
  sendData.fb_light3    = FB_Light;        // data[2]
  sendData.train2_speed = Send2 * 4.5;     // data[3] 
  sendData.fb_light4    = FB_Light;        // data[4]
  sendData.train3_speed = Send3;           // data[5]
  sendData.fb_light3    = FB_Light;        // data[6] (※構造体側がfb_light3の使い回しになっているため)
  sendData.train4_speed = Send4 * 2;       // data[7]
  sendData.fb_light4    = FB_Light;        // data[8] (※ここも構造体の名前に合わせます)
  
  // 💡 Serial2のReceive()から届いたポジションデータ
  sendData.train1_poji  = Train1;          // data[9]
  sendData.train2_poji  = Train2;          // data[10]
  sendData.train3_poji  = Train3;          // data[11]
  sendData.train4_poji  = Train4;          // data[12]
  
  sendData.Data5        = Data5;           // data[13] (SIn)
  sendData.Data6        = Data6;           // data[14] (SOut)
  sendData.Data7        = Data7;           // data[15] (SubLed)
  sendData.Data8        = Data7;           // data[16] (MainLed 重複回避用)
  sendData.ctr          = ctr;             // data[17]
  sendData.startbutton  = startbutton;     // data[18]
  sendData.data88       = 88;              // data[19] フッター
}
void setup(){
  setup_servo();
  Serial.println("start_button");
  startbutton = 1;
  Serial.begin(250000);   // USB_Moniter 
  Serial2.begin(1000000, SERIAL_8N1, 16, 17);  // 16Tx,17Rx Mega

  pinMode(StartSW, INPUT_PULLUP);
  pinMode(servoInt, OUTPUT);
  pinMode(NANO_reset, OUTPUT);
  pinMode(EN_reset, OUTPUT);
  
  digitalWrite(NANO_reset, LOW);
  delay(500);
  digitalWrite(NANO_reset, HIGH);

  // SPIFFSのセットアップ
  if(!SPIFFS.begin(true)){
    return;
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());
  
  if (esp_now_init() != ESP_OK) {
    ESP.restart();
  }
  
  // 受信・送信コールバックの登録
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  memset(&slave, 0, sizeof(slave));
  for (int i = 0; i < 6; ++i) {
    slave.peer_addr[i] = (uint8_t)0xff;
  }
  esp_now_add_peer(&slave);

  // スタートボタン待機処理群
  while(digitalRead(StartSW) == HIGH){}
  delay(2000);
  
  Serial.print("start_button=");
  Serial.println(startbutton);
  while(digitalRead(StartSW) == HIGH){}
  startbutton = 0; 
  Serial.print("start_button=");
  Serial.println(startbutton);
  delay(2000); 

  while(digitalRead(StartSW) == HIGH){}
  startbutton = 1;
  Serial.print("start_button=");
  Serial.println(startbutton);
  delay(2000); 

  Serial2.write("R");
  delay(100);
  Serial2.write("4");
  Serial2.write("Z");
  setup_servo();

  // 固定値の初期化
  sendData.header = 99;
}

void loop(){
  // ① Serial2から常に最新の位置・スイッチ状態を受信
  Receive();

  // ② コンピートモードの判定
  if(compete == 1){
    control_1();
    control_2();
    control_3();
    control_4();
  }
  if(compete == 0){
    compete_0();
  } 

  // ----------------------------------------------------
  // ③ 自動制御および最新データのパッキング
  // ----------------------------------------------------
  PackStationData(); // 上で定義した関数を呼び出し、一括でデータを最新にします

  // ----------------------------------------------------
  // ④ キーボード割り込み（デバッグ用の強制書き換え）
  // ----------------------------------------------------
  if (Serial.available() > 0) {
    char input = Serial.read();
    switch(input) {
      case 's': 
        sendData.train1_speed = 0; // 1番列車強制停止
        Serial.println("【KEY】Train1 -> STOP");
        break;
      case 'w': 
        sendData.train1_speed = 150; // 1番列車強制前進
        Serial.println("【KEY】Train1 -> FORWARD");
        break;
    }
  }

  // ----------------------------------------------------
  // ⑤ 300msごとのESP-NOW一括送信
  // ----------------------------------------------------
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 300) { 
    lastSend = millis();
    
    // 💡 もし printStationData() のエラーが出る場合はここをコメントアウトしてください
    printStationData();  
    
    // 20バイトの構造体をブロードキャストアドレス（全列車・モニター）へ送信
    esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(sendData));
  }
  
  // 各種周辺制御関数の実行
  in_Servo_Contlor();
  out_Servo_Contlor();
  sankaku();
  Serialprint2();
  startFlag_compete();

  delay(50); // 元の500msだとキーボード等の反応が遅くなるため50ms程度を推奨します
}