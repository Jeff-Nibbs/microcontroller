#define GREEN 3
#define YELLOW 5
#define RED 6
#define lightDelay 7000 // time between light changes
#define BUTTON 9

int button = 0;
int crosswalk;


void setup() {
pinMode(GREEN, OUTPUT);
pinMode(YELLOW, OUTPUT);
pinMode(RED, OUTPUT);
pinMode(BUTTON, INPUT_PULLUP);

digitalWrite(GREEN, HIGH);
digitalWrite(YELLOW, LOW);
digitalWrite(RED, LOW);
}

void loop() {
  if ( button == 0 && digitalRead(BUTTON) == LOW ) {
    crosswalk = 0;
    stopLights(lightDelay, crosswalk);
  }
  else if ( button == 1)

}

void stopLights(int delay, int cross) {

  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, HIGH);
  delay(delay);

  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, HIGH);
  delay(delay - 2700);

  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH);
  delay(delay + cross);
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
}
