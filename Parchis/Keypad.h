/**
	@file Keypad.h
	@author Sebastià A. Noceras Anderson
	@date 02/03/2020
	@brief Keypad library.
 */

#include <Arduino.h>
#include <stdint.h>

#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_NO_KEY 255

typedef enum keypad_block {BLOCK, NO_BLOCK} keypad_block_t;

class Keypad {
public:
	Keypad();

	// Initializations
	void initKeypad(
		uint8_t pin_c0, uint8_t pin_c1, uint8_t pin_c2, uint8_t pin_c3,
		uint8_t pin_r0, uint8_t pin_r1, uint8_t pin_r2, uint8_t pin_r3
	);

	void initLeds(
		uint8_t pin_l0, uint8_t pin_l1, uint8_t pin_l2, uint8_t pin_l3,
		uint8_t pin_l4, uint8_t pin_l5, uint8_t pin_l6, uint8_t pin_l7
	);

	// Keypad
	uint8_t readKeypad(keypad_block_t block);

	// LEDs
	void onLed  (uint8_t num_led);
	void offLed (uint8_t num_led);

private:
  
   uint8_t pinsKeypadCol[4];
   uint8_t pinsKeypadRow[4];
   uint8_t pinsLed[8];
};

#endif /* KEYPAD_H */
