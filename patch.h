#ifndef PATCH_H
#define PATCH_H

typedef unsigned char u1;
typedef unsigned char u2;
typedef unsigned char u3;
typedef unsigned char u4;
typedef unsigned char u5;
typedef unsigned char u6;
typedef unsigned char u7;
typedef unsigned char u8;

typedef unsigned char u11;

//NB: all fields MUST be able to fit into their type size (eg any u3 must be < 8)
struct operator_config_s {
  //0 positive, 1 negative
  u1 detune_sign;
  u2 detune;
  //multiple of zero means 1/2, otherwise its just the number
  u3 multiple;
  u7 total_level;
  u2 rate_scaling;
  u5 attack_rate;
  u1 amplitude_modulation;
  u5 first_decay_rate;
  u5 second_decay_rate;
  u4 second_decay_level;
  //this has one less bit than all the other rates (AR, D1R, D2R)
  //the LSB is assumed to always be one, and this u4 defines the 4 MSBs
  u4 release_rate;
};
const struct operator_config_s DEFAULT_OPERATOR_CONFIG = {0, 0, 1, 127, 0, 31, 0, 16, 16, 8, 8};

struct channel_config_s {
  u3 feedback;
  u3 algorithm;
  u1 stereo_left_enable;
  u1 stereo_right_enable;
  u2 am_sensitivity;
  u3 fm_sensitivity;
  struct operator_config_s operators[4];
};

const struct channel_config_s DEFAULT_CHANNEL_CONFIG = {
  0, 7, 1, 1, 0, 0,
  {DEFAULT_OPERATOR_CONFIG, DEFAULT_OPERATOR_CONFIG, DEFAULT_OPERATOR_CONFIG, DEFAULT_OPERATOR_CONFIG}
};

struct global_config_s {
  u1 lfo_enable;
  u3 lfo_frequency;
  //skip channel 3 alternate mode settings (should be normal mode be default)
  //skip dac reg and dac enable (should be off by default)

  struct channel_config_s channels[6];
};

const struct global_config_s DEFAULT_GLOBAL_CONFIG = {
  0, 0,
  {
    DEFAULT_CHANNEL_CONFIG,
    DEFAULT_CHANNEL_CONFIG,
    DEFAULT_CHANNEL_CONFIG,
    DEFAULT_CHANNEL_CONFIG, 
    DEFAULT_CHANNEL_CONFIG, 
    DEFAULT_CHANNEL_CONFIG
  }
};

void write_operator_config(u8 channel, u8 op, struct operator_config_s* c);
void write_channel_config(u8 channel, struct channel_config_s* c);
void write_global_config(struct global_config_s* c);
void set_freq(u8 channel, u11 frequency, u3 block);
void set_key(u8 channel, bool on_off);

#endif
