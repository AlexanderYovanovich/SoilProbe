#include <SPI.h>
#include <RH_RF69.h>
 
// Singleton instance of the radio driver
//RH_RF95 rf95Client;
int CSS = 5;
int SSS = 4;
int CINTR = 3;
int SINTR = 2;
int CRST = 7;
int SRST = 6;

RH_RF69 rf69Server(CSS, CINTR);
RH_RF69 rf69Client(SSS, SINTR);

void serverSetup() {

	if (!rf69Server.init()){
		Serial.println("init failed");
	} else {

	}
	
	if (!rf69Server.setFrequency(918.0)){
		Serial.println("setFrequency failed");
	}
	
	rf69Server.printRegisters();
}
void serverLoop() {
	if (rf69Server.waitAvailableTimeout(500)) {
		Serial.println("Server: something was recieved");
		// Should be a message for us now   
		
		uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
		uint8_t len = sizeof(buf);
		
		if (rf69Server.recv(buf, & len)) {
			//RH_RF95::printBuffer("request: ", buf, len);
			Serial.print("Server: got request: ");
			Serial.println((char * ) buf);
			//Serial.print("RSSI: ");
			//Serial.println(rf95Server.lastRssi(), DEC);
		
			//Send a reply
			uint8_t data[] = "UWOTM8??";
			rf69Server.send(data, sizeof(data));
			rf69Server.waitPacketSent();
			Serial.println("Server: Sent a reply");
		} else {
			Serial.println("Server: recv failed");
		}
	} else {
		Serial.println("Server: nothing was recieved");
	}
}

void clientSetup(){
	
	if (!rf69Client.init()){
		Serial.println("init failed");
	}else{
		
	}
	
	if (!rf69Client.setFrequency(918.0)){
		Serial.println("setFrequency failed");
	}

	rf69Client.printRegisters();
}
void clientLoop(){
	uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);
		
	
	Serial.println("Client: Sending to rf69_server");
	// Send a message to rf95_server
	uint8_t data[] = "URMUMGAY";
	rf69Server.setModeRx();
	
	rf69Client.send(data, sizeof(data));
	rf69Client.waitPacketSent();
	rf69Client.setModeRx();
	
	serverLoop();
	
	// Now wait for a reply
	if (rf69Client.waitAvailableTimeout(500))
	{
		// Should be a reply message for us now   
		if (rf69Client.recv(buf, &len))
		{
			Serial.print("Client: got reply: ");
			Serial.print((char*)buf);
			Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		}
		else
		{
			Serial.println("Client: recv failed");
		}
	}
	else
	{
		Serial.println("Client: No reply");
	}
	Serial.println("==============NEW_TRANSACT==================");
}

 
void setup() 
{
	Serial.begin(9600);
	while (!Serial) ; // Wait for serial port to be available
	pinMode(CRST, OUTPUT);
	pinMode(SRST, OUTPUT);
	digitalWrite(CRST, LOW);
	digitalWrite(SRST, LOW);
	digitalWrite(CSS, HIGH);
	digitalWrite(SSS, HIGH);
	
	clientSetup();
	serverSetup();
}

void loop()
{
	clientLoop();
}
