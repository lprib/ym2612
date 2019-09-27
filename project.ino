#include "ym.h"
#include "patch.h"

/** Program entry point */
int main2(void) {
  setup_ym2612();

  /* YM2612 Test code */
  setreg(0x22, 0x00); // LFO off
  setreg(0x27, 0x00); // Note off (channel 0)
  setreg(0x28, 0x01); // Note off (channel 1)
  setreg(0x28, 0x02); // Note off (channel 2)
  setreg(0x28, 0x04); // Note off (channel 3)
  setreg(0x28, 0x05); // Note off (channel 4)
  setreg(0x28, 0x06); // Note off (channel 5)
  setreg(0x2B, 0x00); // DAC off
  setreg(0x30, 0x71); //
  setreg(0x34, 0x0D); //
  setreg(0x38, 0x33); //
  setreg(0x3C, 0x01); // DT1/MUL
  setreg(0x40, 0x23); //
  setreg(0x44, 0x2D); //
  setreg(0x48, 0x26); //
  setreg(0x4C, 0x00); // Total level
  setreg(0x50, 0x5F); //
  setreg(0x54, 0x99); //
  setreg(0x58, 0x5F); //
  setreg(0x5C, 0x94); // RS/AR
  setreg(0x60, 0x05); //
  setreg(0x64, 0x05); //
  setreg(0x68, 0x05); //
  setreg(0x6C, 0x07); // AM/D1R
  setreg(0x70, 0x02); //
  setreg(0x74, 0x02); //
  setreg(0x78, 0x02); //
  setreg(0x7C, 0x02); // D2R
  setreg(0x80, 0x11); //
  setreg(0x84, 0x11); //
  setreg(0x88, 0x11); //
  setreg(0x8C, 0xA6); // D1L/RR
  setreg(0x90, 0x00); //
  setreg(0x94, 0x00); //
  setreg(0x98, 0x00); //
  setreg(0x9C, 0x00); // Proprietary
  setreg(0xB0, 0x32); // Feedback/algorithm
  setreg(0xB4, 0xC0); // Both speakers on
  setreg(0x28, 0x00); // Key off
  setreg(0xA4, 0x22); //
  setreg(0xA0, 0x69); // Set frequency

  /* Program loop */
  for (;;) {
    _delay_ms(1000);
    setreg(0x28, 0xF0); // Key on
    _delay_ms(1000);
    setreg(0x28, 0x00); // Key off
  }

  /* Compiler fix */
  return 0;
}

int main(void) {
  setup_ym2612();
  struct global_config_s g = {
    .lfo_enable = 0,
    .lfo_frequency = 0,
    .channels = {
      {
        .feedback = 0,
        .algorithm = 4,
        .stereo_left_enable = 1,
        .stereo_right_enable = 1,
        .am_sensitivity = 0,
        .fm_sensitivity = 0,
        .operators = {
          {
            .detune_sign = 0,
            .detune = 0,
            .multiple = 1,
            .total_level = 0,
            .rate_scaling = 0,
            .attack_rate = 31,
            .amplitude_modulation = 0,
            .first_decay_rate = 5,
            .second_decay_rate = 5,
            .second_decay_level = 8,
            .release_rate = 3
          },
          {
            .detune_sign = 0,
            .detune = 3,
            .multiple = 3,
            .total_level = 0,
            .rate_scaling = 0,
            .attack_rate = 31,
            .amplitude_modulation = 0,
            .first_decay_rate = 16,
            .second_decay_rate = 16,
            .second_decay_level = 8,
            .release_rate = 3
          },
          {
            .detune_sign = 1,
            .detune = 2,
            .multiple = 2,
            .total_level = 0,
            .rate_scaling = 0,
            .attack_rate = 31,
            .amplitude_modulation = 0,
            .first_decay_rate = 16,
            .second_decay_rate = 16,
            .second_decay_level = 8,
            .release_rate = 3
          },
          DEFAULT_OPERATOR_CONFIG,
        }
      },
      DEFAULT_CHANNEL_CONFIG,
      DEFAULT_CHANNEL_CONFIG,
      DEFAULT_CHANNEL_CONFIG,
      DEFAULT_CHANNEL_CONFIG,
      DEFAULT_CHANNEL_CONFIG
    }
  };

  write_global_config(&g);
  set_freq(0, 1200, 3);
  
  for(;;) {
    set_key(0, true);
    _delay_ms(200);
    set_key(0, false);
    _delay_ms(200);
  }
}
