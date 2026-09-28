#include <WiFi.h>
#include <IRremote.h>
#include <WebServer.h>
#include <HTTPClient.h>


long ir_val=0;
WebServer server(80);
String UID="3632305610426657";  //B站ID
String followerUrl="http://api.bilibili.com/x/relation/stat?vmid="+UID;//B站粉丝数
long follower=0;
//DynamicJsonDocument doc(1024);

void handleRoot(){
  String HTML="<!DOCTYPE html><html><head><meta charset='utf-8'></head><body>你好，很高兴见到你,小试一下。</body></html>";
  server.send(200,"text/html",HTML);
}


IRrecv irrecv_15(15);
decode_results results_15;


void setup(){
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  pinMode(2, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(12,LOW);
  digitalWrite(13,LOW);
  digitalWrite(14,LOW);
  WiFi.begin("93502","hyf93502");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP地址：");
  Serial.println(WiFi.localIP());
  
  server.on("/",handleRoot);
  
  server.on("/hello",[](){server.send(200,"text/html","hello01");});
  
  server.onNotFound([](){server.send(200,"text/html;charset=utf-8","没有找到页面！");});
  
  server.begin();
  
  irrecv_15.enableIRIn();
}

void loop(){
  server.handleClient();
  if (irrecv_15.decode(&results_15)) {
    ir_val=results_15.value;
    String type="UNKNOWN";
    String typelist[14]={"UNKNOWN", "NEC", "SONY", "RC5", "RC6", "DISH", "SHARP", "PANASONIC", "JVC", "SANYO", "MITSUBISHI", "SAMSUNG", "LG", "WHYNTER"};
    if(results_15.decode_type>=1&&results_15.decode_type<=13){
      type=typelist[results_15.decode_type];
    }
    Serial.print("IR TYPE:"+type+"  ");
    Serial.println(ir_val,HEX);
    irrecv_15.resume();
  } else {
  }
  // 如果按0，全部熄灭。
  if (ir_val == 0XFF9867) {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    digitalWrite(14,LOW);

  }
  // 如果按1，绿灯亮。
  if (ir_val == 0XFFA25D) {
    digitalWrite(12,HIGH);

  }
  // 如果按2，蓝灯亮。
  if (ir_val == 0XFF629D) {
    digitalWrite(13,HIGH);

  }
  // 如果按3，红灯亮。
  if (ir_val == 0XFFE21D) {
    digitalWrite(14,HIGH);

  }
  // 如果按3，红灯亮。
  if (ir_val == 0XFF22DD) {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    digitalWrite(14,LOW);

  }
  // 如果按3，红灯亮。
  if (ir_val == 0XFFC23D) {
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);
    digitalWrite(14,LOW);

  }
  // 如果按4，关灯。
  if (ir_val == 0XFF22DD) {
    digitalWrite(2,LOW);
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    digitalWrite(14,LOW);
    
  }

}