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

/**
 * Implementing game actors.
 */

#include "Object.h"
#include "Graphics.h"
#include "TextArea.h"
#include "Common.h"

extern const uint16_t *appleImage;
extern const uint16_t *bigbrain0Image;
extern const uint16_t *bigbrain1Image;
extern const uint16_t *bombImage;
extern const uint16_t *enemyship0Image;
extern const uint16_t *enemyship1Image;
extern const uint16_t *explode0Image;
extern const uint16_t *explode1Image;
extern const uint16_t *explode2Image;
extern const uint16_t *explode3Image;
extern const uint16_t *explode4Image;
extern const uint16_t *kumaImage;
extern const uint16_t *missileImage;
extern const uint16_t *orangeImage;
extern const uint16_t *pineappleImage;
extern const uint16_t *spaceship0Image;
extern const uint16_t *spaceship1Image;
extern const uint16_t *space_ship_0Image;
extern const uint16_t *space_ship_1Image;
extern const uint16_t *star1Image;
extern const uint16_t *star2Image;
extern const uint16_t *star3Image;
extern const uint16_t *ufo0Image;
extern const uint16_t *ufo1Image;
extern const uint16_t *usagiImage;

// --------------------------------------------------

static Cannon _cannon;
static Missiles _missiles;
static Bombs _bombs;
static Stars _stars;
static Specials _specials;

static Cannon *s_cannon = &_cannon;
static Missiles *s_missiles = &_missiles;
static Bombs *s_bombs = &_bombs;
static Stars *s_stars = &_stars;
static Specials *s_specials = &_specials;

// --------------------------------------------------

AlienController::AlienController() {
}

AlienController::~AlienController() {
}


// --------------------------------------------------

ControllerType0::ControllerType0() {
}

ControllerType0::~ControllerType0() {
}

void ControllerType0::move() {
}

Alien *ControllerType0::findCollision(Sprite *tsp) {
  return NULL;
}

// --------------------------------------------------

ControllerType1::ControllerType1() {
  const int ALIENS_IN_ROW = 6;
  const int IW = 16;
  const int CW = IW + 12;
  AlienType1 *obj = aliens;
  int bx = (GRAPHIC_WIDTH - (CW * (ALIENS_IN_ROW - 1) + IW)) / 2;
  for (int n = 0; n < ALIENS_MAX; n++, obj++) {
    int row = n / ALIENS_IN_ROW;
    int col = (n % ALIENS_IN_ROW);
    obj->characterImages[0] = ufo0Image;
    obj->characterImages[1] = ufo1Image;
    obj->sprite->w = IW;
    obj->sprite->h = Alien::normalHeight;
    obj->baseX = bx + (obj->sprite->w + 12) * col;
    obj->baseY = 10 + (obj->sprite->h + 8) * row;
    obj->homeX = obj->baseX;
    obj->homeY = obj->baseY;
    obj->sprite->image = obj->characterImages[0];
    obj->sprite->x = obj->homeX;
    obj->sprite->y = -obj->sprite->h;
    obj->sprite->show = true;
  }
  waveAngle = 90;
}

ControllerType1::~ControllerType1()
{
  for (int n = 0; n < ALIENS_MAX; n++) {
    if (aliens[n].sprite) {
      aliens[n].sprite->use = false;
      aliens[n].sprite = NULL;
    }
  }
}

