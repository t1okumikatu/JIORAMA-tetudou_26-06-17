#ifndef CONSTANT_H
#define CONSTANT_H

#include <Arduino.h>

// ブロードキャストアドレス
static const uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// 【列車から送信する構造体：4バイト】
typedef struct __attribute__((packed)) struct_message {
  float voltage;
} struct_message;
extern struct_message sendData;

// 💡 【修正：Stationから受信する構造体を完全に同期】
// ステーション側の data[0]〜data[19] の意味に完全に一致させます
typedef struct __attribute__((packed)) struct_return {
  uint8_t header;       // data[0] : 99
  uint8_t data1;        // data[1] : 140
  uint8_t data2;        // data[2] : 0
  uint8_t Send2;        // data[3] : 💡指令。ログの「222」はここに入ります！
  uint8_t dummy[16];    // data[4]〜[19] : 残りの16バイト分（パディング）
} struct_return;
extern struct_return recvData;

// モーターピン・PWM設定
const int VALUE_MAX = 255;
const int ledPin = 6;
const int in1 = 1;
const int in2 = 2;
const int freq = 5000; 
#define resolution 8
const int ch1 = 1;
const int ch2 = 2;
const int ch0 = 0;

int webstopFlag = 0;
bool ledFlag = true;      
bool UpdateFlag = false;
String UpdateURL = "";

#endif