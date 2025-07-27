// pins
const int buttonPins[4] = {26, 27, 28, 29};
const int ledPins[4] = {6, 7, 0, 4};  
const int buzzer1Pin = 1;            
const int buzzer2Pin = 2;            

const int sequence[] = {0, 1, 3, 2}; 
const int sequenceLength = sizeof(sequence) / sizeof(sequence[0]);

const int noteDuration = 500;   // ms
const int buttonTimeout = 2000; // ms to press button

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); // buttons active low
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buzzer1Pin, OUTPUT);
  pinMode(buzzer2Pin, OUTPUT);
}

void playTone(int pin, int frequency, int duration) {
  tone(pin, frequency, duration);
  delay(duration);
  noTone(pin);
}

void showSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    int led = sequence[i];
    digitalWrite(ledPins[led], HIGH);
    playTone(buzzer1Pin, 440 + led * 100, noteDuration);
    delay(noteDuration);
    digitalWrite(ledPins[led], LOW);
    delay(200);
  }
}

bool waitForButton(int expectedButton) {
  unsigned long startTime = millis();
  while (millis() - startTime < buttonTimeout) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        delay(20); // debounce
        if (digitalRead(buttonPins[i]) == LOW) {
          while (digitalRead(buttonPins[i]) == LOW); // wait release
          return i == expectedButton;
        }
      }
    }
  }
  return false;
}

void errorFeedback() {
  playTone(buzzer2Pin, 200, 500);
}

void successMelody() {
  int melody[] = {440, 523, 659, 880};
  for (int i = 0; i < 4; i++) {
    playTone(buzzer1Pin, melody[i], 200);
    delay(100);
  }
}

void loop() {
  showSequence();

  for (int i = 0; i < sequenceLength; i++) {
    if (!waitForButton(sequence[i])) {
      errorFeedback();
      delay(1000);
      return; // restart loop
    }
  }

  successMelody();
  delay(2000);
}
