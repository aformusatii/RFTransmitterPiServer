/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include "../nrf24l01/RF24.h"
#include "../misc/Server.h"

void *data_handler(char[], int);

SocketServer socketServer;
RF24 radio;
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

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
  radio.setChannel(115);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  
  radio.printDetails();
  radio.stopListening();

  while (1) {
	  sleep(1000);
  }

  return 0;
}

void dataReceivedIRQ() {
}

void *data_handler(char data[], int size) {
	bool ok = radio.write(data, size);

	if (ok)
	{
		printf("ok...\n\r");
	}
	else
	{
		printf("failed.\n\r");
	}

	radio.stopListening();

	printf("\n");

        for (int i = 0; i < size; i++) {
		printf("[%d]", data[i]);
        }

	fflush(stdout);
	return 0;
}
