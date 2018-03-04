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

/*
 * Common function.
 */

#include "Object.h"
#include "TextArea.h"
#include "Common.h"

// --------------------------------------------------

static Game _game;
static TextArea _text;

Game *s_game = &_game;
TextArea *s_text = &_text;

// --------------------------------------------------

extern const int *circleTable;

void getCirclePos(int angle, int gx, int gy, int xradius, int yradius, int &x, int &y) {
  angle = fixAngle(angle);
  x = gx + (circleTable[angle * 2 + 0] * xradius ) / 1000;
  y = gy + (circleTable[angle * 2 + 1] * yradius ) / 1000;
}

// --------------------------------------------------

