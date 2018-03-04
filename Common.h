/*
 * The MIT License
 * 
 * Copyright (c) 2018 gaogoa.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <Arduino.h>

class Game;
class TextArea;

#define INLINE inline __attribute__((always_inline))

#define LARGER(a,b) ( (a) > (b) ? (a) : (b) )
#define LARGER3(a,b,c) (LARGER(LARGER((a),(b)),(c)))

extern Game *s_game;
extern TextArea *s_text;

INLINE int fixAngle(int angle) {
  return angle != 0 ? angle % 360 : 0;
}

void getCirclePos(int angle, int gx, int gy, int xradius, int yradius, int &x, int &y);

#endif

