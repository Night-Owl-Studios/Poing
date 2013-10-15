/* 
 * File:   gameApp.h
 * Author: hammy
 *
 * Created on July 17, 2013, 2:09 AM
 */

#ifndef __POING_GAME_APPLICATION_H__
#define	__POING_GAME_APPLICATION_H__

#include "main.h"
#include "blankNormalMap.h"
#include "objects.h"

//-----------------------------------------------------------------------------
//	Game Application Class
//-----------------------------------------------------------------------------
class arenaRenderApp : public hge::dsRenderer {
    private:
        blankNormalMap      blankNormMap;
        hge::camera         viewportCam;
        
        hge::bitmap         enemyTex;
        hge::billboard      enemyBB;
        
        hge::mesh           floorMesh;
        hge::drawTransform  floorTrans;
        
        hge::sphere         spherePrim;
        hge::bitmap         bulletTex;
        
        hge::cubemap        skyTex;
        hge::drawTransform  skyTrans;
        
        hge::font           overlayFont;
        hge::text           overlayStr;
        hge::drawTransform  overlayTrans;

    public:
        arenaRenderApp             ( const arenaRenderApp& )    = delete;
        arenaRenderApp             ( arenaRenderApp&& )         = delete;
        arenaRenderApp& operator = ( const arenaRenderApp& )    = delete;
        arenaRenderApp& operator = ( arenaRenderApp&& )         = delete;
        
        arenaRenderApp          () {}
        ~arenaRenderApp         () { terminate(); }
        
        bool    init            ();
        void    terminate       () { hge::dsRenderer::terminate(); }
        
        void    updateScene     ( float );
        
        void    rotateArenaCam  ( float dx, float dy );
        void    zoomArenaCam    ( float amount );
        void    retainArenaCam  ();
        const hge::camera& getViewportCam() { return viewportCam; }
        
    private:
        void    changeResolution( const math::vec2i& );
        
        void    drawSceneLit    ();
        void    drawSceneUnlit  () {}
        void    drawSky         ();
        void    drawFonts       ();
        void    drawBillboards  ();
        void    drawDebug       () {}
        
        void    drawBullets     ( const bulletList_t& );
        void    drawEnemies     ( const enemyList_t& );
};

#endif	/* __POING_GAME_APPLICATION_H__ */

