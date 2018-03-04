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
 * Handles the sprite and background display.
 * Text display supports two lines at the bottom of the screen.
 * Background display is currently implemented only for demonstration purposes.
 */

#include "Sprite.h"
#include "Graphics.h"
#include "TextArea.h"

static const int SPRITE_MAX  = 51;
static Sprite sprites[SPRITE_MAX];
static const uint16_t transparentColor = 0x8410;

static const uint8_t BG_CH_MASK = 0b111;
static bool backgroundEnable = false;
static uint8_t background[BG_COLS * BG_ROWS];
static const uint16_t *backgroundTable[8];

void Sprites_Init() {
  memset(bitmapBuff, 0x1f, sizeof(bitmapBuff));
  for (int part = 0; part < GRAPHIC_HEIGHT / GRAPHIC_BUFF_HEIGHT; part++) {
    int y1 = GRAPHIC_BUFF_HEIGHT * part;
    LCD_Update16(bitmapBuff, y1, GRAPHIC_BUFF_HEIGHT);
  }

  memset(background, 0, sizeof(background));
  memset(backgroundTable, 0, sizeof(backgroundTable));
}

Sprite *Sprites_Allocate() {
  Sprite *sp = sprites;
  for (int n = 0; n < SPRITE_MAX; n++, sp++) {
    if (!sp->use) {
      sp->use = true;
      return sp;
    }
  }
  s_text->error("SPRITE ALLOC FAIL!");
  return NULL;
}

INLINE bool Hit(int my1, int my2m, int ey1, int ey2m) {
  return (my1 < ey2m && ey1 < my2m);
}

INLINE void Sprites_DrawBackground(int part) {
  int by1 = part * GRAPHIC_BUFF_HEIGHT;
  int by2 = part * GRAPHIC_BUFF_HEIGHT + GRAPHIC_BUFF_HEIGHT;
  const int BG_ROWS_IN_PART = GRAPHIC_BUFF_HEIGHT / BG_SIZE;
  uint8_t *rp = background + (part * BG_ROWS_IN_PART * BG_COLS);
  for (int r = 0; r < BG_ROWS_IN_PART; r++) {
    for (int c = 0; c < BG_COLS; c++) {
      if (*rp) {
        const uint16_t *irp = backgroundTable[*rp];
        uint16_t *wp = bitmapBuff + GRAPHIC_WIDTH * BG_SIZE * r + c * BG_SIZE;
        for (int y = 0; y < BG_SIZE; y++) {
          for (int x = 0; x < BG_SIZE; x++) {
            uint16_t p = *irp++;
            if (p != transparentColor) {
              *wp = p;
            }
            wp++;
          }
          wp += GRAPHIC_WIDTH - BG_SIZE;
        }
      }
      rp++;
    }
  }
}

INLINE void Sprites_DrawSprite(int part) {
  int by1 = part * GRAPHIC_BUFF_HEIGHT;
  int by2 = part * GRAPHIC_BUFF_HEIGHT + GRAPHIC_BUFF_HEIGHT;
  Sprite *sp = sprites;
  for (int n = 0; n < SPRITE_MAX; n++, sp++) {
    if (!sp->use || !sp->show || sp->image == NULL) {
      continue;
    }
    if (!Hit(sp->y, sp->y + sp->h, by1, by2)) {
      continue;
    }
    const uint16_t *rp = sp->image;
    for (int y = 0; y < sp->h; y++) {
      for (int x = 0; x < sp->w; x++) {
        int px = sp->x + x;
        int py = sp->y + y;
        if (px >= 0 && px < GRAPHIC_WIDTH && py >= by1 && py < by2 && *rp != transparentColor) {
          bitmapBuff[px + (py - by1) * GRAPHIC_WIDTH] = *rp;
        }
        rp++;
      }
    }
  }
}

void Sprites_Update() {
  for (int part = 0; part < GRAPHIC_HEIGHT / GRAPHIC_BUFF_HEIGHT; part++) {
    memset(bitmapBuff, 0, sizeof(bitmapBuff));
    if (backgroundEnable) {
      Sprites_DrawBackground(part);
    }
    Sprites_DrawSprite(part);
    LCD_Update16(bitmapBuff, part * GRAPHIC_BUFF_HEIGHT, GRAPHIC_BUFF_HEIGHT);
  }
}

bool Sprite::hit(Sprite *other) {
  return x < other->x + other->w &&
         other->x < x + w &&
         y < other->y + other->h &&
         other->y < y + h;
}

void Bg_SetEnable(bool on) {
  backgroundEnable = on;
}

void Bg_Fill(uint8_t ch) {
  memset(background, ch & BG_CH_MASK, sizeof(background));
}
void Bg_Set(int x, int y, uint8_t ch) {
  int offset = x + BG_COLS * y;
  if (offset >= 0 && offset < sizeof(background)) {
    background[offset] = ch & BG_CH_MASK;
  }
}

void Bg_SetCharacter(uint8_t ch, const uint16_t *image) {
  if (ch < sizeof(backgroundTable)) {
    backgroundTable[ch] = image;
  }
}

