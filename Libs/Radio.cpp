#include "Radio.h" //include the declaration for this class
#include <SPI.h>
#include <RH_RF95.h>
#include <RHEncryptedDriver.h>
#include <Speck.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0
 
const byte LED_PIN = 13; //use the LED @ Arduino pin 13, this should not change so make it const (constant)

RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
//<<constructor>> setup the LED, make pin 13 an OUTPUT
Radio::Radio(){
  
}
 
//<<destructor>>
Radio::~Radio(){/*nothing to destruct*/}

void Radio::init(){
	 pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  while (!Serial);
  Serial.begin(9600);
  delay(100);
 
  Serial.println("Arduino LoRa TX Test!");
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(20, false);

   if (!rf95.setModemConfig(1)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  rf95.setHeaderTo(4);
}
 
//turn the LED on
String Radio::send(String radioText){
	char radiopacket[20];
    radioText.toCharArray(radiopacket, 20); 
	rf95.send((uint8_t *)radiopacket, 20);
	rf95.waitPacketSent();
  
  // Now wait for a reply
	uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);
  
  
	if (rf95.waitAvailableTimeout(1000))
	{ 
		// Should be a reply message for us now   
		if (rf95.recv(buf, &len))
		{
		String reply = (char*)buf; 
		return reply;
		}
		else
		{
		  return("Receive failed");
		}
	  }
	  else
	  {
		return("No reply, is there a listener around?");
	  }
    
  		
}
void Radio::setChannel(int channel){
        digitalWrite(LED_PIN,LOW); //set the pin LOW and thus turn LED off
}

int Radio::rssi(){
	String radioText = "Ping";
	char radiopacket[20];
    radioText.toCharArray(radiopacket, 20); 
	rf95.send((uint8_t *)radiopacket, 20);
	rf95.waitPacketSent();
  
  // Now wait for a reply
	uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);
  
  
	if (rf95.waitAvailableTimeout(1000))
	{ 
		// Should be a reply message for us now   
		if (rf95.recv(buf, &len))
		{
		int rssi = rf95.lastRssi();
		return rssi;	
		}
		else
		{
		  Serial.println("Receive failed");
		}
	  }
	  else
	  {
		Serial.println("No reply, is there a listener around?");
	  }
    
  		
}

String Radio:: read(){
	uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);
	
	if(rf95.recv(buf,len) {
		rf95.send(buf,len);
		rf95.waitPacketSent();
		return (char*) buf;
	}
	
}

