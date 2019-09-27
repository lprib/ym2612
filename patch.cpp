#include "patch.h"
#include "ym.h"
#include <Arduino.h>

//param channel: the channel number from 0 to five
//returns the bank (0=bank1, 1=bank2)
//edits channel accordingly so it will work with the bank
//ie. channels 3, 4, 5 become channel 1, 2, 3 in bank2
bool convert_to_banked(u8* channel) {
  if(*channel > 2) {
    *channel -= 3;
    return true;
  }
  return false;
}

u8 register_offset(u8 channel, u8 op) {
  return 4 * op + channel;
}

void write_operator_config(u8 channel, u8 op, struct operator_config_s* c) {
  bool bank = convert_to_banked(&channel);
  
  u8 offset = register_offset(channel, op);
  setreg_bank(0x30 + offset, c->multiple | c->detune << 3, bank);
  setreg_bank(0x40 + offset, c->total_level, bank);
  setreg_bank(0x50 + offset, c->rate_scaling << 6 | c->attack_rate, bank);
  setreg_bank(0x60 + offset, c->amplitude_modulation << 7 | c->first_decay_rate, bank);
  setreg_bank(0x70 + offset, c->second_decay_rate, bank);
  setreg_bank(0x80 + offset, c->second_decay_level << 4 | c->release_rate, bank);
  //proprietary reg
  setreg_bank(0x90 + offset, 0, bank);
}

void write_channel_config(u8 channel, struct channel_config_s* c) {
  for (int i = 0; i < 4; i++) {
    write_operator_config(channel, i, &c->operators[i]);
  }
  
  //its OK to mutate channel now since we've already used the non-mutated version above (its no longer needed)
  bool bank = convert_to_banked(&channel);
  
  setreg_bank(0xB0 + channel, c->feedback << 3 | c->algorithm, bank);
  setreg_bank(0xB4 + channel, c->stereo_left_enable << 7 | c->stereo_right_enable << 6 | c->am_sensitivity << 3 | c->fm_sensitivity, bank);
}

void write_global_config(struct global_config_s* c) {
  setreg(0x22, c->lfo_enable << 3 | c->lfo_frequency);
  
  //put channels 3 and 6 into normal operation mode (not dissonant mode)
  setreg_bank(0x27, 0, false);
  setreg_bank(0x27, 0, true);
  
  for(int i = 0; i < 6; i++) {
    write_channel_config(i, &c->channels[i]);
  }
}

void set_freq(u8 channel, u11 frequency, u3 block) {
  bool bank = convert_to_banked(&channel);
  setreg_bank(0xA4 + channel, block << 3 | frequency >> 8, bank);
  setreg_bank(0xA0 + channel, frequency & 0xFF, bank);
}

void set_key(u8 channel, bool on_off) {
  if(on_off) {
    setreg(0x28, 0xF0 | channel);
  } else {
    setreg(0x28, 0x00 | channel);
  }
}
