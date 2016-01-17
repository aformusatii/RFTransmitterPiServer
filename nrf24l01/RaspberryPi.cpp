#include "RaspberryPi.h"

volatile int fd = 0;

/* ======================================================= */
// Set up a memory regions to access GPIO
void setup_io()
{
   printf("Initialize GPIO...\n");
   wiringPiSetup();

   pinMode (SPI_CSN, OUTPUT);
   pinMode (SPI_CE, OUTPUT);
   pinMode (SPI_IRQ, INPUT);

   if (wiringPiISR (SPI_IRQ, INT_EDGE_FALLING, &dataReceivedIRQ) < 0) {
     pabort("Unable to setup ISR");
   }

} // setup_io

/* ======================================================= */
void setCSN(uint8_t value)
{
	if (value) {
		digitalWrite(SPI_CSN, HIGH);
	} else {
		digitalWrite(SPI_CSN, LOW);
	}
}

/* ======================================================= */
void setCE(uint8_t value)
{
	if (value) {
		digitalWrite(SPI_CE, HIGH);
	} else {
		digitalWrite(SPI_CE, LOW);
	}
}

/* ======================================================= */
// Set up SPI interface
void setup_spi()
{
	printf("Initialize SPI interface...\n");
	fd = wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);

	if (fd < 0) {
		pabort("can't initialize SPI interface");
	}
} // setup_spi

/* ======================================================= */
// SPI transfer
uint8_t transfer_spi(uint8_t tx_)
{
	// One byte is transfered at once
	uint8_t rw_buffer[] = {tx_};
	int ret = wiringPiSPIDataRW(SPI_CHANNEL, rw_buffer, 1);
	if (ret < 1) {
		perror("can't send spi message");
		abort();
	}

	return rw_buffer[0];
} // transfer_spi

/* ======================================================= */
void __msleep(int32_t milisec)
{
	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = milisec * 1000000L;
	nanosleep(&req, (struct timespec *)NULL);
}

/* ======================================================= */
void __usleep(int32_t micros)
{
	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = micros * 1000L;
	nanosleep(&req, (struct timespec *)NULL);
}

/* ======================================================= */
void __start_timer()
{
	gettimeofday(&start, NULL);
}

/* ======================================================= */
long __millis()
{
	gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	return mtime;
}

/* ======================================================= */
void pabort(const char *s)
{
    perror(s);
    abort();
}

