#include "timbangan.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);

String getIdTAG(){
SPI.begin();
mfrc522.PCD_Init();
if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return "0";
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return "0";
  }
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  return content;
}