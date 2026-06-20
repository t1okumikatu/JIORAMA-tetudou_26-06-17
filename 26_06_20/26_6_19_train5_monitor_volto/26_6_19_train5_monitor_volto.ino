#include <esp_now.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "esp_wifi.h"

const char* ssid = "Train5_Monitor";
const char* password = "password123";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// 💡 データを正しく仕分けてキープしておくための変数
float lastKnownVoltage = 0.0;
struct __attribute__((packed)) CombinedPayload {
  uint8_t header;       // data[0]  (99)
  uint8_t train1_speed; // data[1]
  uint8_t fb_light3;    // data[2]
  uint8_t train2_speed; // data[3]
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
  uint8_t data88;       // data[19]  (88)
  
  float voltage;        // 💡 24バイト目に合体している列車2の電圧データ
};

CombinedPayload incomingPack; 
CombinedPayload stationData; // ステーションの最新制御データを保存する箱

// =================================================================
// 📱 スマホ用 HTML / JavaScript 画面データ
// =================================================================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Train5 Monitor 確定版</title>
    <style>
        body { font-family: Arial, sans-serif; background: #222; color: #fff; margin: 20px; }
        h1 { text-align: center; color: #00ffcc; font-size: 24px; }
        .container { max-width: 500px; margin: 0 auto; background: #333; padding: 15px; border-radius: 10px; box-shadow: 0 4px 10px rgba(0,0,0,0.5); }
        .data-row { display: flex; justify-content: space-between; padding: 12px 0; border-bottom: 1px solid #444; font-size: 16px; }
        .label { color: #aaa; }
        .value { font-weight: bold; color: #ffeb3b; font-family: monospace; }
        .space-above { margin-top: 15px; }
        .voltage-box { background: #111; border: 1px solid #00ffcc; padding: 12px; border-radius: 8px; margin-bottom: 15px; display: flex; justify-content: space-between; align-items: center; }
        .voltage-label { color: #00ffcc; font-weight: bold; font-size: 16px; }
        .voltage-value { font-size: 22px; color: #00ffcc; font-weight: bold; font-family: monospace; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Train5 運行モニター</h1>
        
        <div class="voltage-box">
            <span class="voltage-label">🔋 トレイン2 バッテリー電圧:</span>
            <span class="voltage-value"><span id="voltage_val">0.00</span> V</span>
        </div>

        <div class="data-row"><span class="label">Train Spd1:</span><span class="value" id="d1">-</span></div>
        <div class="data-row"><span class="label">Train Spd2:</span><span class="value" id="d3">-</span></div>
        <div class="data-row"><span class="label">Train Spd3:</span><span class="value" id="d5">-</span></div>
        <div class="data-row"><span class="label">Train Spd4:</span><span class="value" id="d7">-</span></div>
        
        <div class="data-row space-above"><span class="label">Train1 / 2 位置:</span><span class="value"><span id="d9">-</span> / <span id="d10">-</span></span></div>
        <div class="data-row"><span class="label">Train3 / 4 位置:</span><span class="value"><span id="d11">-</span> / <span id="d12">-</span></span></div>
        
        <div class="data-row"><span class="label">SIn / SOut:</span><span class="value" id="custom_status">- / -</span></div>
        
        <div class="data-row space-above"><span class="label">Counter / StartBtn:</span><span class="value"><span id="d17">-</span> / <span id="d18">-</span></span></div>
    </div>

    <script>
        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;
        function initWebSocket() {
            websocket = new WebSocket(gateway);
            websocket.onmessage = onMessage;
            websocket.onclose = function() { setTimeout(initWebSocket, 2000); };
        }
        function onMessage(event) {
            var data = event.data.trim().split(',');
            
            if(data.length >= 21) {
                // 0〜19番目（全運行データ）をIDに基づいて画面に反映
                for(var i=0; i<20; i++) {
                    var el = document.getElementById('d' + i);
                    if(el) el.innerText = data[i];
                }
                
                // 💡 ステーションの仕様（data[13]=SIn, data[14]=SOut）に合わせて判定
                var sinVal = data[13];
                var soutVal = data[14];
                
                // 1. SIn側の数値を文字に変換
                if (sinVal == "1") {
                    sinVal = "SUB";
                } else if (sinVal == "2" || sinVal == "0") {
                    sinVal = "MAIN";
                }

                // 2. SOut側の数値を文字に変換 (SInと全く同じルール)
                if (soutVal == "1") {
                    soutVal = "SUB";
                } else if (soutVal == "2" || soutVal == "0") {
                    soutVal = "MAIN";
                }

                var statusEl = document.getElementById('custom_status');
                if (statusEl) statusEl.innerText = sinVal + " / " + soutVal;
                
                // 21番目（data[20]）の電圧データを復元して表示
                var rawVolt = parseInt(data[20]);
                if (rawVolt > 0) {
                    var voltVal = rawVolt / 50.0;
                    var voltEl = document.getElementById('voltage_val');
                    if (voltEl) voltEl.innerText = voltVal.toFixed(2);
                }
            }
        }
        window.addEventListener('load', initWebSocket);
    </script>
</body>
</html>
)rawliteral";

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
  }
}

// === 🎯 ESP-NOW 受信・仕分けコールバック ===
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  
  if (len >= sizeof(CombinedPayload)) {
    memcpy(&incomingPack, incomingData, sizeof(CombinedPayload));
    
    if (incomingPack.header == 99 && incomingPack.data88 == 88) {
      // 🟢 ステーションからの「制御指示」
      stationData = incomingPack;
    } else if (incomingPack.voltage > 0.01) {
      // 🔵 列車2からの「電圧データ」
      lastKnownVoltage = incomingPack.voltage;
    }

    // ステーションの親機データがある時だけスマホへ完全同期送信
    if (stationData.header == 99) {
      String csvStr = "";
      csvStr += String(stationData.header) + ",";       // [0]
      csvStr += String(stationData.train1_speed) + ","; // [1]
      csvStr += String(stationData.fb_light3) + ",";    // [2]
      csvStr += String(stationData.train2_speed) + ","; // [3]
      csvStr += String(stationData.fb_light4) + ",";    // [4]
      csvStr += String(stationData.train3_speed) + ","; // [5]
      csvStr += String(stationData.data5_light) + ",";  // [6]
      csvStr += String(stationData.train4_speed) + ","; // [7]
      csvStr += String(stationData.data6_light) + ",";  // [8]
      csvStr += String(stationData.train1_poji) + ",";  // [9]
      csvStr += String(stationData.train2_poji) + ",";  // [10]
      csvStr += String(stationData.train3_poji) + ",";  // [11]
      csvStr += String(stationData.train4_poji) + ",";  // [12]
      csvStr += String(stationData.Data5_SIn) + ",";    // [13]
      csvStr += String(stationData.Data6_SOut) + ",";   // [14]
      csvStr += String(stationData.Data7_Sub) + ",";    // [15]
      csvStr += String(stationData.Data8_Main) + ",";   // [16]
      csvStr += String(stationData.ctr) + ",";          // [17]
      csvStr += String(stationData.startbutton) + ",";  // [18]
      csvStr += String(stationData.data88) + ",";       // [19]
      
      // 21番目の要素にパッキング電圧を乗せる
      uint8_t packedVolt = (uint8_t)(lastKnownVoltage * 50.0 + 0.5);
      csvStr += String(packedVolt); 

      ws.textAll(csvStr); // WebSocketで全スマホ画面を更新
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password, 1); 
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // チャンネル1固定

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // 💡 文字だけの表示をやめ、ご提示いただいた本格的なHTML画面データを直接返すように修正！
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });

  server.begin();
  Serial.println("Monitor 機（HTML直出し・完全同期版）稼働しました！");
}

void loop() {
  ws.cleanupClients();
  delay(50);
}