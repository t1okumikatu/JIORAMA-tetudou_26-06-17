#ifndef CONSTANT_H
#define CONSTANT_H

#include <Arduino.h>

// ブロードキャストアドレス
static const uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// 💡 【列車から送信する構造体：4バイト】
typedef struct __attribute__((packed)) struct_message {
  float voltage;
} struct_message;
extern struct_message sendData;

// 💡 【Stationから受信する構造体：6バイト仕様】
typedef struct __attribute__((packed)) struct_return {
  float newvoltage;
  uint8_t flag;
  uint8_t Send2;
} struct_return;
extern struct_return recvData;

// モーターピン・PWM設定
const int VALUE_MAX = 255;      // PWMの最大値
const int ledPin = 6;
const int in1 = 1;              // IN1
const int in2 = 2;              // IN2
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