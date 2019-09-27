#include "ym.h"

/**
   Send raw data to the YM2612

   @param data Data to write
*/
void write_ym(uint8_t data) {
  YM_CTRL_PORT &= ~_BV(YM_CS); // CS LOW
  YM_DATA_PORT = data;
  _delay_us(1);
  YM_CTRL_PORT &= ~_BV(YM_WR); // Write data
  _delay_us(5);
  YM_CTRL_PORT |= _BV(YM_WR);
  _delay_us(5);
  YM_CTRL_PORT |= _BV(YM_CS); // CS HIGH
}

/**
   Write data into a specific register of the YM2612

   @param reg Destination register address
   @param data Data to write
*/
void setreg_bank(uint8_t reg, uint8_t data, bool bank_2) {
  YM_CTRL_PORT &= ~_BV(YM_A0); // A0 low (select register)

  if (!bank_2) {
    //bank 1
    YM_CTRL_PORT &= ~_BV(YM_A1);
  } else {
    //bank 2
    YM_CTRL_PORT |= _BV(YM_A1);
  }

  write_ym(reg);
  YM_CTRL_PORT |= _BV(YM_A0);  // A0 high (write register)
  write_ym(data);
}

//same as setreg_bank but defaults to bank 1
void setreg(uint8_t reg, uint8_t data) {
  setreg_bank(reg, data, false);
}

void setup_ym2612() {
  /* Pins setup */
  YM_CTRL_DDR |= _BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD) | _BV(YM_A0) | _BV(YM_A1);
  YM_DATA_DDR = 0xFF;
  YM_MCLOCK_DDR |= _BV(YM_MCLOCK);
  YM_CTRL_PORT |= _BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD); /* IC, CS, WR and RD HIGH by default */
  YM_CTRL_PORT &= ~(_BV(YM_A0) | _BV(YM_A1)); /* A0 and A1 LOW by default */

  /* F_CPU / 2 clock generation */
  TCCR1A = _BV(COM1A0);            /* Toggle OCA1 on compare match */
  TCCR1B = _BV(WGM12) | _BV(CS10); /* CTC mode with prescaler /1 */
  TCCR1C = 0;                      /* Flag reset */
  TCNT1 = 0;                       /* Counter reset */
  OCR1A = 0;                       /* Divide base clock by two */

  /* Reset YM2612 */
  YM_CTRL_PORT &= ~_BV(YM_IC);
  _delay_ms(10);
  YM_CTRL_PORT |= _BV(YM_IC);
  _delay_ms(10);
}
