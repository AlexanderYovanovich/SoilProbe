//load on 84A

#include <RH_RF69.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define LED 0

// Singleton instance of the radio driver
RH_RF69 rf69(1, 8);

void strobe(int milliseconds){
	digitalWrite(LED, HIGH);
	delay(milliseconds/2);
	digitalWrite(LED, LOW);
	delay(milliseconds/2);
}

void setup() 
{
  pinMode(LED, OUTPUT);
  strobe(2000);
  if (!rf69.init())
	while(true){
		//strobe(25);
	}
  if (!rf69.setFrequency(915.0))
    while(true){
		//strobe(200);
	}
 
  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(17, true);
 
  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  digitalWrite(LED, HIGH);
}
 
 
void loop()
{
  
  // Send a message to rf69_server
  uint8_t data[] = "Hello World!";
  rf69.send(data, sizeof(data));
  
  rf69.waitPacketSent();
  rf69.setModeRx();
  // Now wait for a reply
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  
  
  if (rf69.waitAvailableTimeout(500))
  { 
    // Should be a reply message for us now   
    if (rf69.recv(buf, &len))
    {
	    uint8_t data[] = "Thanks!";
		rf69.send(data, sizeof(data));
    }
    else
    {
      strobe(50);
    }
  }
  else
  {
	uint8_t data[] = "Didn't hear you.";
	rf69.send(data, sizeof(data));
  }
  delay(400);
}