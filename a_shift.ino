void init_key_shift_in(int pin_clock, int pin_latch, int pin_output) {
  pinMode(pin_clock, OUTPUT);
  pinMode(pin_latch, OUTPUT);
  pinMode(pin_output, INPUT);
}

void key_shift_in(int pin_clock, int pin_latch, int pin_output, bool* outputArray) {
  digitalWrite(pin_latch, LOW);
  digitalWrite(pin_clock, LOW);
  
  outputArray[0] = digitalRead(pin_output);

  digitalWrite(pin_latch, HIGH);

  for(int i = 1; i < 16; i++) {
      digitalWrite(pin_clock, LOW);
      digitalWrite(pin_clock, HIGH);
      outputArray[i] = digitalRead(pin_output);
  }


//  outputArray[5] = 1;
  pinMode(A0, INPUT_PULLUP);
}
