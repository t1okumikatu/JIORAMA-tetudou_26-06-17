void APSTA(){
WiFi.mode(WIFI_STA);
//WiFi.mode(WIFI_AP_STA);///
  /*
  WiFi.softAP("esp-AP181", "12345678");
  const IPAddress ip(192, 168, 4, 181);
  const IPAddress gateway(192, 168, 4, 1);
  const IPAddress netmask(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gateway, netmask);

  Serial.println("=== Train (AP181) Started ===");
  Serial.print("AP SSID: "); Serial.println("esp-AP181");
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  Serial.print("AP MAC: ");  Serial.println(WiFi.softAPmacAddress());
  */
}