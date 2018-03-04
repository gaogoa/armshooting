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

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Common.h"
#include "Sprite.h"

const int PORT_SW_LEFT  = PB0;
const int PORT_SW_RIGHT = PB1;
const int PORT_SW_FIRE  = PB3;

const int ALIENS_MAX = 18;

class Game;
class Alien;
class Cannon;
class Missiles;
class Specials;

// --------------------------------------------------

class AlienController {
  public:
    AlienController();
    virtual ~AlienController();
  public:
    virtual void move() = 0;
    virtual Alien *findCollision(Sprite *tsp) = 0;
  public:
    bool destroyAllMode = false;
};

// --------------------------------------------------

class Alien {
  public:
    static const int normalHeight = 16;
    static const int explodeHeight = 32;
  public:
    enum State { State_Normal, State_Explode, State_Dead };
    Sprite *sprite = NULL;
    State state = State_Normal;
    const uint16_t *characterImages[2];
    const uint16_t *explodeImages[5];
    int stateTick;
  public:
    Alien();
    bool nearCannon();
    bool nearCannon2();
    bool checkMissileHit();
    bool toExplode();
    void explodeTask();
};

// --------------------------------------------------

class ControllerType0 : public AlienController {
  public:
    ControllerType0();
    virtual ~ControllerType0();
    void move();
    Alien *findCollision(Sprite *tsp);
};

// --------------------------------------------------
class AlienType1 : public Alien {
  public:
    enum SubState { SubState_Normal, SubState_Attack, SubState_Return };
    SubState subState = SubState_Normal;
    int xspeed;
    int yspeed;
    int xaccel;
    int xlimit;
    int ylimit;
    int baseX, baseY;
    int homeX, homeY;
  public:
    AlienType1();
    void startAttack();
    void startReturn();
    void domove();
    void update();
    bool canFire();
    int getDestroyScore();
};

class ControllerType1 : public AlienController {
  public:
    AlienType1 aliens[ALIENS_MAX];
    int waveAngle;
  public:
    ControllerType1();
    virtual ~ControllerType1();
    void move();
    Alien *findCollision(Sprite *tsp);
};

// --------------------------------------------------

class AlienType2 : public Alien {
  public:
    enum SubState { SubState_Hide, SubState_Run};
    SubState subState = SubState_Hide;
    int hideCount = 0;
    int dir;
    int xspeed;
    int yspeed;
    int hx;
    int hy;
  public:
    AlienType2();
    bool canFire();
    int getDestroyScore();
};

class ControllerType2 : public AlienController {
  public:
    AlienType2 aliens[ALIENS_MAX];
  public:
    ControllerType2();
    virtual ~ControllerType2();
    void move();
    Alien *findCollision(Sprite *tsp);
};

// --------------------------------------------------

class AlienType3 : public Alien {
  public:
    enum SubState { SubState_Boss, SubState_Zako };
    SubState subState = SubState_Zako;
    int gx;
    int gy;
    int angle;
    int hits;
  public:
    AlienType3();
    int getDestroyScore();
};

class ControllerType3 : public AlienController {
  public:
    AlienType3 aliens[ALIENS_MAX];
  public:
    ControllerType3();
    virtual ~ControllerType3();
    void move();
    Alien *findCollision(Sprite *tsp);
};

// --------------------------------------------------

class Special {
  public:
    enum Type { Type_None, Type_Point, Type_Multi, Type_Destroyer };
    Sprite *sprite;
    Type type;
    int dx;
    int dy;
};

class Specials {
  public:
    static const int SPECIAL_MAX = 5;
    Special specials[SPECIAL_MAX];
    int specialItemTick = 0;
  public:
    void init();
    void move();
    void launch(Special::Type type, int x, int y, int dx, int dy);
    void drop(int x, int y, int dx, int dy);
    void drop(int x, int y, int dx, int dy, Special::Type type);
};

// --------------------------------------------------

class Cannon {
  public:
    Sprite *sprite;
    enum State { State_Disable, State_Enable, State_Live, State_Explode, State_Recovery };
    const uint16_t *images[2];
    int moveSpeed = 2;
    int fireDelay = 0;
    int stateTick;
    int multiFireTimer = 0;
  private:
    State state = State_Disable;
  public:
    void init();
    void setState(State s);
    void move();
    void startMultiFire(bool extendOnly);
    void applySpecial(Special::Type type);
  private:
    bool checkHit();
    void moveBySwitch();
    void fireBySwitch();
};

// --------------------------------------------------

class Missile {
  public:
    enum State { State_Idle, State_Run };
    Sprite *sprite;
    State state = State_Idle;
    bool isMain = false;
  public:
    void startFire(int x, int y);
    void stop();
};

class Missiles {
  public:
    static const int MISSILE_MAX = 15;
    static const int MAIN_COUNT = 5;
    static const int SUB_COUNT = 2;
    Missile missiles[MISSILE_MAX];
  public:
    void init();
    void reset();
    void move();
    void fire(int x, int y, bool multi);
    Missile *findHit(Sprite *tsp);
};

// --------------------------------------------------

class Bomb {
  public:
    enum State { State_Idle, State_Run };
    Sprite *sprite;
    State state = State_Idle;
    int bx;
    int dx;
    int dy;
};

class Bombs {
  public:
    static const int BOMB_MAX = 3;
    Bomb bombs[BOMB_MAX];
  public:
    void init();
    void move();
    Bomb* fire(int x, int y, int dy);
    Bomb *findHit(Sprite *tsp);
};

// --------------------------------------------------

class Star {
  public:
    Sprite *sprite;
    int dy;
};

class Stars {
  public:
    static const int STAR_MAX = 9;
    Star stars[STAR_MAX];
  public:
    void init();
    void move();
};

// --------------------------------------------------

class Game {
  public:
    enum State { State_Demo, State_Start, State_Play, State_NextStage, State_GameOver };
    void init();
    void move();
    void drawTextPart(int fps);
    void stageClear();
    bool cannonDestroy();
    bool leftButtonPressed();
    bool rightButtonPressed();
    bool fireButtonPressed();
    void setState(State s);
  private:
    void loadAlienController();
  private:
    State state = State_Demo;
  public:
    int stateTick = 0;
    int tick = 0;
    AlienController *alienController = NULL;
    int stage = 0;
    int cannonLeft;
    int score = 0;
};

#endif

