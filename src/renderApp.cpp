/* 
 * File:   gameApp.cpp
 * Author: hammy
 * 
 * Created on July 17, 2013, 2:09 AM
 */

#include <string>
#include "renderApp.h"
#include "logicApp.h"

//-----------------------------------------------------------------------------
//	Initialization
//-----------------------------------------------------------------------------
bool arenaRenderApp::init() {
    if ( !hge::dsRenderer::init( pWindow->getResolution() ) ) return false;
    
    /*
     * Bitmaps
     */
    if ( !blankNormMap.init() ) return false;
    if ( !floorMesh.load( "resources/arena/arena.obj" ) ) return false;
    if ( !bulletTex.load( "resources/bullet.jpg" ) ) return false;
    if ( !enemyTex.load( "resources/enemy.png" ) ) return false;
    
    /*
     * Billboards
     */
    if ( !enemyBB.setNumImages( MAX_ENEMIES_SQRT, MAX_ENEMIES_SQRT ) ) return false;
    
    /*
     * Primitives
     */
    if ( !skyTex.load( "resources/sky/right.jpg", 0 ) ) return false;
    if ( !skyTex.load( "resources/sky/left.jpg", 1 ) ) return false;
    if ( !skyTex.load( "resources/sky/bottom.jpg", 2 ) ) return false;
    if ( !skyTex.load( "resources/sky/top.jpg", 3 ) ) return false;
    if ( !skyTex.load( "resources/sky/front.jpg", 4 ) ) return false;
    if ( !skyTex.load( "resources/sky/back.jpg", 5 ) ) return false;
    if ( !spherePrim.init() ) return false;
    
    /*
     * Fonts
     */
    if ( !overlayFont.load( "resources/CALIBRIB.TTF", OVERLAY_FONT_SIZE ) ) return false;
    
    /*
     * Object Setup
     */
    viewportCam.setViewMode     ( hge::camera::VIEW_ORBIT );
    viewportCam.setOrbitDist    ( DEFAULT_CAMERA_DISTANCE );
    viewportCam.lockYAxis       ( true );
    viewportCam.look            ( math::vec3( 1.f ), math::vec3( 0.f ), hge::pipeline::getWorldAxisY() );
    
    changeResolution            ( pWindow->getResolution() );
    viewportCam.update          ();
    printGlError("Viewport setup error");
    
    overlayTrans.setPos         ( math::vec3( -5.f, 1.f, -5.f ) );
    overlayTrans.setScale       ( math::vec3( 0.05f ) );
    overlayTrans.update         ();
    overlayStr.setString        ( overlayFont, GAME_TITLE );
    printGlError("Overlay setup error");
    
    setFontColor                ( math::vec4( 0.f, 0.25f, 0.75f, 0.5f ) );
    printGlError("Font setup error");
    
    {
        hge::dsPointLight pntLight;
        pntLight.color              = math::vec4( 1.f, 0.f, 0.f, 1.f );
        pntLight.attrib.intensity   = 50.f;
        pntLight.scale = pntLight.calcInfluenceRadius();
        pntLight.position           = math::vec3( 0.f, -3.f, 0.f );
        addPointLight( pntLight );
        
        pntLight.color              = math::vec4( 0.f, 0.f, 1.f, 1.f );
        pntLight.attrib.intensity   = 100.f;
        pntLight.scale = pntLight.calcInfluenceRadius();
        pntLight.position           = math::vec3( 0.f, 5.f, 0.f );
        addPointLight( pntLight );
        printGlError("Point Light setup error");
    }
    
    floorTrans.setScale     ( math::vec3( 2.125f ) );
    floorTrans.update       ();
    
    for ( int i = 0; i < MAX_ENEMIES; ++i ) {
        enemyBB.setImagePos( i, math::vec3( 0.f, MAX_LEVEL_DEPTH, 0.f ) );
    }
    
    printGlError("Enemy setup error");
    
    return true;
}

//-----------------------------------------------------------------------------
//	Display Resizing Callback
//-----------------------------------------------------------------------------
void arenaRenderApp::changeResolution( const math::vec2i& res ) {
    viewportCam.setProjectionParams ( 60.f, res[0], res[1], 0.1f, 100.f );
    viewportCam.makePerspective     ();
}

