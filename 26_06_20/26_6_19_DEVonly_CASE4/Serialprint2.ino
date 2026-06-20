 void Serialprint2(){
 unsigned long nowTime = millis(); 
 
  Serial.println(F("========================="));
  Serial.print(F("startFlag------>  "));
  Serial.println(startFlag);
  Serial.print(F("compete===> "));
  Serial.println(compete);
  Serial.print(F("Train1Flag22==>Train2Flag32=="));
  Serial.print(Train1Flag22);
  Serial.println(Train2Flag32); 
  Serial.print(F("Time22===>Time32  "));
  Serial.print(Time22);
  Serial.print(F("  "));
  Serial.println(Time32);
  Serial.print(F("TrainTimeSetFlag1234=="));
  Serial.print(Train1TimeSetFlag1);
  Serial.print(Train2TimeSetFlag2);
  Serial.print(Train3TimeSetFlag3);
  Serial.println(Train4TimeSetFlag4);
  Serial.print(F("nowTime=="));
  Serial.println(nowTime);
  Serial.print(F("SubSetTime22===>MainSetTime32  "));
  Serial.print(SubSetTime22);
  Serial.print(F("  "));
  Serial.println(MainSetTime32);
  Serial.print(F("Data5 Data6  Data7=="));
  Serial.print(Data5);
  Serial.print(Data6);
  Serial.println(Data7);
  Serial.print(F("SubServoTime22==>MainServoTime32  "));
  Serial.print(SubServoTime22);
  Serial.print(F("  "));
  Serial.println(MainServoTime32);
 //=======================
     Serial.print("Train==");
     Serial.println(Train);
     Serial.print("Se1234==");
     Serial.print(Send1);
     Serial.print("-");
     Serial.print(Send2);
     Serial.print("-");
     Serial.print(Send3);
     Serial.print("-");
     Serial.println(Send4);
     Serial.print("Data5==");
     Serial.println(Data5);
     Serial.print("Data6==");
     Serial.println(Data6);
     Serial.print("Data7==");
     Serial.println(Data7);
 }   
 
