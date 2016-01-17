#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

/*
 +-----+-----+---------+------+---+-Model B2-+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5V      |     |     |
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | ALT0 | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | ALT0 | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |  OUT | 1 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI | ALT0 | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO | ALT0 | 0 | 21 || 22 | 0 | OUT  | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK | ALT0 | 0 | 23 || 24 | 1 | OUT  | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 0 | OUT  | CE1     | 11  | 7   |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |  28 |  17 | GPIO.17 |   IN | 0 | 51 || 52 | 0 | IN   | GPIO.18 | 18  | 29  |
 |  30 |  19 | GPIO.19 |   IN | 0 | 53 || 54 | 0 | IN   | GPIO.20 | 20  | 31  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+-Model B2-+---+------+---------+-----+-----+
 */

/* =========== SPI & GPIO configuration ========= */
#define SPI_SPEED 250000
#define SPI_CHANNEL 1
#define SPI_CSN 6 // BCM 25
#define SPI_CE  3 // BCM 22
#define SPI_IRQ 1 // BCM 18

/* =========== SPI and GPIO function ============ */
void setup_io();
void setup_spi();
void setCSN(uint8_t value);
void setCE(uint8_t value);
uint8_t transfer_spi(uint8_t tx_);
void pabort(const char *s);
void dataReceivedIRQ();

/* =========== TIME function and variables ====== */
#ifdef	__cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <time.h>
#include <sys/time.h>

// added attribute unused to avoid compiler warnings
static struct timeval start __attribute__ ((unused)) ,end __attribute__ ((unused));

static long __attribute__ ((unused)) mtime;
static long __attribute__ ((unused)) seconds;
static long __attribute__ ((unused)) useconds;

void __msleep(int32_t milisec);
void __usleep(int32_t micros);
void __start_timer();
long __millis();

#ifdef	__cplusplus
}
#endif