void ControllerType1::move() {
  bool canDropSpecial = true;
  AlienType1 *attackableAliens[ALIENS_MAX];
  int attackableAliensCount = 0;
  AlienType1 *fireableAliens[ALIENS_MAX];
  int fireableAliensCount = 0;
  int lives = 0;
  bool fire = random(20) == 0;

  int offsetX, offsetY;
  waveAngle = fixAngle(waveAngle + 2);
  getCirclePos(waveAngle, 0, 0, 20, 0, offsetX, offsetY);

  AlienType1 *obj = aliens;
  for (int n = 0; n < ALIENS_MAX; n++, obj++) {
    if (obj->state == Alien::State_Dead) {
      continue;
    }
    lives++;
    Sprite *sp = obj->sprite;
    switch (obj->state) {
      case Alien::State_Normal:
        {
          if (obj->checkMissileHit() || (destroyAllMode && random(6) == 0)) {
            if (obj->toExplode()) {
              s_game->score += obj->getDestroyScore();
            }
            if (canDropSpecial) {
              s_specials->drop(sp->x, sp->y, 0, 2);
              canDropSpecial = false;
            }
          }
          obj->homeX = obj->baseX + offsetX;
          obj->homeY = obj->baseY + offsetY;
          obj->domove();
          attackableAliens[attackableAliensCount++] = obj;
          if (obj->canFire() && obj->nearCannon()) {
            fireableAliens[fireableAliensCount++] = obj;
          }
          sp->h = Alien::normalHeight;
          obj->sprite->image = obj->characterImages[(s_game->tick >> 2) & 1];
        }
        break;
      case Alien::State_Explode:
        obj->explodeTask();
        break;
    }
  }
  if (lives == 0) {
    s_game->stageClear();
  }
  else {
    if (random(20) == 0 && attackableAliensCount > 0) {
      attackableAliens[random(attackableAliensCount)]->startAttack();
    }
    if (fire && fireableAliensCount > 0) {
      AlienType1 *fa = fireableAliens[random(fireableAliensCount)];
      s_bombs->fire(fa->sprite->x + 7, fa->sprite->y + fa->sprite->h + 2, 3 + random(3));
    }
  }
}

Alien *ControllerType1::findCollision(Sprite *tsp) {
  AlienType1 *obj = aliens;
  for (int n = 0; n < ALIENS_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    if (obj->state == Alien::State_Normal && sp->hit(tsp)) {
      return obj;
    }
  }
  return NULL;
}

// --------------------------------------------------

AlienType1::AlienType1()
  : Alien() {
  subState = SubState_Return;
  xspeed = 0;
  yspeed = 2;
  xaccel = 0;
}

void AlienType1::startAttack() {
  if (state == Alien::State_Normal && subState == SubState_Normal) {
    subState = SubState_Attack;
    xaccel = random(2) == 0 ? 1 : -1;
    xlimit = 3 + random(12);
    xspeed = xaccel * xlimit;
    ylimit = 1 + random(6);
    yspeed = -8;
  }
}

void AlienType1::startReturn() {
  subState = SubState_Return;
  sprite->y = -sprite->h;
}

void AlienType1::domove() {
  switch (subState) {
    case SubState_Normal:
      sprite->x = homeX;
      sprite->y = homeY;
      break;
    case SubState_Attack:
      update();
      sprite->x += xspeed;
      sprite->y += yspeed;
      if (sprite->y > GRAPHIC_HEIGHT) {
        startReturn();
      }
      break;
    case SubState_Return:
      sprite->x = homeX;
      sprite->y += yspeed;
      if (sprite->y >= homeY) {
        subState = SubState_Normal;
        sprite->y = homeY;
      }
      break;
  }
}

void AlienType1::update() {
  xspeed += xaccel;
  if (abs(xspeed) > xlimit) {
    xspeed -= xaccel;
    xaccel = -xaccel;
  }
  yspeed += 1;
  if (yspeed > ylimit) {
    yspeed = ylimit;
  }
}

bool AlienType1::canFire() {
  return subState == SubState_Attack;
}

int AlienType1::getDestroyScore() {
  return (subState == AlienType1::SubState_Attack ? 80 : 50);
}

// --------------------------------------------------

ControllerType2::ControllerType2() {
  AlienType2 *obj = aliens;
  for (int n = 0; n < ALIENS_MAX; n++, obj++) {
    int row = n / 6;
    int col = (n % 6);
    obj->characterImages[0] = enemyship0Image;
    obj->characterImages[1] = enemyship1Image;
    obj->sprite->w = 16;
    obj->sprite->h = Alien::normalHeight;
    obj->sprite->image = obj->characterImages[0];
    obj->sprite->x = 0;
    obj->sprite->y = -obj->sprite->h;
    obj->sprite->show = true;
  }
}

ControllerType2::~ControllerType2()
{
  for (int n = 0; n < ALIENS_MAX; n++) {
    if (aliens[n].sprite) {
      aliens[n].sprite->use = false;
      aliens[n].sprite = NULL;
    }
  }
}