//-----------------------------------------------------------------------------
//	Main Loop
//-----------------------------------------------------------------------------
void arenaRenderApp::updateScene( float ) {
    const math::vec2i& res = pWindow->getResolution();
    glViewport( 0, 0, res[0], res[1] );
    
    /*
     * Update the camera's position
     */
    //math::vec3 newPos = viewportCam.getPos() - (viewportCam.getPos() * 2.f );
    math::vec3 newPos = math::normalize( -viewportCam.getPos() );
    newPos[0] *= DEFAULT_CAMERA_DISTANCE;
    newPos[1] = 0.f;
    newPos[2] *= DEFAULT_CAMERA_DISTANCE;
    
    overlayTrans.setPos( newPos );
    overlayTrans.look( viewportCam.getPos() );
    
    overlayTrans.update();
    
    printGlError("Pre main-loop error");
    
    // Update the viewport
    retainArenaCam();
    viewportCam.update();
    
    // reposition the skybox
    skyTrans.setPos( viewportCam.getPos() );
    skyTrans.update();
    
    applyMatrix( HGE_VP_MAT, viewportCam.getVPMatrix() );
}

//-----------------------------------------------------------------------------
//      Rotating the poing arena camera
//-----------------------------------------------------------------------------
void arenaRenderApp::rotateArenaCam( float dx, float dy ) {
    viewportCam.rotate(
        math::vec3(
            dx * viewportCam.getOrbitDist(), dy * viewportCam.getOrbitDist(), 0.f
        )
    );
}

//-----------------------------------------------------------------------------
//      zooming the poing arena cam
//-----------------------------------------------------------------------------
void arenaRenderApp::zoomArenaCam( float amount ) {
    float dist = viewportCam.getOrbitDist() + amount;
    
    if ( dist < MIN_CAMERA_DISTANCE )
        dist = MIN_CAMERA_DISTANCE;
    else if ( dist > MAX_CAMERA_DISTANCE )
        dist = MAX_CAMERA_DISTANCE;
    
    viewportCam.setOrbitDist( dist );
}

//-----------------------------------------------------------------------------
//      Keeping the arena camera within certain limits
//-----------------------------------------------------------------------------
void arenaRenderApp::retainArenaCam() {
    const float heightLimit   = viewportCam.getOrbitDist() * std::sin( HL_PI_OVER_4 );
    //const float heightLimit    = viewportCam.getOrbitDist() * std::sin( HL_PI_OVER_6 );
    const float depthLimit    = 0.f;
    
    const math::vec3& camPos = viewportCam.getPos();
    
    if ( camPos[1] > heightLimit ) {
        viewportCam.setPos( math::vec3( camPos[0], heightLimit, camPos[2] ) );
    }
    else if ( camPos[1] < depthLimit ) {
        viewportCam.setPos( math::vec3( camPos[0], depthLimit, camPos[2] ) );
    }
}

/******************************************************************************
 * BULLET DRAWING
******************************************************************************/
void arenaRenderApp::drawBullets( const bulletList_t& bulletList ) {
    bulletTex.activate();
    
    for ( unsigned i = 0; i < bulletList.size(); ++i ) {
        dsPointLights[ i+2 ].position = bulletList[ i ].trans.getPos();
        applyMatrix( HGE_MODEL_MAT, bulletList[ i ].trans.getModelMatrix() );
        spherePrim.draw();
    }
    
    bulletTex.deActivate();
    
    lightSphere.setLightBuffer( dsPointLights.data(), dsPointLights.size() );
}

/******************************************************************************
 * ENEMY DRAWING
******************************************************************************/
void arenaRenderApp::drawEnemies( const enemyList_t& enemyList ) {
    unsigned iter = 0;
    
    for ( const enemy& e : enemyList ) {
        enemyBB.setImagePos( iter++, e.pos );
    }
    
    enemyTex.activate();
    enemyBB.draw();
    enemyTex.deActivate();
}

/******************************************************************************
 * NO-LIGHTING PASS
******************************************************************************/
void arenaRenderApp::drawSceneLit() {
    // Draw all bullet objects
    blankNormMap.activate();
    drawBullets( arenaLogicApp::objMgr.getBulletList() );
    
    // Objects using the point-light shader
    applyMatrix( HGE_MODEL_MAT, floorTrans.getModelMatrix() );
    floorMesh.draw();
    blankNormMap.deActivate();
}

/******************************************************************************
 * BILLBOARD PASS
******************************************************************************/
void arenaRenderApp::drawBillboards() {
    setBillboardTarget( viewportCam.getPos() );
    drawEnemies( arenaLogicApp::objMgr.getEnemyList() );
}

/******************************************************************************
 * SKY PASS
******************************************************************************/
void arenaRenderApp::drawSky() {
    applyMatrix( HGE_MODEL_MAT, skyTrans.getModelMatrix() );
    hge::skybox::draw( skyTex, spherePrim );
}

/******************************************************************************
 * FONT PASS
******************************************************************************/
void arenaRenderApp::drawFonts() {
    overlayStr.setString(
        overlayFont,
        std::string( "Score: " + std::to_string( pGameLogic->getScore() ) ).c_str()
    );
    
    overlayFont.activate();
    applyMatrix( HGE_MODEL_MAT, overlayTrans.getModelMatrix() );
    overlayStr.draw();
    overlayFont.deActivate();
}

