//Broad_Train-1
//sankaku 追加
#include <esp_now.h>
#include <WiFi.h>
const int adcPin = 34;
//const int R1 = 100000;
//const int R2 = 68000;
const int R1 = 5100;
const int R2 = 2000;
float voltage;
/* 使うピンの定義 */
const int IN1 = 23;
const int IN2 = 22;
/* チャンネルの定義 */
const int CHANNEL_0 = 0;//可聴20_20000
const int CHANNEL_1 = 1;
const int LEDC_TIMER_BIT = 9;   // PWMの範囲(8bitなら0〜255、10bitなら0〜1023)
const int LEDC_BASE_FREQ = 100;  // 周波数(Hz)490-78000-100000-120000-50k_NG
//1k_NG-10k_NG-50k_5k_kion-10k_bres-200-500-40k-1220-800
//60000-30000-15000-7500-3000-1500-20000-10000-490-1000
const int VALUE_MAX = 512;      // PWMの最大値
//===================================
  int Send1;//speed
  int Send2;//speed
  int Send3;//speed
  int Send4;//speed
  int ctr;
  int Train1;//poji
  int Train2;//poji
  int Train3;//poji
  int Train4;//poji
  int Data5;   //servo In
  int Data6;  //servo Out
  int Data7;   //sub_start   1 
//===============================
uint8_t data[15]={99,0,0,0,0,0,0,0,0,0,0,0,0,0,88};
 esp_now_peer_info_t slave;
//================================ 
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int data_len) {
    // 送信元のMACアドレスを使いたい場合は、以下のように取得します
    const uint8_t *mac_addr = info->src_addr; 
    
  if(data[0]==99){
      Send1 = data[1];                 
      Send2 = data[2];
      Send3 = data[3];
      Send4 = data[4]; 
        ctr = data[5];
      Train1= data[6];
      Train2= data[7];
      Train3= data[8];
      Train4= data[9];
      Data5 = data[10];           // Train4 position(3-32)
      Data6 = data[11]; 
      Data7 = data[12];            // ServoOUT position(1:Sub 2:Main)
      Data7 = data[13];
     
    // esp_err_t result = esp_now_send(slave.peer_addr, data, sizeof(data));          
}
}



void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT); // IN1
  pinMode(IN2, OUTPUT); // IN2

  // 【バージョン3.x対応】
  // チャンネルの概念がなくなり、ピンに対して直接 周波数と解像度 を設定します
  ledcAttach(IN1, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcAttach(IN2, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  //===========================
   if (esp_now_init() == ESP_OK) {
  } else {
    return;
  }
  for (int i = 0; i < 6; ++i) {
    slave.peer_addr[i] = (uint8_t)0xff;
  }
  
  esp_err_t addStatus = esp_now_add_peer(&slave);
  if (addStatus == ESP_OK) {       
  }
  esp_now_register_recv_cb(OnDataRecv);
  
}

void loop() {
  //Send1=100;
   Serial.println(Send1);
    forward(Send1*1.5);
   delay(1);
}

