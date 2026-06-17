#ifndef CONSTANT_H
#define CONSTANT_H
// Station（親）の STAモードMACアドレス
//A0:B7:65:58:6E:6C   RSSI
uint8_t stationAddress[] = {0xA0, 0xB7, 0x65, 0x58, 0x6E, 0x6C};
//uint8_t stationAddress[] = {0x24, 0x6F, 0x28, 0x7A, 0x0A, 0xBC};
//24:6F:28:7A:0A:BC 治具
// データ構造体
typedef struct struct_message {
  //int command;
  float voltage;
} struct_message;
struct_message sendData;
//struct_message recvData;

//// Station→Trainへデータ（電圧＋フラグ）
typedef struct struct_return {
  float newvoltage;
  uint8_t flag;
  uint16_t Send1;
} struct_return;
struct_return recvData;


const int VALUE_MAX = 255;      // PWMの最大値
const int ledPin =6;
const int in1 = 23;//23    //1
const int in2 = 22;//22    //
const int freq = 5000; 
#define resolution 8
const int ch1 =1;
const int ch2 =2;
const int ch0 =0;
int webstopFlag=0;
bool ledFlag = true;      // LED制御フラグ
bool UpdateFlag = false;
String UpdateURL = "";
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
  int T2ADC;
#endif
//----------------------------------------