void ControllerType2::move() {
  bool canDropSpecial = true;
  AlienType2 *attackableAliens[ALIENS_MAX];
  int attackableAliensCount = 0;
  AlienType2 *fireableAliens[ALIENS_MAX];
  int fireableAliensCount = 0;
  int lives = 0;
  bool fire = random(20) == 0;

  AlienType2 *obj = aliens;
  for (int n = 0; n < ALIENS_MAX; n++, obj++) {
    if (obj->state == Alien::State_Dead) {
      continue;
    }
    lives++;
    Sprite *sp = obj->sprite;
    switch (obj->state) {
      case Alien::State_Normal:
        {
          if (obj->checkMissileHit() || (destroyAllMode && random(6) == 0)) {
            if (obj->toExplode()) {
              s_game->score += obj->getDestroyScore();
            }
            if (canDropSpecial) {
              s_specials->drop(sp->x, sp->y, 0, 2);
              canDropSpecial = false;
            }
            break;
          }
          switch (obj->subState) {
            case AlienType2::SubState_Hide:
              attackableAliens[attackableAliensCount++] = obj;
              break;
            case AlienType2::SubState_Run:
              obj->xspeed -= 10 * obj->dir;
              obj->hx += obj->xspeed;
              obj->hy += obj->yspeed;
              obj->sprite->x = obj->hx / 100;
              obj->sprite->y = obj->hy / 100;
              if (obj->sprite->x < -obj->sprite->w || obj->sprite->x > GRAPHIC_WIDTH || obj->sprite->y > GRAPHIC_HEIGHT) {
                obj->sprite->show = false;
                obj->subState = AlienType2::SubState_Hide;
              }
              else {
                sp->h = Alien::normalHeight;
                obj->sprite->image = obj->characterImages[(s_game->tick >> 2) & 1];
                if (obj->canFire() && obj->nearCannon2()) {
                  fireableAliens[fireableAliensCount++] = obj;
                }
              }
              break;
          }
        }
        break;
      case Alien::State_Explode:
        obj->explodeTask();
        break;
    }
  }
  if (lives == 0) {
    s_game->stageClear();
  }
  else {
    if (random(15) == 0 && attackableAliensCount > 0) {
      AlienType2 *fa = attackableAliens[random(attackableAliensCount)];
      fa->subState = AlienType2::SubState_Run;
      fa->dir = random(2) == 0 ? 1 : -1;
      fa->xspeed = (400 + random(30) - 15) * fa->dir;
      fa->yspeed = 400;
      fa->hx = (fa->dir == 1 ? random(50) : GRAPHIC_WIDTH - fa->sprite->w - random(50)) * 100;
      fa->hy = -fa->sprite->h * 100;
      fa->sprite->x = fa->hx / 100;
      fa->sprite->y = fa->hy / 100;
      fa->sprite->show = true;
      fa->state = Alien::State_Normal;
    }
    if (fire && fireableAliensCount > 0) {
      AlienType2 *fa = fireableAliens[random(fireableAliensCount)];
      s_bombs->fire(fa->sprite->x + 7, fa->sprite->y + fa->sprite->h + 2, fa->yspeed / 100 + 1 + random(3));
    }
  }
}

Alien *ControllerType2::findCollision(Sprite *tsp) {
  AlienType2 *obj = aliens;
  for (int n = 0; n < ALIENS_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    if (obj->state == Alien::State_Normal && sp->hit(tsp)) {
      return obj;
    }
  }
  return NULL;
}

// --------------------------------------------------

AlienType2::AlienType2()
  : Alien() {
  subState = SubState_Hide;
  xspeed = 0;
  yspeed = 0;
}

bool AlienType2::canFire() {
  return subState == SubState_Run;
}

int AlienType2::getDestroyScore() {
  return 120;
}

// --------------------------------------------------

ControllerType3::ControllerType3() {
  const int gx = GRAPHIC_WIDTH / 2;
  const int gy = 80;
  AlienType3 *obj = aliens;
  int angle = 0;
  for (int n = 0; n < ALIENS_MAX; n++, obj++) {
    obj->sprite->h = Alien::normalHeight;
    obj->sprite->x = -32;
    obj->sprite->y = -32;
    obj->hits = 0;
    if (n == 0) {
      obj->characterImages[0] = bigbrain0Image;
      obj->characterImages[1] = bigbrain1Image;
      obj->sprite->w = 32;
      obj->gx = gx - obj->sprite->w / 2;
      obj->gy = gy - obj->sprite->h / 2;
      obj->subState = AlienType3::SubState_Boss;
      obj->angle = 0;
    }
    else {
      obj->characterImages[0] = ufo0Image;
      obj->characterImages[1] = ufo1Image;
      obj->sprite->w = 16;
      obj->subState = AlienType3::SubState_Zako;
      obj->angle = angle;
      angle += 360 / (ALIENS_MAX - 1);
    }
    obj->sprite->image = obj->characterImages[0];
    obj->sprite->show = true;
  }
}

