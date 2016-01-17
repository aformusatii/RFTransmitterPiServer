/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include "HardwarePlatform.h"

/* ======================================================= */
void HardwarePlatform::initIO() {
	setup_io();
}

void HardwarePlatform::initSPI() {
	setup_spi();
}

void HardwarePlatform::csn(uint8_t value) {
	setCSN(value);
}

void HardwarePlatform::ce(uint8_t value) {
	setCE(value);
}

uint8_t HardwarePlatform::spiTransfer(uint8_t tx_) {
	return transfer_spi(tx_);
}

void HardwarePlatform::delayMicroseconds(uint64_t micros) {
	__usleep(micros);
}

void HardwarePlatform::delayMilliseconds(uint64_t milisec) {
	__msleep(milisec);
}

void HardwarePlatform::startTimer() {
	__start_timer();
}

uint64_t HardwarePlatform::getElapsedMilliseconds() {
	return __millis();
}
