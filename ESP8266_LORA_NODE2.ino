/*
NodeMCU Pins                         SX1278 Pins
GND                                       GND
3.3V                                      VCC
D8                                        NSS
D7                                        MOSI
D6                                        MISO
D5                                        SCK
D0                                        RST
 */

#include <SPI.h>
#include <LoRa.h>
#define ss 15
#define rst 16
#define dio0 3
#define LED_BUILTIN 2
int counter;
//#define ss 5
//#define rst 14
//#define dio0 2
void setup() {
  Serial.begin(9600);
  while (!Serial);
pinMode(LED_BUILTIN,OUTPUT);
digitalWrite(2,LOW);
  Serial.println("LoRa Receiver");
 LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
     LoRa.setSpreadingFactor(10);
 // LoRa.setSignalBandwidth(62.5E3);
  LoRa.crc();
}


void loop()
{
Serial.print("arduino Sending packet: E-DATA ");
 // Serial.println(counter);
Serial.print(counter);
  // send packet
  LoRa.beginPacket();
  LoRa.print("E-DATA");
  LoRa.print(counter);
//char s=int_to_char(counter);
  //LoRa.print(int_to_char(counter));
 // LoRa.print("\0");
  LoRa.endPacket();
 //digitalWrite(LED_BUILTIN, HIGH); 

int j=0;
char datas[30];
  while(j<750)
  {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("arduino Received packet '");
 //digitalWrite(LED_BUILTIN, HIGH); 
    // read packet

    int i=0;
    char s;
    while (LoRa.available()) {
   s=(char)LoRa.read();
   
      datas[i++]=s;
  //    Serial.print(s);
   
    }
   // i--;
    datas[i]='\0';
  if (strcmp(datas,"esp ACK") == 0 )
 { 
  digitalWrite(LED_BUILTIN, HIGH); 
  counter++;
  
 }
 else
 delay(7000);   
Serial.print(datas);

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    delay(2000);
    break;
  }
delay(4);
j++;
  }
//digitalWrite(LED_BUILTIN,LOW); 
 delay(2000);
 digitalWrite(LED_BUILTIN,LOW);
  //counter++;
  //datas[0]="k";
}