ControllerType3::~ControllerType3()
{
  for (int n = 0; n < ALIENS_MAX; n++) {
    if (aliens[n].sprite) {
      aliens[n].sprite->use = false;
      aliens[n].sprite = NULL;
    }
  }
}

void ControllerType3::move() {
  bool canDropSpecial = true;
  AlienType3 *fireableAliens[ALIENS_MAX];
  int fireableAliensCount = 0;
  int lives = 0;
  bool fire = random(20) == 0;

  AlienType3 *obj = aliens;
  for (int n = 0; n < ALIENS_MAX; n++, obj++) {
    if (obj->state == Alien::State_Dead) {
      continue;
    }
    lives++;
    Sprite *sp = obj->sprite;
    const int HITS_MAX = 50;
    const int HITS_DAMAGE = 20;
    switch (obj->state) {
      case Alien::State_Normal:
        {
          switch (obj->subState) {
            case AlienType3::SubState_Boss:
              if (obj->checkMissileHit() || (destroyAllMode && random(6) == 0)) {
                obj->hits += HITS_DAMAGE;
                if (obj->hits > HITS_MAX) {
                  if (obj->toExplode()) {
                    s_game->score += obj->getDestroyScore();
                    s_game->alienController->destroyAllMode = true;
                  }
                  //                  for (int r = 0; r < 360; r += 72) {
                  //                    int x, y;
                  //                    getCirclePos(r, sp->x, sp->y, 30, 20, x, y);
                  //                    s_game->drop(x, y, 0, 2, random(4) == 0 ? Special::Type_Point : Special::Type_Multi);
                  //                  }
                  break;
                }
              }
              obj->angle = fixAngle( obj->angle + 3);
              getCirclePos(obj->angle, obj->gx, obj->gy, 30, 10, obj->sprite->x, obj->sprite->y);
              break;
            case AlienType3::SubState_Zako:
              if (obj->checkMissileHit() || (destroyAllMode && random(6) == 0)) {
                obj->hits += HITS_DAMAGE;
                if (obj->hits > HITS_MAX) {
                  if (obj->toExplode()) {
                    s_game->score += obj->getDestroyScore();
                  }
                  if (canDropSpecial) {
                    s_specials->drop(sp->x, sp->y, 0, 2);
                    canDropSpecial = false;
                  }
                  break;
                }
              }
              AlienType3 *boss = &(aliens[0]);
              int bx = boss->sprite->x + boss->sprite->w / 2;
              int by = boss->sprite->y + boss->sprite->h / 2;
              int mx, my;
              obj->angle = fixAngle(obj->angle + 3);
              getCirclePos(obj->angle, bx, by, 85, 60, mx, my);
              sp->x = mx - sp->w / 2;
              sp->y = my - sp->h / 2;
              break;
          }
          if (obj->hits > 0) {
            obj->hits--;
          }
          sp->h = Alien::normalHeight;
          obj->sprite->image = (obj->hits == 0 || (s_game->tick & 1)) ? obj->characterImages[(s_game->tick >> 2) & 1] : NULL;
          if (obj->nearCannon2()) {
            fireableAliens[fireableAliensCount++] = obj;
          }
        }
        break;
      case Alien::State_Explode:
        obj->explodeTask();
        break;
    }
  }
  if (lives == 0) {
    s_game->stageClear();
  }
  else {
    if (fire && fireableAliensCount > 0) {
      AlienType3 *fa = fireableAliens[random(fireableAliensCount)];
      s_bombs->fire(fa->sprite->x + 7, fa->sprite->y + fa->sprite->h + 2, 3 + random(3));
    }
  }
}

Alien *ControllerType3::findCollision(Sprite *tsp) {
  AlienType3 *obj = aliens;
  for (int n = 0; n < ALIENS_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    if (obj->state == Alien::State_Normal && sp->hit(tsp)) {
      return obj;
    }
  }
  return NULL;
}

// --------------------------------------------------

AlienType3::AlienType3()
  : Alien() {
}

int AlienType3::getDestroyScore() {
  return subState == AlienType3::SubState_Boss ? 500 : 200;
}

// --------------------------------------------------

