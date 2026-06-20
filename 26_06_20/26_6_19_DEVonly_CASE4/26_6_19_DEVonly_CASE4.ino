#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include "SPIFFS.h"

// 💡 constant.hに構造体の実体を1つだけ作らせるため、ここでの定義はすべて constant.h に一任します
#include "constant.h" 

#define CHANNEL 0
#define PIN_PE7 25          
#define NANO_reset  27
#define EN_reset    35
#define PIN_PE0 4           
#define PIN_PE1 18          
#define PIN_PE2 23          
#define PIN_PE3 19          
#define StartSW 13
#define servoInt 15 

esp_now_peer_info_t slave;

// === ① 受信コールバック：列車2から「24バイトの共通構造体」で電圧が届く ===
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (len >= sizeof(CombinedPayload)) {
    memcpy(&recvData, incomingData, sizeof(CombinedPayload));
    
    // 💡 届いた統合構造体の中から「voltage」だけを抜き取って最新値を変数に保存
    currentFeedbackVoltage = recvData.voltage;
    
    Serial.print("【列車2より受信】電圧: ");
    Serial.println(currentFeedbackVoltage, 2);
  }
}

// === ② 送信完了コールバック（最新コア仕様に準拠） ===
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {}

// 他のinoタブにある関数のプロトタイプ宣言
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

// =================================================================
// ⚙️ 【高追従・遅延防止】データパッキング ＆ 送受信共通構造体 送信関数
// =================================================================
void PackAndSendStationData() {
  // 制御データを統合構造体の各メンバーにセット
  sendData.header       = 99;              
  sendData.train1_speed = Send1 * 1.4;     
  sendData.fb_light3    = FB_Light;        
  sendData.train2_speed = Send2 * 4.5;     
  sendData.fb_light4    = FB_Light;         
  sendData.train3_speed = Send3;           
  sendData.data5_light  = FB_Light;         
  sendData.train4_speed = Send4 * 2;       
  sendData.data6_light  = FB_Light;         
  sendData.train1_poji  = Train1;          
  sendData.train2_poji  = Train2;          
  sendData.train3_poji  = Train3;          
  sendData.train4_poji  = Train4;          
  sendData.Data5_SIn    = Data5;           
  sendData.Data6_SOut   = Data6;           
  sendData.Data7_Sub    = Data7;           
  sendData.Data8_Main   = Data8;           
  sendData.ctr          = ctr;             
  sendData.startbutton  = startbutton;     
  sendData.data88       = 88;              

  // 💡 【ここを修正】列車2から届いている最新の電圧データを載せてモニターへ送り返す！
  sendData.voltage      = currentFeedbackVoltage; 

  // しゃくり（カクつき）を完全に防止するため、主要データが変化したかチェック
  bool isChanged = (sendData.train1_speed != lastSendData.train1_speed) ||
                   (sendData.train2_speed != lastSendData.train2_speed) ||
                   (sendData.train3_speed != lastSendData.train3_speed) ||
                   (sendData.train4_speed != lastSendData.train4_speed) ||
                   (sendData.Data5_SIn    != lastSendData.Data5_SIn)    ||
                   (sendData.Data6_SOut   != lastSendData.Data6_SOut)   ||
                   (sendData.voltage      != lastSendData.voltage);

  unsigned long currentTime = millis();

  // 「データが変わった瞬間」または「変化がなくても100ms経過した時」だけ送信
  if (isChanged || (currentTime - lastSendTime >= 100)) {
    esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(CombinedPayload));
    
    // 今回送ったデータを記憶
    lastSendData = sendData;
    lastSendTime = currentTime;
  }
}
void setup(){
  setup_servo();
  Serial.println("start_button");
  startbutton = 1;
  Serial.begin(250000);   
  Serial2.begin(1000000, SERIAL_8N1, 16, 17);  

  pinMode(StartSW, INPUT_PULLUP);
  pinMode(servoInt, OUTPUT);
  pinMode(NANO_reset, OUTPUT);
  pinMode(EN_reset, OUTPUT);
  
  digitalWrite(NANO_reset, LOW);
  delay(500);
  digitalWrite(NANO_reset, HIGH);

  if(!SPIFFS.begin(true)){
    return;
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  
  if (esp_now_init() != ESP_OK) {
    ESP.restart();
  }
  
  // 送受信コールバックを新しい統合関数で登録
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  // ブロードキャスト用ピア（相手先設定）
  memset(&slave, 0, sizeof(slave));
  for (int i = 0; i < 6; ++i) {
    slave.peer_addr[i] = (uint8_t)0xff;
  }
  esp_now_add_peer(&slave);

  // スタートボタン処理
  Serial.println("start_button1");
  while(digitalRead(StartSW) == HIGH){}
  delay(2000);
  startbutton = 0; 
  delay(2000); 
  Serial.println("start_button2");
  while(digitalRead(StartSW) == HIGH){}
  startbutton = 2;
  delay(2000); 

  Serial2.write("R");
  delay(100);
  Serial2.write("4");
  Serial2.write("Z");
  setup_servo();
}

void loop(){
  Receive(); // ①シリアル等の受信

  // ②速度制御計算（送信を間引いたので、ここの加減速計算が本来の滑らかなテンポで走ります）
  if(compete == 1){
    control_1(); control_2(); control_3(); control_4();
  }
  if(compete == 0){
    compete_0();
  } 

  // ③手動デバッグ用割り込み
  if (Serial.available() > 0) {
    char input = Serial.read();
    switch(input) {
      case 's': Send1 = 0; break;
      case 'w': Send1 = 100; break;
    }
  }
  Serial.print("【現在の共有電圧】: ");
    Serial.print(currentFeedbackVoltage, 2);
    Serial.println(" V");
  // ④洗練された統合送信関数を実行
  PackAndSendStationData(); 
  
  // ⑤周辺機器の制御
  in_Servo_Contlor();
  out_Servo_Contlor();
  sankaku();
  Serialprint2();
  startFlag_compete();

  // ループ自体のウェイトを20msに縮め、速度制御全体の追従性と滑らかさを最高値に引き上げます
  delay(20); 
}