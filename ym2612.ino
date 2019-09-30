#include "ym.h"
#include "patch.h"
#include "melody.h"

void key_shift_in(int pin_clock, int pin_latch, int pin_output, bool* outputArray);
void init_key_shift_in(int pin_clock, int pin_latch, int pin_output);
void init_pots();
void update_pots();
bool pot_changed(int index, int max_value, int* out_value);


#define PIN_CLOCK 7
#define PIN_LATCH 6
#define PIN_OUTPUT 5

int keyboard_notes[] = {NOTE_C, NOTE_Cs, NOTE_D, NOTE_Ds, NOTE_E, NOTE_F, NOTE_Fs, NOTE_G, NOTE_Gs, NOTE_A, NOTE_As, NOTE_B};
bool keyboard_state[12] = {false};

int keyboard_mapping[12] = {7, 6, 5, 3, 4, 2, 1, 0, 15, 14, 13, 12};

//this is where intermediate values for key on/off are stored when shifting them in
bool key_shift_values[16];

//pins that the pots are connected to
int pot_pins[] = {A1, A2, A3, A4, A5, A6, A7};

//stores which key is assigned to which polyphony slot
int poly_state[] = { -1, -1, -1};

#define NUM_POLY 3

int fm_amount = 0;
int attack = 0;
int rel = 0;
int algorithm = 0;
int feedback = 0;

struct channel_config_s my_chan = {
    .feedback = 0,
    .algorithm = 4,
    .stereo_left_enable = 1,
    .stereo_right_enable = 1,
    .am_sensitivity = 0,
    .fm_sensitivity = 7,
    .operators = {
      {
        .detune_sign = 0,
        .detune = 3,
        .multiple = 5,
        .total_level = 5,
        .rate_scaling = 3,
        .attack_rate = 31,
        .amplitude_modulation = 1,
        .first_decay_rate = 5,
        .second_decay_rate = 0,
        .second_decay_level = 16,
        .release_rate = 16
      },
      {
        .detune_sign = 1,
        .detune = 3,
        .multiple = 1,
        .total_level = 5,
        .rate_scaling = 3,
        .attack_rate = 31,
        .amplitude_modulation = 1,
        .first_decay_rate = 0,
        .second_decay_rate = 0,
        .second_decay_level = 16,
        .release_rate = 16
      }, {
        .detune_sign = 0,
        .detune = 0,
        .multiple = 2,
        .total_level = 0,
        .rate_scaling = 3,
        .attack_rate = 1,
        .amplitude_modulation = 1,
        .first_decay_rate = 0,
        .second_decay_rate = 0,
        .second_decay_level = 16,
        .release_rate = 16
      }, {
        .detune_sign = 0,
        .detune = 1,
        .multiple = 3,
        .total_level = 0,
        .rate_scaling = 3,
        .attack_rate = 1,
        .amplitude_modulation = 1,
        .first_decay_rate = 0,
        .second_decay_rate = 0,
        .second_decay_level = 16,
        .release_rate = 16
      },
    }
  };

  struct global_config_s g = {
    .lfo_enable = 1,
    .lfo_frequency = 1,
    .channels = {
      my_chan,
      my_chan,
      my_chan,
      DEFAULT_CHANNEL_CONFIG,
      DEFAULT_CHANNEL_CONFIG,
      DEFAULT_CHANNEL_CONFIG,
    }
  };

void setup() {
  Serial.begin(9600);
  setup_ym2612();

  write_global_config(&g);
  init_key_shift_in(PIN_CLOCK, PIN_LATCH, PIN_OUTPUT);
  init_pots();
}


void loop() {
  update_pots();
  key_shift_in(PIN_CLOCK, PIN_LATCH, PIN_OUTPUT, key_shift_values);


  for (int i = 0; i < 12; i++) {
    int key_index = keyboard_mapping[i];
    bool new_state = !key_shift_values[key_index];

    if (new_state && !keyboard_state[i]) {
      for (int j = 0; j < NUM_POLY; j++) {
        if (poly_state[j] == -1) {
          //free poly state
          set_freq(j, keyboard_notes[i], 3);
          set_key(j, true);
          poly_state[j] = i;
          break;
        }
      }
    }
    else if (!new_state && keyboard_state[i]) {
      for (int j = 0; j < NUM_POLY; j++) {
        if (poly_state[j] == i) {
          set_key(j, false);
          poly_state[j] = -1;
          break;
        }
      }
    }
    keyboard_state[i] = new_state;
  }
  //    _delay_ms(20);

      if(pot_changed(4, 7, &fm_amount)) {
        Serial.println(fm_amount);
        for(int i = 0; i < 3; i++) {
          setreg(0xB4 + i, my_chan.stereo_left_enable << 7 | my_chan.stereo_right_enable << 6 | my_chan.am_sensitivity << 3 | fm_amount);
        }
      }

      if(pot_changed(5, 31, &attack)) {
        for(int ch = 0; ch < 3; ch++) {
          for(int op = 0; op < 4; op++) {
            int offset = 4 * op + ch;
            setreg(0x50 + offset, 3 << 6 | attack);
          }
        }
      }

      if(pot_changed(1, 15, &rel)) {
        for(int ch = 0; ch < 3; ch++) {
          for(int op = 0; op < 4; op++) {
            int offset = 4 * op + ch;
            setreg(0x80 + offset, 16 << 4 | rel);
          }
        }
      }

      if(pot_changed(6, 7, &algorithm) || pot_changed(2, 7, &feedback)) {
        for(int i = 0; i < 3; i++) {
          setreg(0xB0 + i, feedback << 3 | algorithm);
        }
      }

}
