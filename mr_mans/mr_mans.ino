#include <SPI.h>
#include <U8g2lib.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


//be sure to initialize with your display.  see https://github.com/olikraus/u8g2/wiki/setup_tutorial
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

int COLOR_WHITE = 1;
int COLOR_BLACK = 0;

int eye_width = 35;
int eye_height = 40;
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
void makeMainFace() {
  clearDisplay();
  // left eye
  u8g2.drawRBox(left_eye_x, eye_y, eye_width, eye_height, eye_radius);
  // right eye
  u8g2.drawRBox(right_eye_x, eye_y, eye_width, eye_height, eye_radius);
  // mouth
  u8g2.drawRBox(mouth_x, mouth_y, mouth_width, mouth_height, mouth_radius);
  sendDisplay();
}

void resetMainFace(int lx, int rx, int ty, int bx, int by) {
  int distance_x = ((lx - left_eye_x) / 1 );
  int distance_y = ((ty - eye_y) / 1);
  int third_x = distance_x / 3;
  int third_y = distance_y /3;

  int reset_left_eye_x = lx;
  int reset_right_eye_x = rx;
  int reset_eye_y = ty;
  int reset_mouth_x = bx;
  int reset_mouth_y = by;

  for (int i = 0; i < 2; i++) {
    if (lx > left_eye_x) {
      if (ty > eye_y) {
        reset_left_eye_x = reset_left_eye_x - third_x;
        reset_right_eye_x = reset_right_eye_x - third_x;
        reset_mouth_x = reset_mouth_x - third_x;
        reset_eye_y = reset_eye_y - third_y;
        reset_mouth_y = reset_mouth_y - third_y;

        makeFace(reset_left_eye_x, reset_right_eye_x, reset_eye_y, eye_width, eye_height, eye_radius, reset_mouth_x, reset_mouth_y, mouth_width, mouth_height, mouth_radius);

      } else if (ty < eye_y) {
        reset_left_eye_x = reset_left_eye_x - third_x;
        reset_right_eye_x = reset_right_eye_x - third_x;
        reset_mouth_x = reset_mouth_x - third_x;
        reset_eye_y = reset_eye_y - third_y;
        reset_mouth_y = reset_mouth_y - third_y;

        makeFace(reset_left_eye_x, reset_right_eye_x, reset_eye_y, eye_width, eye_height, eye_radius, reset_mouth_x, reset_mouth_y, mouth_width, mouth_height, mouth_radius);

      } else {
        reset_left_eye_x = reset_left_eye_x - third_x;
        reset_right_eye_x = reset_right_eye_x - third_x;
        reset_mouth_x = reset_mouth_x - third_x;

        makeFace(reset_left_eye_x, reset_right_eye_x, eye_y, eye_width, eye_height, eye_radius, reset_mouth_x, reset_mouth_y, mouth_width, mouth_height, mouth_radius);

      }

    } else if (lx < left_eye_x) {
      if (ty > eye_y) {
        reset_left_eye_x = reset_left_eye_x - third_x;
        reset_right_eye_x = reset_right_eye_x - third_x;
        reset_mouth_x = reset_mouth_x - third_x;
        reset_eye_y = reset_eye_y - third_y;
        reset_mouth_y = reset_mouth_y - third_y;

        makeFace(reset_left_eye_x, reset_right_eye_x, reset_eye_y, eye_width, eye_height, eye_radius, reset_mouth_x, reset_mouth_y, mouth_width, mouth_height, mouth_radius);

      } else if (ty < eye_y) {
        reset_left_eye_x = reset_left_eye_x - third_x;
        reset_right_eye_x = reset_right_eye_x - third_x;
        reset_mouth_x = reset_mouth_x - third_x;
        reset_eye_y = reset_eye_y - third_y;
        reset_mouth_y = reset_mouth_y - third_y;

        makeFace(reset_left_eye_x, reset_right_eye_x, reset_eye_y, eye_width, eye_height, eye_radius, reset_mouth_x, reset_mouth_y, mouth_width, mouth_height, mouth_radius);
        
      } else {
        reset_left_eye_x = reset_left_eye_x - third_x;
        reset_right_eye_x = reset_right_eye_x - third_x;
        reset_mouth_x = reset_mouth_x - third_x;

        makeFace(reset_left_eye_x, reset_right_eye_x, reset_eye_y, eye_width, eye_height, eye_radius, reset_mouth_x, reset_mouth_y, mouth_width, mouth_height, mouth_radius);
        
      }
    } else {
      if (ty > eye_y) {
        reset_eye_y = reset_eye_y - third_y;
        reset_mouth_y = reset_mouth_y - third_y;

        makeFace(reset_left_eye_x, reset_right_eye_x, reset_eye_y, eye_width, eye_height, eye_radius, reset_mouth_x, reset_mouth_y, mouth_width, mouth_height, mouth_radius);
        
      } else if (ty < eye_y) {
        reset_eye_y = reset_eye_y - third_y;
        reset_mouth_y = reset_mouth_y - third_y;

        makeFace(reset_left_eye_x, reset_right_eye_x, reset_eye_y, eye_width, eye_height, eye_radius, reset_mouth_x, reset_mouth_y, mouth_width, mouth_height, mouth_radius);
        
      }
    }
    third_x = third_x + (distance_x / 3);
    third_y = third_y + (distance_y / 3);
    delay(50);
  }
}
void makeFace(int lx, int rx, int ty, int ew, int eh, int er, int bx, int by, int mw, int mh, int mr) {
  clearDisplay();
  u8g2.drawRBox(lx, ty, ew, eh, er);
  u8g2.drawRBox(rx, ty, ew, eh, er);
  u8g2.drawRBox(bx, by, mw, mh, mr);
  sendDisplay();
}

