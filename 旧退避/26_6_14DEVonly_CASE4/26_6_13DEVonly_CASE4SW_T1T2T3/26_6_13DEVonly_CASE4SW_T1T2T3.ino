//***************************************

//９９，００，２２，００，００，０１，１６８４５，CRLF
#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include "SPIFFS.h"
#include "constant.h"
#define CHANNEL 0
#define PIN_PE7 25          //PIN_PE7 51 LoopFlag(時間)>>>mega21
//=============
#define NANO_reset  27
#define EN_reset    35
// 変数の実体
struct_message recvData;
StationPayload sendData; // 従来の uint8_t data[18] の代わり
//uint8_t data[20]={99,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88};
byte Poji22  = 0;
byte Poji32  = 0;
byte Train1G = 0;
byte Train2G = 0;
byte Train3G = 0;
byte Train4GS = 0;
byte Train1GS= 0;
byte Train2GS= 0;
byte Train3GS= 0;
byte Train4G = 0;
byte T1MS    = 0;
byte T1SS    = 0;
byte T2MS    = 0;
byte T2SS    = 0;
byte T3MS    = 0;
byte T3SS    = 0;
byte T4MS    = 0;
byte T4SS    = 0;


//ADC-monita/===========
//const byte LED_PIN = 26;  // LED黄色
const byte LED_MPIN = 0;  // LED_motor-B
String ledState;
int restart=0;
const int adcPin = 34;
const int R1 = 5100;
const int R2 = 2000;
float voltage;
//ADC-monita/===========
#define PIN_PE0 4          // Servo In/HIGH=>Sub  LOW=>Main
#define PIN_PE1 18          // Servo In
#define PIN_PE2 23         // Servo Out/HIGH=>Sub LOW=>Main
#define PIN_PE3 19          // Servo Out/Sub
#define StartSW 13
#define servoInt 15
byte ServoInFlag  = 0;       // ServoInFlag(0:Sub, 1:Main)
byte ServoOutFlag1 = 0;    // ServoOutFlag(0:Sub, 1:Main)
byte ServoOutFlag2 =0;
byte SubTimeResetFlag  = 0;  // SetTimeResetフラグ
byte MainTimeResetFlag = 0;  // SetTimeResetフラグ
byte Data6Flag=0;
byte startFlag=0;
byte startFlag21=0;
byte startFlag31=0;
byte startFlag22=0;
byte startFlag32=0;
byte peerFlag=0;
String Termination,SInPoji,Train, Hedder,mTrain;
int  Train1, Train2, Train3,Train4;
String Train1s, Train2s, Train3s,Train4s;
int Stop1, Stop2, Stop3, Stop4;
int Slow1, Slow2, Slow3, Slow4;
int data5;
int s1,m1,f1;
int s2,m2,f2;
int s3,m3,f3;
int s4,m4,f4;
//===================
int Data1;//Train1
int Data2;//Train2
int Data3;//Train3
int Data4;//Train4

int Data8;//Main
int Main2=0;

boolean direction = 0;
//====================
// Flag
volatile int Train1Flag10=0;
volatile int Train2Flag10=0;
volatile int Train3Flag10=0;
volatile int Train4Flag10=0;
volatile int Train1Flag13=0;
volatile int Train2Flag13=0;
volatile int Train3Flag13=0;
volatile int Train4Flag13=0;
volatile int Train1Flag15D1=0;
volatile int Train1Flag15D2=0;
volatile int Train2Flag15D1=0;
volatile int Train2Flag15D2=0;
volatile int Train3Flag15D1=0;
volatile int Train3Flag15D2=0;
volatile int Train4Flag15D1=0;
volatile int Train4Flag15D2=0;

volatile int compete=0;
volatile int Train1TimeSetFlag1=0;
volatile int Train2TimeSetFlag2=0;
volatile int Train3TimeSetFlag3=0;
volatile int Train4TimeSetFlag4=0;
volatile int Train1Flag1=0;
volatile int Train2Flag2=0;
volatile int Train3Flag3=0;
volatile int Train4Flag4=0;

volatile int TrainFlag21=0;
volatile int TrainFlag31=0;

