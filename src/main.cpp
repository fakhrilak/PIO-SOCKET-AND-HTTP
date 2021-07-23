#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "httpclient.h"
#include <ArduinoJson.h>
#include "timbangan.h"
#include <SocketIoClient.h>
SocketIoClient Socket;
String token;
const char* ssid     = "STTekno_lt1";
const char* password = "STTekno12345";
String mode="1";
String userID = "60f50845aefb7b6b73a1f636";
StaticJsonDocument<200> doc;

void event(const char * payload, size_t length) {
  Serial.print("ini response socket : ");
  Serial.println(payload);
  mode = String(payload);
}
void event1(const char * payload, size_t length) {
  Serial.print("ini response socket : ");
  Serial.println(payload);
  Serial.println(mode);
  String Data = "{\"id_User\":\""+  userID 
                  +"\",\"mode\":\""+ mode
                  +"\"}";
  Socket.emit("MyStatus",Data.c_str());
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  String payload = httpPostToENDpoint("login",
  "{\"email\":\"fakhrilak23@gmail.com\",\"password\":\"fakhrilak\"}","x");
  Serial.println(payload);
  deserializeJson(doc, payload);
  String tkn = doc["token"];
  token = tkn;
  Socket.begin("192.168.10.120",3004);
  Serial.println("connect?");
  Socket.on("60f50845aefb7b6b73a1f636",event);
  Socket.on("giveStatus60f50845aefb7b6b73a1f636",event1);
}

void loop() {
  // put your main code here, to run repeatedly:
  Socket.loop();
  
  if( mode == "1"){
      String content = getIdTAG();
      if(content !="0"){
        String res_post = httpPostToENDpoint("id-tag",
        "{\"id_tag\":\""+ content +"\"}",token);
        deserializeJson(doc, res_post);
        String message = doc["message"];
        String Dataaa = "{\"User_id\":\""+  userID 
                 +"\",\"message\":\""+ message
                 +"\"}";
        Socket.emit("ResTimbangan",Dataaa.c_str());
      }
  }else if(mode == "0"){
      String content = getIdTAG();
      if(content !="0"){
        String id_timb = "1";
        long bb = random(10,100);
        String res_post = httpPostToENDpoint("weight-got",
         "{\"id_timbangan\":\""+  id_timb 
             +"\",\"id_tag\":\""+ content
             +"\",\"bb\":\""+ String(bb)
             +"\"}",token);
        deserializeJson(doc, res_post);
        String data = doc["data"];
        String message = doc["message"];
        String Dataaa = "{\"User_id\":\""+  userID 
                 +"\",\"message\":\""+ message
                 +"\"}";
        Socket.emit("ResTimbangan",Dataaa.c_str());
      }
  }
  
  delay(500);
}