void blink(int lx, int rx, int ty, int ew, int eh, int er, int bx, int by, int mw, int mh, int mr) {

  int blink_left_eye_x = lx;
  int blink_right_eye_x = rx;
  int blink_eye_y = ty;
  int blink_eye_width = ew;
  int blink_eye_height = eh;
  int blink_eye_radius = er;

  // Close eye loop
  for (int i = 0; i < 3; i++) {

    blink_eye_height = blink_eye_height - 12;
    blink_eye_y = blink_eye_y + 6;
    blink_eye_width = blink_eye_width + 3;
    blink_eye_radius = blink_eye_radius - 2;
    blink_left_eye_x = blink_left_eye_x - 1;
    blink_right_eye_x = blink_right_eye_x - 1;


    clearDisplay();
    // left eye
    u8g2.drawRBox(blink_left_eye_x, blink_eye_y, blink_eye_width, blink_eye_height, blink_eye_radius);
    // right eye
    u8g2.drawRBox(blink_right_eye_x, blink_eye_y, blink_eye_width, blink_eye_height, blink_eye_radius);
    // mouth
    u8g2.drawRBox(bx, by, mw, mh, mr);
    sendDisplay();
    delay(50);
  }

  // Open eye loop
  for (int i = 0; i < 3; i++) {

    blink_eye_height = blink_eye_height + 12;
    blink_eye_y = blink_eye_y - 6;
    blink_eye_width = blink_eye_width - 3;
    blink_eye_radius = blink_eye_radius + 2;
    blink_left_eye_x = blink_left_eye_x + 1;
    blink_right_eye_x = blink_right_eye_x + 1;

    clearDisplay();
    // left eye
    u8g2.drawRBox(blink_left_eye_x, blink_eye_y, blink_eye_width, blink_eye_height, blink_eye_radius);
    // right eye
    u8g2.drawRBox(blink_right_eye_x, blink_eye_y, blink_eye_width, blink_eye_height, blink_eye_radius);
    // mouth
    u8g2.drawRBox(bx, by, mw, mh, mr);
    sendDisplay();
    delay(50);
  }
}


void move() {
  int rand1 = random(18);
  int rand2 = random(9);
  int picker = random(8);
  int blink_rand = random(2);
  int lag = random(1, 4);

  rand1 = rand1 / 3;
  rand2 = rand2 / 3;

  int lex = 19;
  int rex = 74;
  int ey = 10;
  int mx = 57;
  int my = 50;

  for (int i = 0; i < 3; i++) {

    if (picker == 0) {
      ey = eye_y - rand2;
      my = mouth_y - rand2;

    } else if (picker == 1) {

      lex = left_eye_x + rand1;
      rex = right_eye_x + rand1;
      mx = mouth_x + rand1;
      ey = eye_y - rand2;
      my = mouth_y - rand2;

    } else if (picker == 2) {

      lex = left_eye_x + rand1;
      rex = right_eye_x + rand1;
      mx = mouth_x + rand1;

    } else if (picker == 3) {

      lex = left_eye_x + rand1;
      rex = right_eye_x + rand1;
      mx = mouth_x + rand1;
      ey = eye_y + rand2;
      my = mouth_y + rand2;

    } else if (picker == 4) {

      ey = eye_y + rand2;
      my = mouth_y + rand2;

    } else if (picker == 5) {

      lex = left_eye_x - rand1;
      rex = right_eye_x - rand1;
      mx = mouth_x - rand1;
      ey = eye_y + rand2;
      my = mouth_y + rand2;

    } else if (picker == 6) {

      lex = left_eye_x - rand1;
      rex = right_eye_x - rand1;
      mx = mouth_x - rand1;

    } else if (picker == 7) {

      lex = left_eye_x - rand1;
      rex = right_eye_x - rand1;
      mx = mouth_x - rand1;
      ey = eye_y - rand2;
      my = mouth_y - rand2;

    }
    makeFace(lex, rex, ey, eye_width, eye_height, eye_radius, mx, my, mouth_width, mouth_height, mouth_radius);
    rand1 = rand1 + rand1;
    rand2 = rand2 + rand2;
    delay(50);
  }

  if (blink_rand == 1) {

    blink(lex, rex, ey, eye_width, eye_height, eye_radius, mx, my, mouth_width, mouth_height, mouth_radius);
  }
  delay(lag * 1000);
  resetMainFace(lex, rex, ey, mx, my);
  delay(50);
}

void setup() {
  u8g2.setI2CAddress(0x78);
  u8g2.setDisplayRotation(U8G2_R0);
  u8g2.begin();
  makeMainFace();
  delay(3000);
}

void loop() {
  int rand_time = random(1, 4);
  move();
  makeMainFace();
  if (rand_time == 3) {
    blink(left_eye_x, right_eye_x, eye_y, eye_width, eye_height, eye_radius, mouth_x, mouth_y, mouth_width, mouth_height, mouth_radius);
  }
  delay(rand_time * 1000);
}