volatile int Data5Flag1=1;
volatile int Data5Flag2=1;
volatile int Data5Flag3=1;
volatile int Data5Flag4=1;
//
volatile int Train1Flag22=0;
volatile int Train1Flag32=0;
volatile int Train2Flag22=0;
volatile int Train2Flag32=0;
volatile int Train3Flag22=0;
volatile int Train3Flag32=0;
volatile int Train4Flag22=0;
volatile int Train4Flag32=0;
//
volatile int Train1delayFlag22=0;//start優先
volatile int Train2delayFlag22=0;//start優先
volatile int Train3delayFlag22=0;//start優先
volatile int Train4delayFlag22=0;//start優先
volatile int Train1delayFlag32=0;//start優先
volatile int Train2delayFlag32=0;//start優先
volatile int Train3delayFlag32=0;//start優先
volatile int Train4delayFlag32=0;//start優先
volatile int Train1sesor10Flag1=0;
volatile int Train2sesor10Flag2=0;
volatile int Train3sesor10Flag3=0;
volatile int Train4sesor10Flag4=0;

// Timer 
unsigned long nowTime;       // 現在時刻 
unsigned long nowTimeOld;
unsigned long SubSetTime=0;    // "4""5""12"スタート時刻
unsigned long MainSetTime=0;    // "4""5""12"スタート時刻

unsigned long SubSetTime22=0;   // "32"スタート時刻　＊
unsigned long MainSetTime32=0;   // "32"スタート時刻　＊
unsigned long SubStartTime22=0;
unsigned long MainStartTime32=0;
unsigned long Sub=0;   // "32"スタート時刻　＊
unsigned long Main=0;   // "32"スタート時刻　＊
unsigned long SubSetTime21=0;    // "21"スタート時刻
unsigned long MainSetTime31=0;    // "21"スタート時刻
unsigned long Time21=0;    // "21"スタート時刻
unsigned long Time31=0;    // "21"スタート時刻
unsigned long Timecompete=0;
unsigned long SubSetTime4=0; 
unsigned long SubSetTime5=0;
unsigned long SubSetTime6=0; 
unsigned long SubSetTime7=0;
unsigned long SubSetTime8=0; 
unsigned long SubSetTime9=0;
unsigned long SubSetTime10=0; 
unsigned long SubSetTime11=0; 
unsigned long MainSetTime11=0; 
unsigned long SubSetTime12=0; 
unsigned long MainSetTime12=0; 
unsigned long SubSetTime13=0;   // "31"スタート時刻　＊
unsigned long MainSetTime13=0;   // "31"スタート時刻　＊
unsigned long SubSetservoTime13=0;
unsigned long Time22=0;
unsigned long Time32=0;
unsigned long Time822=0;
unsigned long Time832=0;
unsigned long count22=0;
unsigned long count32=0;
unsigned long count22Flag=0;
unsigned long count32Flag=0;



unsigned long MainServoTime32;
unsigned long SubServoTime22;
int SubServoTime22Flag=0;
int control_Flag1=0;
int control_Flag2=0;
int startFlag1=0;//start優先
int startFlag2=0;//start優先
int startFlag3=0;//start優先
int startFlag4=0;//start優先
int MainServoTime32Flag=0;
int cont = 0;             // 時間配列カウント(22 & 32)
int Lcont = 0;            // 時間配列カウント(10)
int lpcnt=0;
int lpcnt2=0;
int mc;
uint8_t FB_LIght=13;
 
  uint8_t ctr=0;
 
  uint8_t Data5=0;   //servo In  Data5 1-sub 2-main
  uint8_t Data6=0;  //servo Out Data6 1-sub 2-main
  uint8_t Data7=0;   //sub_start   1
  
  //==============
  uint8_t Send1=0;
  uint8_t y=0; 
  //==============
  uint8_t Send2=0;
  uint8_t z=0; 
   //==============
  uint8_t Send3=0;
  uint8_t v=0; 
    //==============
  uint8_t Send4=0;
  uint8_t w=0; 
  uint8_t startbutton=1;
//=====================
esp_now_peer_info_t slave;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

