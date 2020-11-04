/**
	@file Keypad.cpp
	@author Sebastià A. Noceras Anderson
	@date 02/03/2020
	@brief Keypad library.
 */
 
#include "Keypad.h"

Keypad::Keypad() {}


/*******************************************************************************
 ** Initializations
 ******************************************************************************/

void Keypad::initKeypad(
	uint8_t pin_c0, uint8_t pin_c1, uint8_t pin_c2, uint8_t pin_c3,
	uint8_t pin_r0, uint8_t pin_r1, uint8_t pin_r2, uint8_t pin_r3)
	{
	pinsKeypadCol[0] = pin_c0;
	pinsKeypadCol[1] = pin_c1;
	pinsKeypadCol[2] = pin_c2;
	pinsKeypadCol[3] = pin_c3;
	
	pinsKeypadRow[0] = pin_r0;
	pinsKeypadRow[1] = pin_r1;
  pinsKeypadRow[2] = pin_r2;
	pinsKeypadRow[3] = pin_r3;
	
	pinMode(pinsKeypadCol[0], OUTPUT);
	pinMode(pinsKeypadCol[1], OUTPUT);
	pinMode(pinsKeypadCol[2], OUTPUT);
	pinMode(pinsKeypadCol[3], OUTPUT);
	
	pinMode(pinsKeypadRow[0], INPUT_PULLUP);
	pinMode(pinsKeypadRow[1], INPUT_PULLUP);
	pinMode(pinsKeypadRow[2], INPUT_PULLUP);
	pinMode(pinsKeypadRow[3], INPUT_PULLUP);
	
}

void Keypad::initLeds(
	uint8_t pin_l0, uint8_t pin_l1, uint8_t pin_l2, uint8_t pin_l3,
	uint8_t pin_l4, uint8_t pin_l5, uint8_t pin_l6, uint8_t pin_l7)
{
	pinsLed[0] = pin_l0;
	pinsLed[1] = pin_l1;
	pinsLed[2] = pin_l2;
	pinsLed[3] = pin_l3;
	pinsLed[4] = pin_l4;
	pinsLed[5] = pin_l5;
	pinsLed[6] = pin_l6;
	pinsLed[7] = pin_l7;
	
	for (uint8_t i = 0; i < 8; i++)
	{
		pinMode(pinsLed[i], OUTPUT);
		digitalWrite(pinsLed[i], HIGH);
	}
}


/*******************************************************************************
 ** Keypad
 ******************************************************************************/

uint8_t Keypad::readKeypad(keypad_block_t block)
{
	 uint8_t col;
   uint8_t row;
   uint8_t key = 0; 

    
   while(true){
     digitalWrite(pinsKeypadCol[0],HIGH);
     digitalWrite(pinsKeypadCol[1],HIGH);
     digitalWrite(pinsKeypadCol[2],HIGH);
     digitalWrite(pinsKeypadCol[3],HIGH);

     key = 0;
     for (col = 0; col <= 3; col++){
       if (col > 0){
            digitalWrite (pinsKeypadCol[col - 1], HIGH);
            delay(10);
       }
       digitalWrite (pinsKeypadCol[col], LOW);
       delay(10);
       for (row = 0; row <= 3; row++){
         if (digitalRead(pinsKeypadRow[row]) == LOW){ 
           key = col + row * 4;
           return key;
         }
       }
    }
    if (block == NO_BLOCK) return KEYPAD_NO_KEY;    
  }
}


/*******************************************************************************
 ** LEDs
 ******************************************************************************/

void Keypad::onLed (uint8_t num_led)
{
	digitalWrite(pinsLed[num_led], LOW);
}

void Keypad::offLed(uint8_t num_led)
{
	digitalWrite(pinsLed[num_led], HIGH);
}
