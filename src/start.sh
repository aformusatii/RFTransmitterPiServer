#!/bin/bash
cd "$(dirname "$0")"

sudo ./transmitter > transmitter.log 2>&1 &
ps aux | grep transmitter
echo Started NRF24L01 transmitter.