void Receive();
void in_Servo_Contlor();
void out_Servo_Contlor();
void sankaku();
void control_1();
void control_2();
void control_3();
void control_4();
//void Broad();
void Serialprint();
void Serialprint2();
//======================== 
void setup_servo();
void setup() {
  unsigned long nowTime = millis();  
  setup_servo();
  Serial.println("start_button");
  startbutton=1;
  Serial.begin(250000);   // USB_Moniter 
  Serial2.begin(1000000,SERIAL_8N1,16,17);        // 16Tx,17Rx Mega  18-RX1,17-RX2  serial 1
  //disableCore0WDT();
  //disableCore1WDT();
  pinMode(StartSW ,INPUT_PULLUP);
  pinMode(servoInt ,OUTPUT);
  pinMode(NANO_reset, OUTPUT);
  pinMode(EN_reset,OUTPUT);
  
  digitalWrite(NANO_reset,  LOW);
  delay(500);
  digitalWrite(NANO_reset, HIGH);
   // SPIFFSのセットアップ
  if(!SPIFFS.begin(true)){
    //Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());
  
  if (esp_now_init() != ESP_OK) {
   ESP.restart();
  }
  memset(&slave, 0, sizeof(slave));
  for (int i = 0; i < 6; ++i) {
    slave.peer_addr[i] = (uint8_t)0xff;
  }
  esp_err_t addStatus = esp_now_add_peer(&slave);
  if (addStatus == ESP_OK) {
  }

  //while(digitalRead(StartSW) == HIGH){//1-0

//Broad();
//}
  delay(2000);
  
   
 //=============
  Serial.print("start_button=");
  Serial.println(startbutton);
  //while(digitalRead(StartSW) == HIGH){}//1-0
  startbutton=0; 
  //Broad();
  Serial.print("start_button=");
  Serial.println(startbutton);
  delay(2000); 
  //while(digitalRead(StartSW) == HIGH){}//1-0
  startbutton=1;
  //Broad();
  Serial.print("start_button=");
  Serial.println(startbutton);
 delay(2000); 
 Serial2.write("R");
 delay(100);
 Serial2.write("4");//1=2ms 4ms  err 6ms-OK Loop-4ms
 Serial2.write("Z");
 setup_servo();
 // 固定値の初期化
  sendData.header = 99;
}

void Receive();
void in_Servo_Contlor();
void out_Servo_Contlor();
void sankaku();
void startFlag_compete();
void compete_0();
void compete1_2();
void control_1();
void control_2();
//void control_3();
//void control_4();

void Serialprint();
void Serialprint2();
//=======================
void loop() {
 if(compete==1){
         control_1();
         control_2();
         control_3();
         control_4();
      }
      if(compete==0){
        compete_0();
      } 
// ----------------------------------------------------
  // ① 従来の自動制御ロジック（位置受信による制御）
  // ----------------------------------------------------
  // これまで「data[1] = ...」と書いていた部分を構造体名に変えるだけ！
  sendData.train1_speed = Send1 * 1.4; 
  //sendData.fb_light1    = FB_Light;
  sendData.train2_speed = Send2 * 4.5; 
  // ... (従来の列車位置に基づいた計算をそのまま記述)
  // ----------------------------------------------------
  // ② 右側のスケッチから持ってきた「キーボード割り込み」
  // ----------------------------------------------------
  if (Serial.available() > 0) {
    char input = Serial.read();
    switch(input) {
      case 's': // 停止キーが押されたら、自動制御の値を無視して強制書き換え
        sendData.train1_speed = 0; // 1番列車停止
        Serial.println("【KEY】Train1 -> STOP");
        break;
      case 'w': // 前進キー
        sendData.train1_speed = 150; // 強制的に速度150
        Serial.println("【KEY】Train1 -> FORWARD");
        break;
      // 必要に応じて他の列車のキー（'a'で2番列車など）もここに追加できます
    }
  }

  // ----------------------------------------------------
  // ③ 一括送信（一定間隔、またはループごと）
  // ----------------------------------------------------
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 300) { // 300msごとに送信
    lastSend = millis();
    
    // 構造体をそのまま一斉送信
    esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(sendData));
  }
  
  delay(10); // 適切なウェイト

Receive();
in_Servo_Contlor();
out_Servo_Contlor();
sankaku();
Serialprint2();
startFlag_compete();
//control_3();
//control_4();
}
  
