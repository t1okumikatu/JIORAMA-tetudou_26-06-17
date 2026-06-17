#ifndef CONSTANT_H
#define CONSTANT_H

#include <Arduino.h>

// ブロードキャストアドレス
static const uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// 列車からの返信用構造体（必要に応じて）
typedef struct struct_message {
  float voltage; 
} struct_message;
extern struct_message recvData;

// ==================================================
// ★従来の配列(data[0]〜[17])と完全に一致する構造体
// ==================================================
struct StationPayload {
  uint8_t header;       // data[0] (99)
  uint8_t train1_speed; // data[1]
  uint8_t fb_light1;    // data[2]
  uint8_t train2_speed; // data[3]
  uint8_t fb_light2;    // data[4]
  uint8_t train3_speed; // data[5]
  uint8_t fb_light3;    // data[6]
  uint8_t train4_speed; // data[7]
  uint8_t fb_light4;    // data[8]
  uint8_t train5_speed; // data[9]
  uint8_t fb_light5;    // data[10]
  uint8_t train1_poji;  // data[11]
  uint8_t train2_poji;  // data[12]
  uint8_t train3_poji;  // data[13]
  uint8_t train4_poji;  // data[14]
  uint8_t Data5;        // data[15] (SIn)
  uint8_t Data6;        // data[16] (SOut)
  uint8_t Data7;        // data[17] (SubLed / ctr)
  uint8_t startbutton;  
  uint8_t data88;  
} __attribute__((packed)); // ⚠️隙間を詰める

extern StationPayload sendData;
//sendData.train1_speed = Send1 * 1.4; // 構造体の名前に代入して送信！

#endif