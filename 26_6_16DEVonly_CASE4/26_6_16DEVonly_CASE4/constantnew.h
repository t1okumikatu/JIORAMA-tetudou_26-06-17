#ifndef CONSTANTNEW_H
#define CONSTANTNEW_H
struct_message recvData;
StationPayload sendData; // 従来の uint8_t data[18] の代わり
//uint8_t data[20]={99,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88};
String Termination,SInPoji,Train, Hedder,mTrain;
int  Train1, Train2, Train3,Train4;
String Train1s, Train2s, Train3s,Train4s;
volatile int compete = 0;
int Data1;//Train1
int Data2;//Train2
int Data3;//Train3
int Data4;//Train4
int FB_Light;
boolean direction = 0;
uint8_t ctr=0;
uint8_t Data5=0;   //servo In  Data5 1-sub 2-main
uint8_t Data6=0;  //servo Out Data6 1-sub 2-main
uint8_t Data7=0;   //sub_start   1
uint8_t Send1=0;
uint8_t Send2=0;
uint8_t Send3=0;
uint8_t Send4=0;
uint8_t startbutton=1;
volatile int Train1TimeSetFlag1=0;
volatile int Train2TimeSetFlag2=0;
volatile int Train3TimeSetFlag3=0;
volatile int Train4TimeSetFlag4=0;
volatile int Train1Flag1=0;
volatile int Train2Flag2=0;
volatile int Train3Flag3=0;
volatile int Train4Flag4=0;
volatile int Data5Flag1=1;
volatile int Data5Flag2=1;
volatile int Data5Flag3=1;
volatile int Data5Flag4=1;
volatile int Train1Flag22=0;
volatile int Train1Flag32=0;
volatile int Train2Flag22=0;
volatile int Train2Flag32=0;
volatile int Train3Flag22=0;
volatile int Train3Flag32=0;
volatile int Train4Flag22=0;
volatile int Train4Flag32=0;
int mc;
byte ServoInFlag  = 0;       // ServoInFlag(0:Sub, 1:Main)
byte ServoOutFlag1 =0;    // ServoOutFlag(0:Sub, 1:Main)
byte ServoOutFlag2 =0;
byte startFlag=0;
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
volatile int Train1sesor10Flag1=0;
volatile int Train2sesor10Flag2=0;
volatile int Train3sesor10Flag3=0;
volatile int Train4sesor10Flag4=0;
unsigned long SubSetTime22=0;   // "32"スタート時刻　＊
unsigned long MainSetTime32=0;   // "32"スタート時刻　＊
unsigned long Timecompete=0;
unsigned long SubSetTime10=0; 
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
#endif