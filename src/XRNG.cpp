#include "XRNG.h"
#include <Arduino.h>
#include <WiFi.h>
#include <Crypto.h>
#include <SHA256.h>
#include "RNG.h"

#define NOISE_NOT_CALIBRATING 0
#define NOISE_CALIBRATING	  1

XRNG::XRNG(): calibratingState(NOISE_CALIBRATING) {
	pinMode(ADC_PIN, INPUT);
	analogSetPinAttenuation(ADC_PIN, ADC_2_5db);
	digitalWrite(ADC_PIN, LOW);
}

XRNG::~XRNG() {
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);
	WiFi.persistent(false);
}

void XRNG::added()
{
	//Serial.print("lets go");
	if (WiFi.getMode() == WIFI_MODE_NULL) {
		WiFi.mode(WIFI_AP);
	}

	delay(20);
}

bool XRNG::calibrating() const
{
	return calibratingState != NOISE_NOT_CALIBRATING;
}

bool XRNG::getRandomBit()
{
	while(true)
	{
		bool bit1, bit2;
		bit1 = 0x1 & analogRead(ADC_PIN);
		
		//https://espressif-docs.readthedocs-hosted.com/projects/esp-faq/en/latest/software-framework/peripherals/adc.html
		delayMicroseconds(1500);

		bit2 = 0x1 & analogRead(ADC_PIN);
		
		if(bit1 != bit2)
		{
			//Serial.print('+');
			return bit1;
		}
	
		delayMicroseconds(1500);
	}
}

uint8_t XRNG::getRandomByte() {
	uint8_t rnd = 0;
	for (uint8_t i = 0; i < 8; ++i) {
		rnd |= (getRandomBit() << i);
	}

	return rnd;
}

void XRNG::stir() {
	calibratingState = NOISE_CALIBRATING;
	uint8_t buffer[32] = {0};
	uint8_t random[POOL_SIZE] = {0};
	SHA256 sha256;

	esp_fill_random(random, POOL_SIZE);

	for(int i = 0; i < POOL_SIZE; i++)
	{
		pool[i] ^= random[i];
	}
	sha256.update(pool, sizeof(POOL_SIZE));

	uint8_t bytes[32] = {0};
	for(int i = 0; i < 32; i++)
	{
		bytes[i] = getRandomByte();
	}
	sha256.update(bytes, 32);

	sha256.finalize(buffer, 32);

	output(buffer, 32, 128);
	calibratingState = NOISE_NOT_CALIBRATING;
}