#ifndef YM_H
#define YM_H

/* Dependencies */
#include <avr/io.h>     // For I/O and other AVR registers
#include <util/delay.h> // For timing

/* Pinmap (Arduino UNO compatible) */
#define YM_IC (5) // PC5 (= pin A5 for Arduino UNO)
#define YM_CS (0) // PC4 (= pin A4 for Arduino UNO)
#define YM_WR (1) // PC3 (= pin A3 for Arduino UNO)
#define YM_RD (2) // PC2 (= pin A2 for Arduino UNO)
#define YM_A0 (3) // PC1 (= pin A1 for Arduino UNO)
#define YM_A1 (4) // PC0 (= pin A0 for Arduino UNO)
#define YM_CTRL_DDR DDRK
#define YM_CTRL_PORT PORTK
#define YM_DATA_DDR DDRA
#define YM_DATA_PORT PORTA // Whole PORT D for data bus (= pins D0 to D7 for Arduino UNO)
#define YM_MCLOCK (5) // PB1 = OC1A (= pin D9 for Arduino UNO)
#define YM_MCLOCK_DDR DDRB

/**
   Write data into a specific register of the YM2612

   @param reg Destination register address
   @param data Data to write
*/
void setreg_bank(uint8_t reg, uint8_t data, bool bank_2);

//same as setreg_bank but defaults to bank 1
void setreg(uint8_t reg, uint8_t data);

void setup_ym2612();

#endif
