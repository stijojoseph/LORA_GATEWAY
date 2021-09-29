/*
 * 3.3V 3.3V
Gnd Gnd
En/Nss  D10
G0/DIO0 D2
SCK D13
MISO  D12
MOSI  D11
RST D9
 */



#include <SPI.h>
#include <LoRa.h>
int k=0;
int counter = 0;
#define LED_BUILTIN 7
void setup() {
  Serial.begin(9600);
  // pinMode(LED_BUILTIN, OUTPUT);
// digitalWrite(LED_BUILTIN, LOW);
  while (!Serial);

  Serial.println("LoRa Sender");

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
Serial.print("arduino Sending packet: A-DATA ");
 // Serial.println(counter);
Serial.print(counter);
  // send packet
  LoRa.beginPacket();
  LoRa.print("A-DATA");
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
  if (strcmp(datas,"uno ACK") == 0 )
 { counter++;
  
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
  //counter++;
  //datas[0]="k";
}
