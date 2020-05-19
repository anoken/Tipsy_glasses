//Copyright (c) 2020 aNo研　<https://github.com/anoken>

//LovyanGFX https://github.com/lovyan03/LovyanGFX
#include <LGFX_TFT_eSPI.hpp>
static TFT_eSPI lcd;               
TFT_eSprite *Spr;

static float rot_theta = 0;
const int x_zero = 160;
const int y_zero = 120;

uint32_t front_col = TFT_BLACK;
uint32_t back_col = TFT_YELLOW;

unsigned long time_old_ms = 0;
unsigned long time_static_ms = 0;
float d_rot_speed;


void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.setRotation(1);

  Spr = new TFT_eSprite(&lcd);
  Spr->setColorDepth(16);
  Spr->createSprite(320, 240);
  Spr->setBitmapColor(front_col, back_col);
  time_old_ms = millis();
}

void loop() {
  Spr->fillSprite(back_col);

  long time_now_ms = millis();
  long dt = time_now_ms - time_old_ms;
  time_old_ms = time_now_ms;
  time_static_ms += dt;

  if (time_static_ms > 4000) {
    d_rot_speed = 0.2;
  }
  else if (time_static_ms > 2000) {
    d_rot_speed = 0.1;
  }

  if (time_static_ms > 40000) {
    d_rot_speed = -0.0;
  }

  rot_theta += d_rot_speed;
  float open = 10;
  draw_face( open, rot_theta);

  if  (rot_theta > 2 * 3.1415)rot_theta -= 2 * 3.1415;
  else if  (rot_theta < -2 * 3.1415)rot_theta += 2 * 3.1415;

}


/* Rotation */
void rot(int16_t &x, int16_t &y, float theta) {
  int16_t x_rot, y_rot;
  x_rot = (x - x_zero) * cos(theta) - (y - y_zero) * sin(theta) + x_zero;
  y_rot = (x - x_zero) * sin(theta) +  (y - y_zero) * cos(theta) + y_zero;
  x = x_rot;
  y = y_rot;
}

/* View Triangle */
void fillTriangle_r(TFT_eSprite *spi, int16_t x0, int16_t y0, int16_t x1,
                    int16_t y1, int16_t x2, int16_t y2, uint16_t color, float theta) {
  rot(x0, y0, theta);
  rot(x1, y1, theta);
  rot(x2, y2, theta);
  spi->fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

/* View CirCle */
void fillCircle_r(TFT_eSprite *spi, int16_t x, int16_t y, int16_t r, uint16_t color, float theta) {
  rot(x, y, theta);
  spi->fillCircle( x, y, r,  color);
}

/* View Quad */
void fillRect_r(TFT_eSprite *spi, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, float theta) {
  int16_t x0 = x - w / 2;
  int16_t y0 = y - h / 2;
  int16_t x1 = x + w / 2;
  int16_t y1 = y - h / 2;
  int16_t x2 = x - w / 2;
  int16_t y2 = y + h / 2;
  int16_t x3 = x + w / 2;
  int16_t y3 = y + h / 2;

  rot(x0, y0, theta);
  rot(x1, y1, theta);
  rot(x2, y2, theta);
  rot(x3, y3, theta);

  spi->fillTriangle( x0, y0,  x1,  y1,  x3,  y3,  color);
  spi->fillTriangle( x0, y0,  x2,  y2,  x3,  y3,  color);
}

/* View Face */
void draw_face(float open, float theta) {
  static long cnt = 0;

 //Base Smart Slasses E-Kanji https://bit.ly/2ThiHle　Face Shape 
 
  //Face View
  Spr->fillSprite(back_col);  //背景を塗る。
  fillTriangle_r(Spr, 190, 70, 190, 50,
                 280 + random(10), 25 + random(15), front_col, theta); //左眉
  fillTriangle_r(Spr, 130, 70, 130, 50,
                 50 + random(10), 25 + random(15), front_col, theta); //右眉
  fillRect_r(Spr, 163, 188, 60, open / 2, front_col, theta);


  if (cnt < 20) {
    fillCircle_r(Spr, 90 + random(5), 93 + random(5),
                 25 , front_col , theta); //左目
    fillCircle_r(Spr, 230 + random(5), 93 + random(5),
                 25 , front_col , theta); //右目
  }
  else if (cnt < 50) {
    fillRect_r(Spr, 90, 93, 50, 5, front_col, theta);
    fillRect_r(Spr, 230, 93, 50, 5, front_col, theta);
  }
  else {
    cnt = 0;
  }
  cnt++;
  Spr->pushSprite(0, 0);     //スプライトを表示する

  d_rot_speed = 0;
}
