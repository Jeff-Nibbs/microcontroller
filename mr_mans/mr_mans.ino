#include <SPI.h>
#include <U8g2lib.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


//be sure to initialize with your display.  see https://github.com/olikraus/u8g2/wiki/setup_tutorial
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

int COLOR_WHITE = 1;
int COLOR_BLACK = 0;

int eye_width = 35;
int eye_height = 41;
int eye_radius = 7;
int eye_y = 10;

int left_eye_x = 19;
int right_eye_x = 74;

int mouth_x = 57;
int mouth_y = 50;
int mouth_width = 15;
int mouth_height = 6;
int mouth_radius = 1;

void clearDisplay()
{
  u8g2.clearBuffer();
}

void sendDisplay() {
  u8g2.sendBuffer();
}

// Resets up main face
void resetMainFace() {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x, eye_y, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x, eye_y, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x, mouth_y, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void plusPlus(int r1, int r2) {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x + r1, eye_y + r2, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x + r1, eye_y + r2, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x + r1, mouth_y + r2, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void nanPlus(int r1, int r2) {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x, eye_y + r2, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x, eye_y + r2, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x, mouth_y + r2, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void negPlus(int r1, int r2) {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x - r1, eye_y + r2, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x - r1, eye_y + r2, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x - r1, mouth_y + r2, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void negNan(int r1, int r2) {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x - r1, eye_y, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x - r1, eye_y, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x - r1, mouth_y, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void negNeg(int r1, int r2) {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x - r1, eye_y - r2, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x - r1, eye_y - r2, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x - r1, mouth_y - r2, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void nanNeg(int r1, int r2) {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x, eye_y - r2, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x, eye_y - r2, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x, mouth_y - r2, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void plusNeg(int r1, int r2) {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x + r1, eye_y + r2, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x + r1, eye_y + r2, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x + r1, mouth_y + r2, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void plusNan(int r1, int r2) {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x + r1, eye_y, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x + r1, eye_y, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x + r1, mouth_y, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void move() {
  int rand1 = random(18);
  int rand2 = random(9);
  int picker = random(8);

  if (picker == 0) {
    nanNeg(rand1, rand2);
  } else if (picker == 1) {
    plusNeg(rand1, rand2);
  } else if (picker == 2) {
    plusNan(rand1, rand2);
  } else if (picker == 3) {
    plusPlus(rand1, rand2 % 7);
  } else if (picker == 4) {
    nanPlus(rand1, rand2 % 7);
  } else if (picker == 5) {
    negPlus(rand1, rand2 % 7);
  } else if (picker == 6) {
    negNan(rand1, rand2);
  } else if (picker == 7) {
    negNeg(rand1, rand2);
  }
}

void setup() {
  u8g2.setI2CAddress(0x78);
  u8g2.setDisplayRotation(U8G2_R0);
  u8g2.begin();
  // set the face
  resetMainFace();
  delay(3000);
}

void loop() {
  int looks = random(1, 4);
  for (int i = 0; i < looks; i++) {
    move();
    delay(looks * 1000);
  }
  resetMainFace();
  delay(looks * 1000);
}
