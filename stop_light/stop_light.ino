#define GREEN 3
#define YELLOW 5
#define RED 6
#define lightDelay 7000 // time between light changes

void setup() {
pinMode(GREEN, OUTPUT);
pinMode(YELLOW, OUTPUT);
pinMode(RED, OUTPUT);
digitalWrite(GREEN, LOW);
digitalWrite(YELLOW, LOW);
digitalWrite(RED, HIGH);
}

void loop() {
// starts on red and then 5sec untill green light
delay(lightDelay);
digitalWrite(RED, LOW);
digitalWrite(GREEN, HIGH);

// its green for 3 sec then yellow
delay(lightDelay);
digitalWrite(GREEN, LOW);
digitalWrite(YELLOW, HIGH);

//its yellow for 3sec then red
delay(lightDelay - 2700);
digitalWrite(YELLOW, LOW);
digitalWrite(RED, HIGH);
}
