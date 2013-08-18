/* 
 * File:   gameApp.cpp
 * Author: hammy
 * 
 * Created on July 18, 2013, 2:10 PM
 */

#include <iostream>
#include "main.h"
#include "logicApp.h"
#include "objects.h"
#include "renderApp.h"

using hge::window;
using math::vec2i;
using math::vec2d;
using math::vec3;

arenaObjMgr arenaLogicApp::objMgr;

//-----------------------------------------------------------------------------
//	Keyboard Button Callback Function
//-----------------------------------------------------------------------------
void arenaLogicApp::arenaKeyButton( window::context*, hgeKey_t k, int, hgeAction_t, hgeMod_t ) {
    switch( k ) {
        // PROGRAM SHUT-DOWN
        case hgeKey_t::HGE_KEY_ESCAPE:
            pRenderer->stopRunning();
            break;
            
        default:
            break;
    }
}

//-----------------------------------------------------------------------------
//	Mouse Updating Callback Function
//-----------------------------------------------------------------------------
void arenaLogicApp::arenaMousePos( window::context*, double mx, double my ) {
    const vec2i screenCenter    = pWindow->getResolution();
    const vec2d mousePos = vec2d( 0.5 * screenCenter[0], 0.5 * screenCenter[1] ); // center the cursor
    const vec2d mouseDelta = vec2d(
        getMouseSpeed().v[0] * (mousePos[0] - mx),
        getMouseSpeed().v[1] * (mousePos[1] - my)
    );
    
    pRenderer->rotateArenaCam( mouseDelta[0], -mouseDelta[1] );
    
    pInput->setMousePos( mousePos );
}

//-----------------------------------------------------------------------------
//	Mouse Button Callback Function
//-----------------------------------------------------------------------------
void arenaLogicApp::arenaMouseButton( window::context*, hgeMouse_t mb, hgeAction_t a, hgeMod_t ) {
    if ( mb != hgeMouse_t::HGE_MB_LEFT || a != hgeAction_t::HGE_PRESSED )
        return;
    
    const hge::camera& cam = pRenderer->getViewportCam();
    objMgr.launchBullet( cam.getPos(), cam.getDir() );
}

//-----------------------------------------------------------------------------
//	Mouse Wheel Callback
//-----------------------------------------------------------------------------
void arenaLogicApp::arenaMouseWheel( hge::window::context*, double, double y ) {
    pRenderer->zoomArenaCam( y );
}

//-----------------------------------------------------------------------------
//	Logic Updating
//-----------------------------------------------------------------------------
void arenaLogicApp::tick() {
    appTimer.update();
    float tickTime = appTimer.getTickDuration();
    timeAccum += tickTime; 
    
    if ( timeAccum >= 1.f ) {
        objMgr.emitEnemy();
        timeAccum = 0.f;
    }
    
    objMgr.updateBulletPositions( tickTime );
    objMgr.updateEnemyPositions( tickTime );
}