Alien::Alien() {
  state = Alien::State_Normal;
  sprite = Sprites_Allocate();
  explodeImages[0] = explode0Image;
  explodeImages[1] = explode1Image;
  explodeImages[2] = explode2Image;
  explodeImages[3] = explode3Image;
  explodeImages[4] = explode4Image;
}

bool Alien::nearCannon() {
  return sprite->y >= 50 && sprite->y < 170 && abs(s_cannon->sprite->x - sprite->x) < 60;
}

bool Alien::nearCannon2() {
  return sprite->y >= 10 && sprite->y < 90 && abs(s_cannon->sprite->x - sprite->x) < 60;
}

bool Alien::checkMissileHit() {
  Missile *ms = s_missiles->findHit(sprite);
  if (ms) {
    ms->stop();
    return true;
  }
  return false;
}

bool Alien::toExplode() {
  if (state == State_Normal) {
    state = State_Explode;
    stateTick = 0;
    return true;
  }
  return false;
}

void Alien::explodeTask() {
  sprite->h = explodeHeight;
  sprite->image = explodeImages[(stateTick >> 2) % 5];
  stateTick++;
  if ((stateTick >> 2) >= 5) {
    state = Alien::State_Dead;
    sprite->show = false;
  }
}

// --------------------------------------------------

void Cannon::init() {
  images[0] = spaceship0Image;
  images[1] = spaceship1Image;
  sprite = Sprites_Allocate();
  sprite->image = images[0];
  sprite->w = 16;
  sprite->h = 16;
  setState(State_Disable);
}

void Cannon::setState(State s) {
  state = s;
  switch (state) {
    case State_Disable:
      sprite->show = false;
      sprite->x = GRAPHIC_WIDTH / 2 - sprite->w / 2;
      sprite->y = GRAPHIC_HEIGHT - sprite->h - 4;
      multiFireTimer = 0;
      break;
    case State_Enable:
      sprite->show = true;
      fireDelay = 1;
      setState(State_Live);
      break;
    case State_Live:
      break;
    case State_Explode:
      stateTick = 32;
      break;
    case State_Recovery:
      if (s_game->cannonDestroy()) {
        setState(State_Disable);
      }
      else {
        sprite->x = GRAPHIC_WIDTH / 2 - sprite->w / 2;
        sprite->y = GRAPHIC_HEIGHT;
        moveSpeed = 2;
        multiFireTimer = 0;
        stateTick = 64 + 10;
      }
      break;
  }
}

void Cannon::move()  {
  switch (state) {
    case State_Disable:
      break;
    case State_Enable:
      break;
    case State_Live:
      if (!checkHit()) {
        moveBySwitch();
        sprite->image = images[(s_game->tick >> 2) & 1];
        fireBySwitch();
      }
      break;
    case State_Explode:
      if (stateTick <= 0) {
        setState(State_Recovery);
      }
      else {
        sprite->image = (stateTick--) & 1 ? NULL : images[0];
      }
      break;
    case State_Recovery:
      if (sprite->y != GRAPHIC_HEIGHT - sprite->h - 4) {
        sprite->y--;
      }
      else {
        moveBySwitch();
        if (stateTick <= 0) {
          setState(State_Enable);
        }
        fireBySwitch();
      }
      sprite->image = (stateTick--) & 1 ? NULL : images[0];
      break;
  }
  if (multiFireTimer > 0) {
    multiFireTimer--;
  }
}

bool Cannon::checkHit() {
  {
    Alien *hit = s_game->alienController->findCollision(sprite);
    if (hit) {
      setState(State_Explode);
      return true;
    }
  }
  {
    Bomb *hit = s_bombs->findHit(sprite);
    if (hit) {
      hit->state = Bomb::State_Idle;
      hit->sprite->show = false;
      setState(State_Explode);
      return true;
    }

  }
  return false;
}

void Cannon::moveBySwitch() {
  sprite->x += (s_game->leftButtonPressed() ? -moveSpeed : s_game->rightButtonPressed() ? moveSpeed : 0);
  if (sprite->x < 0) {
    sprite->x = 0;
  }
  if (sprite->x > GRAPHIC_WIDTH - sprite->w) {
    sprite->x = GRAPHIC_WIDTH - sprite->w;
  }
}

