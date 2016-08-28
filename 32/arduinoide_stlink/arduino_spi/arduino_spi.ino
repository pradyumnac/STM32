#include "SPI.h"

#define pinLED PC13
#define pinRST PB0
#define pinIRQ PB1

#define pinSPI_SS  PA4  
#define pinSPI_CLK PA5
#define pinSPI_MISO PA6
#define pinSPI_MOSI PA7

volatile int irqCounter = 0;
int lastIrqCounter = 0;

void handleIRQ(){
  irqCounter++;
}

void setup(){  
  pinMode(pinLED, OUTPUT);
  pinMode(pinSPI_SS, OUTPUT);
  pinMode(pinRST, OUTPUT);
  pinMode(pinIRQ, INPUT);  

  Serial.begin(115200);
  Serial.println("START");
  attachInterrupt(pinIRQ, handleIRQ, RISING);

  // Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high. 
  SPI.begin();  

  digitalWrite(pinRST, LOW);
  delay(200);
  digitalWrite(pinRST, HIGH);
  delay(100);
}

void spiTest(){
  unsigned long msg = 0;
  SPI.beginTransaction(SPISettings(16000000L, MSBFIRST, SPI_MODE0));
  digitalWrite(pinSPI_SS, LOW);
  SPI.transfer(0x00);
  msg = SPI.transfer(0x00);
  msg = (msg << 8) + SPI.transfer(0x00);
  msg = (msg << 8) + SPI.transfer(0x00);
  msg = (msg << 8) + SPI.transfer(0x00);  
  digitalWrite(pinSPI_SS, HIGH);
  SPI.endTransaction();  
  Serial.println(msg, HEX);  
}

void loop(){
  digitalWrite(pinLED, HIGH);
  delay(100);
  digitalWrite(pinLED, LOW);
  delay(100);  
  if (irqCounter != lastIrqCounter){
    Serial.println(irqCounter);
    lastIrqCounter = irqCounter;
  } 
  spiTest();
  delay(2000);
}
