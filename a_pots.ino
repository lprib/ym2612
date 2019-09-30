const int NUM_POTS = sizeof(pot_pins)/sizeof(int);

int pot_state[NUM_POTS] = {0};
int pot_prev_state[NUM_POTS] = {0};

void init_pots() {
  for(int i = 0; i < NUM_POTS; i++) {
    pinMode(pot_pins[i], INPUT);
  }
}

void update_pots() {
  for(int i = 0; i < NUM_POTS; i++) {
    pot_prev_state[i] = pot_state[i];
    pot_state[i] = analogRead(pot_pins[i]);
  }
}

bool pot_changed(int index, int max_value, int* out_value) {
  int prev_scaled = map(pot_prev_state[index], 0, 1023, 0, max_value);
  int now_scaled = map(pot_state[index], 0, 1023, 0, max_value);

  if(prev_scaled != now_scaled) {
    //state change
    *out_value = now_scaled;
    return true;
  }
  return false;
}