void Cannon::fireBySwitch() {
  switch (fireDelay) {
    case 0:
      if (s_game->fireButtonPressed()) {
        fireDelay = 1;
        s_missiles->fire(sprite->x + 7, sprite->y - 2, multiFireTimer > 0);
        s_specials->specialItemTick++;
      }
      break;
    case 1:
      if (!s_game->fireButtonPressed()) {
        fireDelay = 0;
      }
      break;
  }
}

void Cannon::startMultiFire(bool extendOnly) {
  const int add = 60 * 5;
  if (extendOnly) {
    if (multiFireTimer > 0 && multiFireTimer < add) {
      multiFireTimer = add;
    }
  } else {
    multiFireTimer += add;
  }
}

void Cannon::applySpecial(Special::Type type) {
  if (state == State_Live || state == State_Recovery) {
    switch (type) {
      case Special::Type_Point:
        s_game->score += 250;
        moveSpeed = 3;
        break;
      case Special::Type_Multi:
        s_game->score += 50;
        startMultiFire(false);
        break;
      case Special::Type_Destroyer:
        s_game->score += 50;
        s_game->alienController->destroyAllMode = true;
        break;
    }
  }
}

// --------------------------------------------------

void Missiles::init() {
  Missile *obj = missiles;
  for (int n = 0; n < MISSILE_MAX; n++, obj++) {
    obj->sprite = Sprites_Allocate();
    obj->sprite->image = missileImage;
    obj->sprite->w = 1;
    obj->sprite->h = 6;
    obj->isMain = (n < MAIN_COUNT);
  }
  reset();
}

void Missiles::reset() {
  Missile *obj = missiles;
  for (int n = 0; n < MISSILE_MAX; n++, obj++) {
    obj->state = Missile::State_Idle;
    obj->sprite->show = false;
    obj->sprite->image = missileImage;
    obj->sprite->w = 1;
    obj->sprite->h = 6;
  }
}

void Missiles::move()  {
  Missile *obj = missiles;
  for (int n = 0; n < MISSILE_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    switch (obj->state) {
      case Missile::State_Idle:
        break;
      case Missile::State_Run:
        sp->y -= 6;
        if (sp->y <= 0) {
          obj->stop();
        }
        break;
    }
  }
}

void Missiles::fire(int x, int y, bool multi) {
  Missile *main = NULL;
  Missile *obj = missiles;
  for (int n = 0; n < MISSILE_MAX; n++, obj++) {
    if (obj->isMain && obj->state == Missile::State_Idle) {
      main = obj;
      break;
    }
  }
  if (main == NULL) {
    return;
  }
  if (multi) {
    Missile *subs[SUB_COUNT];
    memset(subs, 0, sizeof(subs));
    int subi = 0;
    obj = missiles;
    for (int n = 0; n < MISSILE_MAX && subi < SUB_COUNT; n++, obj++) {
      if (!obj->isMain && obj->state == Missile::State_Idle) {
        subs[subi++] = obj;
      }
    }
    if (subi < SUB_COUNT) {
      return;
    }
    for (int n = 0; n < SUB_COUNT; n++) {
      subs[n]->startFire(x + (n & 1 ? 1 : -1) * 10 * (1 + (n >> 1)), y);
    }
  }
  main->startFire(x, y);
}

void Missile::startFire(int x, int y) {
  sprite->x = x;
  sprite->y = y;
  sprite->show = true;
  state = Missile::State_Run;
}

void Missile::stop() {
  if (state != Missile::State_Idle) {
    sprite->show = false;
    state = Missile::State_Idle;
  }
}

Missile *Missiles::findHit(Sprite *tsp) {
  Missile *obj = missiles;
  for (int n = 0; n < MISSILE_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    if (obj->state == Missile::State_Run && sp->hit(tsp)) {
      return obj;
    }
  }
  return NULL;
}

// --------------------------------------------------

void Bombs::init() {
  Bomb *obj = bombs;
  for (int n = 0; n < BOMB_MAX; n++, obj++) {
    obj->sprite = Sprites_Allocate();
    obj->sprite->image = bombImage;
    obj->sprite->w = 5;
    obj->sprite->h = 5;
  }
}

void Bombs::move() {
  Bomb *obj = bombs;
  for (int n = 0; n < BOMB_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    switch (obj->state) {
      case Bomb::State_Idle:
        break;
      case Bomb::State_Run:
        obj->bx += obj->dx;
        sp->x = obj->bx / 100;
        sp->y += obj->dy;
        if (sp->y > GRAPHIC_HEIGHT) {
          sp->show = false;
          obj->state = Bomb::State_Idle;
        }
        break;
    }
  }
}

