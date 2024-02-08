#ifndef XRNG_H
#define XRNG_H

#include <Arduino.h>

#include "NoiseSource.h"

#define POOL_SIZE 64
#define ADC_PIN 32

class XRNG: public NoiseSource {
	public:
		explicit XRNG();
		virtual ~XRNG();

		bool calibrating() const;
		
		void added();
		void stir();

	private:
		uint8_t calibratingState;
		bool getRandomBit();
		uint8_t getRandomByte();
		uint8_t pool[POOL_SIZE];
};

#endif