g++ -lwiringPi -fpermissive -lpthread -I. -I ../nrf24l01 -I ../misc -o transmitter ../misc/Server.cpp ../nrf24l01/RF24.cpp ../nrf24l01/HardwarePlatform.cpp ../nrf24l01/RaspberryPi.cpp transmitter.cpp
