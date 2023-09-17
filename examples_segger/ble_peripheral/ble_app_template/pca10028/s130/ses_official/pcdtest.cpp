#include "PCD8544.h"
#include "adafruit_control.h"


// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
// PCD8544 nokia = PCD8544(7, 6, 5, 4, 3);

// a bitmap of a 16x16 fruit icon
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16


void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h);
void testdrawchar(void);
void testdrawcircle(void);
void testdrawrect(void);
void testfillrect(void);
void testdrawline(void);
void adafruit_test( void );

extern PCD8544 nokia;


void adafruit_test( void ) {

  nokia.init();
  // you can change the contrast around to adapt the display
  // for the best viewing!
  nokia.setContrast(40);
  // turn all the pixels on (a handy test)
  nokia.command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYALLON);
  delay(500);
  // back to normal
  nokia.command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

  // show splashscreen
  nokia.display();
  delay(2000);
  nokia.clear();

  // draw a single pixel
  nokia.setPixel(10, 10, BLACK);
  nokia.display();        // show the changes to the buffer
  delay(500);
  nokia.clear();

   // draw many lines
  testdrawline();
  nokia.display();       // show the lines
  delay(500);
  nokia.clear();

  // draw mulitple circles
  testdrawcircle();
  nokia.display();
  delay(500);
  nokia.clear();

  // draw the first ~120 characters in the font
  testdrawchar();
  nokia.display();
  delay(500);
  nokia.clear();

  // draw a string at location (0,0)
  nokia.setCursor(0, 0);
  nokia.print("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor");
  nokia.display();
  delay(500);
  nokia.clear();
  nokia.display();
}

void loop(void) {}

#define NUMFLAKES 8
#define XPOS 0
#define YPOS 1
#define DELTAY 2


void testdrawchar(void) {
  for (uint8_t i=0; i < 64; i++) {
    nokia.drawchar((i % 14) * 6, (i/14) * 8, i);
  }
  nokia.display();
  delay(2000);
  for (uint8_t i=0; i < 64; i++) {
    nokia.drawchar((i % 14) * 6, (i/14) * 8, i + 64);
  }
}

void testdrawcircle(void) {
  for (uint8_t i=0; i<48; i+=2) {
    nokia.drawcircle(41, 23, i, BLACK);
  }
}

void testdrawline() {
  for (uint8_t i=0; i<84; i+=4) {
    nokia.drawline(0, 0, i, 47, BLACK);
  }
  for (uint8_t i=0; i<48; i+=4) {
    nokia.drawline(0, 0, 83, i, BLACK);
  }

  nokia.display();
  delay(1000);
  for (uint8_t i=0; i<84; i+=4) {
    nokia.drawline(i, 47, 0, 0, WHITE);
  }
  for (uint8_t i=0; i<48; i+=4) {
    nokia.drawline(83, i, 0, 0, WHITE);
  }
}