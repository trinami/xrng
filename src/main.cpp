#include <Arduino.h>
#include "XRNG.h"
#include "Crypto.h"
#include "NoiseSource.h"
#include "RNG.h"

#define RNG_STARTVALUE "XKWXZGFZSAXK"

XRNG xrng;

void dumpByteArray(const byte *byteArray, const byte arraySize)
{
	for(uint8_t i = 0; i < arraySize; i++)
	{
		if(byteArray[i] < 0x10)
		{
			Serial.print("0");
		}
			
		Serial.print(byteArray[i], HEX);
	}
}

void setup() {
	delay(2000);
	Serial.begin(115200);

	RNG.begin(RNG_STARTVALUE);
	RNG.addNoiseSource(xrng);

	Serial.println("start collecting...");
	while(!RNG.available(32)){RNG.loop();};
	Serial.println("finished:");
	
	uint8_t data[32] = {0};
	RNG.rand(data, 32);
	dumpByteArray(data, 32);

	while(true){}
}


void loop() {
	
}