Bomb* Bombs::fire(int x, int y, int dy) {
  Bomb *obj = bombs;
  for (int n = 0; n < Bombs::BOMB_MAX; n++, obj++) {
    if (obj->state == Bomb::State_Idle) {
      Sprite *sp = obj->sprite;
      sp->x = x;
      sp->y = y;
      sp->show = true;
      obj->bx = sp->x * 100;
      obj->dy = dy;
      obj->dx = (100 * (s_cannon->sprite->x - sp->x) * obj->dy) / (s_cannon->sprite->y - sp->y);
      obj->state = Bomb::State_Run;
      return obj;
    }
  }
  return NULL;
}

Bomb *Bombs::findHit(Sprite *tsp) {
  Bomb *obj = bombs;
  for (int n = 0; n < BOMB_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    if (obj->state == Bomb::State_Run && sp->hit(tsp)) {
      return obj;
    }
  }
  return NULL;
}

// --------------------------------------------------

void Stars::init() {
  Star *obj = stars;
  const uint16_t *images[] = { star1Image, star2Image, star3Image};
  int speeds[] = {5, 3, 1};
  int t = 0;
  for (int n = 0; n < STAR_MAX; n++, obj++) {
    obj->sprite = Sprites_Allocate();
    obj->sprite->image = images[t];
    obj->sprite->show = true;
    obj->sprite->w = 2;
    obj->sprite->h = 2;
    obj->sprite->x = random(GRAPHIC_WIDTH - obj->sprite->w);
    obj->sprite->y = random(GRAPHIC_HEIGHT - obj->sprite->h);
    obj->dy = speeds[t];
    if (++t >= 3) {
      t = 0;
    }
  }
}

void Stars::move() {
  Star *obj = stars;
  for (int n = 0; n < STAR_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    sp->y += obj->dy;
    if (sp->y > GRAPHIC_HEIGHT) {
      sp->x = random(GRAPHIC_WIDTH - sp->w);
      sp->y = -sp->h;
    }
  }
}

// --------------------------------------------------

void Specials::init() {
  Special *obj = specials;
  for (int n = 0; n < SPECIAL_MAX; n++, obj++) {
    obj->sprite = Sprites_Allocate();
    obj->sprite->show = false;
    obj->sprite->w = 16;
    obj->sprite->h = 16;
  }
}

void Specials::move() {
  Special *obj = specials;
  for (int n = 0; n < SPECIAL_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    if (sp->show) {
      sp->x += obj->dx;
      sp->y += obj->dy;
      if (sp->x < 0 || sp->x > GRAPHIC_WIDTH || sp->y < 0 || sp->y > GRAPHIC_HEIGHT) {
        sp->show = false;
      }
      else {
        if (sp->hit(s_cannon->sprite)) {
          sp->show = false;
          s_cannon->applySpecial(obj->type);
        }
      }
    }
  }
}

void Specials::launch(Special::Type type, int x, int y, int dx, int dy) {
  if (type == Special::Type_None) {
    return;
  }
  Special *obj = specials;
  for (int n = 0; n < SPECIAL_MAX; n++, obj++) {
    Sprite *sp = obj->sprite;
    if (!sp->show) {
      sp->x = x;
      sp->y = y;
      sp->show = true;
      obj->type = type;
      obj->sprite->image =
        type == Special::Type_Point     ? orangeImage :
        type == Special::Type_Multi     ? appleImage :
        type == Special::Type_Destroyer ? kumaImage : NULL;
      obj->dx = dx;
      obj->dy = dy;
      break;
    }
  }
}

void Specials::drop(int x, int y, int dx, int dy) {
  const Special::Type typePattern[16] = {
    Special::Type_None,
    Special::Type_Point,
    Special::Type_None,
    Special::Type_None,
    Special::Type_None,
    Special::Type_Multi,
    Special::Type_None,
    Special::Type_Point,
    Special::Type_None,
    Special::Type_None,
    Special::Type_Multi,
    Special::Type_None,
    Special::Type_Destroyer,
    Special::Type_None,
    Special::Type_Point,
    Special::Type_None,
  };
  Special::Type t = typePattern[(specialItemTick++) & 15];
  launch(t, x, y, dx, dy);
}

void Specials::drop(int x, int y, int dx, int dy, Special::Type type) {
  launch(type, x, y, dx, dy);
}

