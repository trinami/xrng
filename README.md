# XRNG - Everything starts with a secure randomness

Combines the ESP32 Hardware RNG and collects randomness from ADC values.

Both entropy sources are hashed with SHA512 to a pool.
On call, only the half of the pool is derived.

Compatible with Arduino [Crypto](http://rweather.github.io/arduinolibs/crypto.html) lib