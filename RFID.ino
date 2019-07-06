#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define OLED_RESET 4

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

int code[] = {153,250,204,89}; //This is the stored UID
int codeRead = 0;
String uidString;
void setup() {
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  pinMode(8,INPUT);
  pinMode(7,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
}

void loop() {
  int switchState = digitalRead(8);
  if(switchState==HIGH)
  {
    int s = 0;
    //red
    digitalWrite(7,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(2,LOW);

    while(1){
    if(  rfid.PICC_IsNewCardPresent())
    {
      bool recognized = readRFID();
      if(recognized == true){
      //green
      digitalWrite(4,HIGH);
      digitalWrite(7,LOW);
      digitalWrite(2,LOW);
      break;
      }
      else
      {
        //piezo
        s=s+1;
        if(s>=3){
        digitalWrite(2,HIGH);
        }
      }
    }
        delay(400);
   }

  }

}

bool readRFID()
{
  
  rfid.PICC_ReadCardSerial();
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }


    uidString = String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]);
    
    int i = 0;
    boolean match = true;
    while(i<rfid.uid.size)
    {
      if(!(rfid.uid.uidByte[i] == code[i]))
      {
           match = false;
      }
      i++;
    }
    digitalWrite(2,HIGH);
    delay(50);
    digitalWrite(2,LOW);
    if(match)
    {
      return true;
    }else
    {
      return false;
    }


    // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
 