// --------------------------------------------------

void Game::init() {
  s_cannon->init();
  s_missiles->init();
  s_bombs->init();
  s_stars->init();
  s_specials->init();
  alienController = new ControllerType0();
  setState(State_Demo);
}

void Game::setState(State s) {
  state = s;
  switch (state) {
    case State_Demo:
      Bg_Fill(0);
      Bg_SetCharacter(1, appleImage);
      Bg_SetCharacter(2, pineappleImage);
      Bg_SetCharacter(3, usagiImage);
      Bg_SetEnable(true);
      s_cannon->setState(Cannon::State_Disable);
      stage = 0;
      loadAlienController();
      stateTick = 0;
      break;
    case State_Start:
      cannonLeft = 3;
      score = 0;
      stateTick = 0;
      break;
    case State_Play:
      break;
    case State_NextStage:
      stateTick = 0;
      break;
    case State_GameOver:
      s_cannon->setState(Cannon::State_Disable);
      stateTick = 0;
      break;
  }
}

bool updateBgForDemo(int tick) {
  if (tick < BG_ROWS) {
    for (int x = 0; x < BG_COLS; x++) {
      Bg_Set(x, tick, 1);
    }
  }
  else {
    if (tick < BG_ROWS * 2) {
      for (int x = 0; x < BG_COLS; x++) {
        Bg_Set(x, tick - BG_ROWS, 0);
      }
    }
    else {
      return true;
    }
  }
  return false;
}

void Game::move() {
  alienController->move();
  s_cannon->move();
  s_missiles->move();
  s_bombs->move();
  s_stars->move();
  s_specials->move();
  switch (state) {
    case State_Demo:
      if (updateBgForDemo(stateTick)) {
        if (fireButtonPressed()) {
          setState(State_Start);
        }
      }
      else {
        stateTick++;
      }
      break;
    case State_Start:
      if (updateBgForDemo(stateTick++)) {
        if (stateTick == BG_ROWS * 2 + 1) {
          Bg_SetEnable(false);
          s_cannon->setState(Cannon::State_Enable);
        }
        if (stateTick > BG_ROWS * 2 + 50) {
          stage = 1;
          loadAlienController();
          setState(State_Play);
        }
      }
      break;
    case State_Play:
      break;
    case State_NextStage:
      if (++stateTick >= 80) {
        stateTick = 0;
        stage = (++stage > 3 ? 1 : stage);
        loadAlienController();
        setState(State_Play);
      }
      break;
    case State_GameOver:
      if (++stateTick >= 240) {
        s_game->setState(Game::State_Demo);
      }
      break;
  }
  tick++;
}

void Game::drawTextPart(int fps) {
  switch (state) {
    case State_Demo:
      if ((tick & 16) == 0) {
        s_text->print(0, 0, ">>> PUSH FIRE TO START <<<");
      }
      break;
    case State_Start:
      break;
    case State_Play:
      break;
    case State_NextStage:
      break;
    case State_GameOver:
      if ((tick & 8) == 0) {
        s_text->print(0, 0, "GAME OVER");
      }
      break;
  }
  if (state != State_Demo) {
    s_text->print(0, 1, "SCORE");
    s_text->print(6, 1, score, 8, '0');
    s_text->print(30 - 4 - 1 - 1, 1, cannonLeft, 1, '0');
    s_text->print(30 - 4, 1, "LEFT");
    if (false) {
      s_text->print(30 - 3 - 3, 0, fps, 3);
      s_text->print(30 - 3, 0, "FPS");
    }
  }
}

bool Game::cannonDestroy() {
  if (--cannonLeft <= 0) {
    setState(State_GameOver);
    return true;
  }
  return false;
}

void Game::stageClear() {
  if (state == State_Play) {
    setState(State_NextStage);
  }
}

void Game::loadAlienController() {
  delete alienController;
  alienController = stage == 1 ? (AlienController *) new ControllerType1() :
                    stage == 2 ? (AlienController *) new ControllerType2() :
                    stage == 3 ? (AlienController *) new ControllerType3() : new ControllerType0();
}

bool Game::leftButtonPressed() {
  return digitalRead(PORT_SW_LEFT) == LOW;
}

bool Game::rightButtonPressed() {
  return digitalRead(PORT_SW_RIGHT) == LOW;
}

bool Game::fireButtonPressed() {
  return digitalRead(PORT_SW_FIRE) == LOW;
}


