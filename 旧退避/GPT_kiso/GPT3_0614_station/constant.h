#ifndef CONSTANT_H
#define CONSTANT_H

static const uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
typedef struct struct_message {
  //int command;     // 0=停止, 1=前進, 2=後退など
  float voltage;   // 電圧（列車からの返信用）
} struct_message;
struct_message recvData;

//// Station→Trainへデータ（電圧＋フラグ）
typedef struct struct_return {
  float newvoltage;
  uint8_t flag;
  uint8_t Send1;
} struct_return;
struct_return sendData;
#endif