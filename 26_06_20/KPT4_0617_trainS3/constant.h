#ifndef CONSTANT_H
#define CONSTANT_H

#include <Arduino.h>

// モーターピンとPWM設定
const int in1 = 1;
const int in2 = 2;
const uint32_t VALUE_MAX = 255;
const double freq = 20000.0;
const uint8_t resolution = 8;
const uint8_t ch1 = 0;
const uint8_t ch2 = 1;

// ブロードキャストアドレス
static const uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// ==========================================================
// 🎯 ステーションと100%一致させた「送受信共通の24バイト統合構造体」
// ==========================================================
struct CombinedPayload {
  uint8_t header;       // data[0] (99)
  uint8_t train1_speed; // data[1]
  uint8_t fb_light3;    // data[2]
  uint8_t train2_speed; // data[3]  ← 💡列車2自身の速度指示！
  uint8_t fb_light4;    // data[4]
  uint8_t train3_speed; // data[5]
  uint8_t data5_light;  // data[6]
  uint8_t train4_speed; // data[7]
  uint8_t data6_light;  // data[8]
  uint8_t train1_poji;  // data[9]
  uint8_t train2_poji;  // data[10]
  uint8_t train3_poji;  // data[11]
  uint8_t train4_poji;  // data[12]
  uint8_t Data5_SIn;    // data[13]
  uint8_t Data6_SOut;   // data[14]
  uint8_t Data7_Sub;    // data[15]
  uint8_t Data8_Main;   // data[16]
  uint8_t ctr;          // data[17]
  uint8_t startbutton;  // data[18]
  uint8_t data88;       // data[19] (88)
  
  float voltage;        // 💡 列車2の測定した電圧を載せるエリア
} __attribute__((packed));

// 送受で同じ構造体定義を使います
#ifdef EXTERN_MAIN
  extern CombinedPayload sendData;
  extern CombinedPayload recvData;
#else
  CombinedPayload sendData;
  CombinedPayload recvData;
#endif

// 関数プロトタイプ宣言
void forward(uint32_t pwm);
void reverse(uint32_t pwm);
void brake();
void coast();
void readV();

#endif