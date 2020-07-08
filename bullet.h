#ifndef __BULLET_H
#define __BULLET_H

struct bullet{
  int16_t curX;
  int16_t curY;
  int16_t endX;
  int16_t endY;
  uint8_t speed;
  int8_t state;
  char* color;


};

#endif