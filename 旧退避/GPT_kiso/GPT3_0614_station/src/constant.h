#ifndef CONSTANT_H
#define CONSTANT_H
// 列車（ESP32-S3）の STAモードMACアドレスを入力
//uint8_t trainAddress[] = {0x98, 0x3D, 0xAE, 0x60, 0x05, 0x38};
//元祖列車84:CC:A8:5D:10:78
uint8_t trainAddress[] = {0x84, 0xCC, 0xA8, 0x5D, 0x10, 0x78};
//Train>>>Station受信データ（電圧）
typedef struct struct_message {
  //int command;     // 0=停止, 1=前進, 2=後退など
  float voltage;   // 電圧（列車からの返信用）
} struct_message;
struct_message recvData;
//// Station→Trainへデータ（電圧＋フラグ）
typedef struct struct_return {
  float newvoltage;
  uint8_t flag;
  uint16_t Send1;
} struct_return;
struct_return sendData;
#endif