/* 
 * File:   gameApp.h
 * Author: hammy
 *
 * Created on July 18, 2013, 2:10 PM
 */

#ifndef __POING_LOGIC_APP_H__
#define	__POING_LOGIC_APP_H__

#include "main.h"
#include "objects.h"

class arenaLogicApp : public hge::application {
    friend class arenaRenderApp;
    
    private:
        static void arenaKeyButton  ( hge::window::context*, hgeKey_t, int, hgeAction_t, hgeMod_t );
        static void arenaMousePos   ( hge::window::context*, double mx, double my );
        static void arenaMouseButton( hge::window::context*, hgeMouse_t, hgeAction_t, hgeMod_t );
        static void arenaMouseWheel ( hge::window::context*, double x, double y );
        
        static arenaObjMgr objMgr;
        
        float timeAccum = 0.f;
        int score = 0;
        
    public:
        arenaLogicApp() {}
        ~arenaLogicApp() {}
            
        void startRunning() {
            pInput->setKeyButtonCB      ( &arenaKeyButton );
            pInput->setKeyTextCB        ( nullptr );
            pInput->setMousePosCB       ( &arenaMousePos );
            pInput->setMouseButtonCB    ( &arenaMouseButton );
            pInput->setMouseWheelCB     ( &arenaMouseWheel );
            pInput->applyCallbacks      ();
            
            appTimer.start();
        }
        
        constexpr static math::vec3 getMouseSpeed() { return math::vec3( DEFAULT_MOUSE_SPEED ); }
        
        void tick();
        
        int getScore() const { return score; }
        void setScore( int s ) { score = s; }
};

#endif	/* __POING_LOGIC_APP_H__ */

