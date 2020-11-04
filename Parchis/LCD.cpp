/**
	@file LCD.cpp
	@author Sebastià A. Noceras Anderson
	@date 06/03/2020
	@brief LCD library.
 */
 
#include "LCD.h"
#include "keypad.h"

/*******************************************************************************
 ** Initializations
 ******************************************************************************/
 
LCD::LCD(
	uint8_t pin_rs, uint8_t pin_rw, uint8_t pin_en,
	uint8_t pin_db4, uint8_t pin_db5, uint8_t pin_db6, uint8_t pin_db7){
	// save pins
	this->pin_rs = pin_rs;
  this->pin_rw = pin_rw;
  this->pin_en = pin_en;
  
  pin_db[0] = pin_db4;
  pin_db[1] = pin_db5;
  pin_db[2] = pin_db6;
  pin_db[3] = pin_db7;
  
	pinMode(pin_rs, OUTPUT);
	pinMode(pin_rw, OUTPUT);
	pinMode(pin_en, OUTPUT); digitalWrite(pin_en, LOW);
}

void LCD::init()
{
	/* See page 46 */
	
	delay(15);

	// Function set: 8 bits
	write4bits(0b0011);
	delayMicroseconds(5000);

	// Function set: 8 bits
	write4bits(0b0011);
	delayMicroseconds(100);

	// Function set: 8 bits
	write4bits(0b0011);
	wait();

	// Function set: 4 bits
	write4bits(0b0010);
	wait();

	// Funtion set: 4 bits, 2 lines and character font 5x8
	send(INSTR, 0b00101000);

	// Display off
	off();

	// Display clear
	clear();

	// Entry mode set: Cursor move direction increment and no shifting
	send(INSTR, 0b00000110);
	
	
	/* INITIALIZATION FINISHED */
	
	// Display on
	on(CURSOR_ON, BLINK_ON);
}


/*******************************************************************************
 ** Print
 ******************************************************************************/
 
void LCD::print(const char* text)
{
	while(*text != '\0'){
		send(DATA, *text);
		wait();
		text++;
	}
}


/*******************************************************************************
 ** Display control
 ******************************************************************************/
 
void LCD::on(lcd_cursor_state_t cursor_state, lcd_blink_state_t blink_state)
{
  if (cursor_state == CURSOR_OFF && blink_state == BLINK_OFF) send(INSTR,0b00001100);
  else if (cursor_state == CURSOR_OFF && blink_state == BLINK_ON) send(INSTR,0b00001101);
  else if (cursor_state == CURSOR_ON && blink_state == BLINK_OFF) send(INSTR,0b00001110);
  else if (cursor_state == CURSOR_ON && blink_state == BLINK_ON) send(INSTR,0b00001111);
}

void LCD::off()
{
  send(INSTR,0b00001000);
}

void LCD::clear()
{
  send(INSTR,0b00000001);
}


/*******************************************************************************
 ** Cursor control
 ******************************************************************************/
 
void LCD::setCursor(uint8_t row, uint8_t col)
{
  uint8_t value; 
  if (row == 1) value = 0b10000000 + col - 1;
  else if(row == 2) value = 0b11000000 + col - 1;
  send(INSTR,value);
}

void LCD::moveCursorRight()
{
  send(INSTR,0b00010100);
}

void LCD::moveCursorLeft()
{
  send(INSTR,0b00010000);
}


/*******************************************************************************
 ** Shift control
 ******************************************************************************/
 
void LCD::shiftDisplayRight()
{
  send(INSTR,0b00011000);
}

void LCD::shiftDisplayLeft()
{
  send(INSTR,0b00011100);
}


/******************************************************************************/
/** PRIVATE FUNCTIONS                                                        **/
/******************************************************************************/

void LCD::write4bits(uint8_t value)
{
	// Set data pins as output
	pinMode(pin_db[0], OUTPUT);
  pinMode(pin_db[1], OUTPUT);
  pinMode(pin_db[2], OUTPUT);
  pinMode(pin_db[3], OUTPUT);

	// Set RS and RW to send instruction
	digitalWrite(pin_rs, LOW);
  digitalWrite(pin_rw, LOW);
	
	// Write value to data pins
	uint8_t mask = 1;
 
  for (uint8_t i = 0; i < 4 ; i++){
      
    if((mask & value) == 0) digitalWrite(pin_db[i], HIGH);//AND BIT A BIT
    else digitalWrite(pin_db[i], LOW);
    mask = mask << 1; 
  }
	enable_pulse();
}

void LCD::send(lcd_send_mode_t mode, uint8_t value)
{
	// Set data pins as output
  pinMode(pin_db[0], OUTPUT);
  pinMode(pin_db[1], OUTPUT);
  pinMode(pin_db[2], OUTPUT);
  pinMode(pin_db[3], OUTPUT);


	// Set RS and RW to send instruction/data depending on mode
	if      (mode == INSTR) digitalWrite(pin_rs, 0);
  else if (mode == DATA ) digitalWrite(pin_rs, 1);
  digitalWrite(pin_rw, 0);

  uint8_t mask = 1;
  uint8_t value_bits[8];
 
  for (uint8_t i=0; i<8 ; i++){
    if((mask & value) == 0) value_bits[i] = LOW;
    else value_bits[i] = HIGH;
    mask = mask << 1;   //Añade un cero a la derecha. 
  }
  
	// Write first nibble (most significant) from value to data pins
  digitalWrite(pin_db[0], value_bits[4]);
  digitalWrite(pin_db[1], value_bits[5]);
  digitalWrite(pin_db[2], value_bits[6]);
  digitalWrite(pin_db[3], value_bits[7]);

	enable_pulse();

	// Write second nibble (less significant) from value to data pins
  digitalWrite(pin_db[0], value_bits[0]);
  digitalWrite(pin_db[1], value_bits[1]);
  digitalWrite(pin_db[2], value_bits[2]);
  digitalWrite(pin_db[3], value_bits[3]);

	enable_pulse();

	wait();
}

void LCD::wait()
{
	uint8_t busy;

	// Set data pins as input
	pinMode(pin_db[0], INPUT);
  pinMode(pin_db[1], INPUT);
  pinMode(pin_db[2], INPUT);
  pinMode(pin_db[3], INPUT);

	// Set RS and RW to read busy flag
	digitalWrite(pin_rs, 0);
  digitalWrite(pin_rw, 1);

	do {
		digitalWrite(pin_en, HIGH);
		
		// Read busy flag (db7)
		busy = digitalRead(pin_db[3]); 
		
		enable_pulse();
	} while (busy == HIGH);
}

void LCD::enable_pulse()
{
	digitalWrite(pin_en, LOW);
	delayMicroseconds(1);
	digitalWrite(pin_en, HIGH);
	delayMicroseconds(1);
	digitalWrite(pin_en, LOW);
	delayMicroseconds(100);
}
