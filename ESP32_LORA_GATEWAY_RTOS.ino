/*
 * 3.3V 3.3V
GND GND
NSS D5
DIO0  D2
SCK D18
MISO  D19
MOSI  D23
RST D14
 */




 //#include <avr/wdt.h>
int count=0;
#include <SPI.h>
#include <LoRa.h>

//#include <Arduino_FreeRTOS.h>
//#include <queue.h>
//#include <semphr.h>
// Define a Array
char pinReadArray[100];
char reads[100];
#define ss 5
#define rst 14
#define dio0 2
#define LED_BUILTIN 3

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );
QueueHandle_t arrayQueue;
SemaphoreHandle_t mutex;
// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
      pinMode(LED_BUILTIN, OUTPUT);
 digitalWrite(LED_BUILTIN, LOW);
 LoRa.setPins(ss, rst, dio0);
 
  arrayQueue=xQueueCreate(20, //Queue length
                        10);

  mutex = xSemaphoreCreateMutex();
   if (mutex != NULL) {
    Serial.println("Mutex created");
  }
                      
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }


  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
  }
   LoRa.setSpreadingFactor(10);
 // LoRa.setSignalBandwidth(62.5E3);
  LoRa.crc();
  // Now set up two tasks to run independently.

  xTaskCreate(
    TaskBlink
    ,  "Blink"   // A name just for humans
    ,  1024  //This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskAnalogRead
    ,  "AnalogRead"
    ,  1024// Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  
  
  //
  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
Serial.print("TASK1");
  for (;;) // A Task shall never return or exit.
  {
    Serial.print("queue  search");
    if(xQueueReceive(arrayQueue,&reads,portMAX_DELAY) == pdPASS ){
      xSemaphoreTake(mutex, 1) ;
      Serial.print("\n queue data:");
      Serial.println(reads);
    
 LoRa.beginPacket();
  if(reads[0]=='E')
  {
  LoRa.print("esp ACK");
  Serial.print("esp ACK");
  }
 else
 if(reads[0]=='A')
  {LoRa.print("uno ACK");
  Serial.print("uno ACK");
  }
  else
  LoRa.print("not ACK");
  LoRa.endPacket();
  xSemaphoreGive(mutex);
      vTaskDelay(50);
    
  }
  vTaskDelay(10);
  }
}

void TaskAnalogRead(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    
 
 if (xSemaphoreTake(mutex, 1) == pdTRUE)
    {   
    // read the input on analog pin 0:
//Serial.println("reciever mode start");

int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("ARDUINO NANO Received packet '");
 

    int i=0;
    char s,datas[30];
    while (LoRa.available()) {
   s=(char)LoRa.read();
   
      datas[i++]=s;
  //    Serial.print(s);
   
    }
   // i--;
    datas[i]='\0';
Serial.print(datas);
 xQueueSend(arrayQueue,datas,portMAX_DELAY);
  // One tick delay (15ms) in between reads for stability
  vTaskDelay(30);
    // print RSSI of packet
  //  Serial.print("' with RSSI ");
 //   Serial.println(LoRa.packetRssi());



    
  }
  vTaskDelay(2); 
//Serial.println("reciever mode end");
xSemaphoreGive(mutex);
   
  }
  }
}
