#ifndef CONSTANT_H
#define CONSTANT_H

#include <Arduino.h>

// 全員宛てのブロードキャストアドレス
static const uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// === 列車からSTATIONへ返ってくるデータ（現状維持） ===
typedef struct struct_message {
  float voltage;   // 電圧
} struct_message;
extern struct_message recvData;

// ==================================================
// ★構造体形式：各列車の個別データ（1台あたり2バイト）
// ==================================================
struct TrainData {
  uint8_t speed;  // 速度 (0〜255)
  uint8_t cmd;    // コマンド (0=停止, 1=前進, 2=後退)
} __attribute__((packed));

// ==================================================
// ★構造体形式：STATIONから一斉送信する全体のデータ
// ==================================================
struct StationPayload {
  float newvoltage;       // 電圧データ (4バイト)
  TrainData trains[5];    // 1番〜5番列車までのデータ (2バイト × 5台 = 10バイト)
} __attribute__((packed)); // ⚠️配列列車と混在させるために隙間を詰める

extern StationPayload sendData;

#endif