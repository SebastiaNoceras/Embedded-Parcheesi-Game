/**
	@file LCD.h
	@author Sebastià A. Noceras Anderson
	@date 06/03/2020
	@brief LCD library.
 */
 
#include <Arduino.h>
#include <stdint.h>

#ifndef LCD_H
#define LCD_H

typedef enum {CURSOR_ON, CURSOR_OFF} lcd_cursor_state_t;
typedef enum {BLINK_ON,  BLINK_OFF } lcd_blink_state_t;

typedef enum {INSTR, DATA} lcd_send_mode_t;

class LCD {
public:
	// Initializations
	LCD(uint8_t pin_rs, uint8_t pin_rw, uint8_t pin_en,
		uint8_t pin_db4, uint8_t pin_db5, uint8_t pin_db6, uint8_t pin_db7);
		
	void init();

	// Print
	void print(const char* text);

	// Display control
	void on(lcd_cursor_state_t cursor_state, lcd_blink_state_t blink_state);
	void off();
	
	void clear();

	// Cursor control
  
	void setCursor(uint8_t row, uint8_t col);
	void moveCursorRight();
	void moveCursorLeft();

	// Shift control
	void shiftDisplayRight();
	void shiftDisplayLeft();

private:
	void write4bits(uint8_t value);

	void send(lcd_send_mode_t mode, uint8_t value);
	void wait();
	
	void enable_pulse();

	uint8_t pin_rs; // LOW: instruction.  HIGH: data.
	uint8_t pin_rw; // LOW: write to LCD.  HIGH: read from LCD.
	uint8_t pin_en; // activated by a HIGH pulse.
	
	uint8_t pin_db[4];
	
};

#endif
