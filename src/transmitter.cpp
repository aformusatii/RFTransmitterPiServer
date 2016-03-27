#include "../nrf24l01/RF24.h"
#include "../misc/Server.h"

void *data_handler(char[], int);

SocketServer socketServer;
RF24 radio;
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
volatile uint8_t rf_channel = 115;

int main(int argc, char** argv)
{

  printf("Start Pi Smar Home Sender Server...");

  // Init Socket Server
  socketServer.addDataListner(data_handler);
  socketServer.init(8888);

  // Init NRF2401+
  radio.begin();
  radio.setRetries(15,15);
  radio.setPayloadSize(8);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(rf_channel);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  
  radio.printDetails();
  radio.stopListening();

  fflush(stdout);
  
  while (1) {
	  sleep(5000);
  }

  return 0;
}

void dataReceivedIRQ() {
}

void *data_handler(char data[], int size) {
	if ((size > 1) && rf_channel != data[0]) {
		radio.setChannel(rf_channel);
	}

	radio.write(data, size);
	radio.stopListening();
	fflush(stdout);

	return 0;
}
