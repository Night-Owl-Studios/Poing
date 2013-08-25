/* 
 * File:   main.h
 * Author: hammy
 *
 * Created on April 16, 2013, 5:46 PM
 */

#ifndef __POING_MAIN_H__
#define	__POING_MAIN_H__

#include "harbinger.h"

/******************************************************************************
 * Namespace setup
******************************************************************************/
using namespace hamLibs;

#define HGE_MODEL_MAT hge::pipeline::HGE_MODEL_MAT
#define HGE_VP_MAT hge::pipeline::HGE_VP_MAT

// harbinger input callbacks
using hgeKey_t     = hge::input::key_t;
using hgeAction_t  = hge::input::action_t;
using hgeMod_t     = hge::input::mod_t;
using hgeMouse_t   = hge::input::mouse_t;

/******************************************************************************
 * Forward declarations of Classes
******************************************************************************/
// Game Render Objects
class arenaRenderApp;

// Game Logic Objects
class arenaLogicApp;

// Game Manager Objects
class arenaObjMgr;

/******************************************************************************
 * GLOBAL CONSTANTS
******************************************************************************/
#define GAME_TITLE          "Poing!"

#define DEFAULT_MOUSE_SPEED 5.0e-3

#define DEFAULT_CAMERA_DISTANCE 20.f
#define MIN_CAMERA_DISTANCE 15.f
#define MAX_CAMERA_DISTANCE 30.f
#define MIN_CAMERA_HEIGHT   5.f
#define MAX_CAMERA_HEIGHT   20.f

#define MAX_ENEMIES         100
#define MAX_ENEMIES_SQRT    10
#define ENEMY_VELOCITY      10.f
#define ENEMY_MASS          3.f
#define ENEMY_BOUNCINESS    1.f

#define BULLET_MASS         5.f
#define BULLET_BOUNCINESS   0.95f
#define BULLET_VELOCITY     5.f
#define BULLET_SCALE_FACTOR 0.25f

#define OVERLAY_FONT_SIZE   72

#define GRAVITY_FACTOR      -0.0983
#define ARENA_SIZE          10.f
#define MAX_LEVEL_DEPTH     -100.f

/******************************************************************************
 * Global Variables
******************************************************************************/
extern hge::window*         pWindow;
extern hge::input*    pInput;
extern arenaRenderApp*      pRenderer;
extern arenaLogicApp*       pGameLogic;

#endif	/* __POING_MAIN_H__ */

