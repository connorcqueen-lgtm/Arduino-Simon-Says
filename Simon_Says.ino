const int ledPins[4] = {2, 3, 4, 5};
const int buttonPins[4] = {13, 12, 11, 10};
const int MAX_SEQUENCE = 100;
int sequence[MAX_SEQUENCE];
int sequenceLength = 1;
 
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  randomSeed(analogRead(A0));
  sequence[0] = random(0, 4);
  delay(1000);
}
 
void loop() {
  Serial.print("Round: ");
  Serial.println(sequenceLength);
  showSequence();
  Serial.println("Your turn!");
 
  bool correct = true;
  for (int i = 0; i < sequenceLength; i++) {
    int pressed = waitForButton();
    Serial.print("Expected: ");
    Serial.println(sequence[i] + 1);
    Serial.print("Pressed: ");
    Serial.println(pressed + 1);
    if (pressed != sequence[i]) {
      correct = false;
      break;
    }
  }
 
  if (correct) {
    Serial.println("Correct!");
    winSignal();
    if (sequenceLength < MAX_SEQUENCE) {
      sequence[sequenceLength] = random(0, 4);
      sequenceLength++;
    } else {
      sequenceLength = 1;
      sequence[0] = random(0, 4);
    }
  } else {
    Serial.println("Wrong! Game Over!");
    errorSignal();
    sequenceLength = 1;
    sequence[0] = random(0, 4);
  }
  delay(1000);
}
 
// Display each LED in the saved sequence.
void showSequence() {
  delay(500);
  for (int i = 0; i < sequenceLength; i++) {
    int led = sequence[i];
    digitalWrite(ledPins[led], HIGH);
    delay(600);
    digitalWrite(ledPins[led], LOW);
    delay(300);
  }
}
 
// Wait for a confirmed button press and release.
int waitForButton() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        delay(50); // Debounce switch contact.
        if (digitalRead(buttonPins[i]) == LOW) {
          Serial.print("Detected button: ");
          Serial.println(i + 1);
          digitalWrite(ledPins[i], HIGH);
          while (digitalRead(buttonPins[i]) == LOW) {
            delay(10);
          }
          digitalWrite(ledPins[i], LOW);
          delay(300);
          return i;
        }
      }
    }
  }
}
 
// All four LEDs blink three times after a win.
void winSignal() {
  for (int repeat = 0; repeat < 3; repeat++) {
    for (int i = 0; i < 4; i++)
      digitalWrite(ledPins[i], HIGH);
    delay(300);
    for (int i = 0; i < 4; i++)
      digitalWrite(ledPins[i], LOW);
    delay(300);
  }
}
 
// All LEDs on, then turn off individually.
void errorSignal() {
  for (int i = 0; i < 4; i++)
    digitalWrite(ledPins[i], HIGH);
  delay(500);
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
    delay(300);
  }
}
