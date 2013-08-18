
#include <iostream>

#include "main.h"
#include "renderApp.h"
#include "logicApp.h"

hge::window*        pWindow     = nullptr;
hge::inputSystem*   pInput      = nullptr;
arenaRenderApp*     pRenderer   = nullptr;
arenaLogicApp*      pGameLogic  = nullptr;

/*
 * Game Initializations
 */
bool init() {
    if ( !hge::init() ) {
        std::cerr << "Error: Unable to initialize Harbinger." << std::endl;
        return -1;
    }
    
    try {
        pWindow     = new hge::window       ( 1280, 720, true, nullptr, false );
        pInput      = new hge::inputSystem  ( *pWindow );
        pRenderer   = new arenaRenderApp    ();
        pGameLogic  = new arenaLogicApp     ();
        
        HL_ASSERT( pRenderer->init() );
    }
    catch( hamLibs::utils::errorType& e ) {
        std::cerr
            << "Unable to initialize " << GAME_TITLE << ". Please "
            << "Check all initialization routines for errors." << std::endl;
        return false;
    }
    
    pInput->showCursor( false );
    
    return true;
}

/*
 * Game Termination
 */
void terminate() {
    delete      pWindow;
    delete      pInput;
    delete      pRenderer;
    delete      pGameLogic;
    
    pWindow     = nullptr;
    pInput      = nullptr;
    pRenderer   = nullptr;
    pGameLogic  = nullptr;
    
    hge::terminate();
}

/******************************************************************************
 * Main
******************************************************************************/
int main() {
    std::ios_base::sync_with_stdio(false);
    
    if ( !init() ) {
        terminate();
        return false;
    }
    
    pWindow->setTitle( GAME_TITLE );
    pGameLogic->startRunning();
    pRenderer->startRunning();
    
    while ( pRenderer->isRunning() && !pWindow->wantsToClose() ) {
        pGameLogic->tick();
        pRenderer->tick();
        pWindow->flip();
    }
    
    terminate();
    
    std::cout
        << "Program terminated successfully. "
        << " Run time: " << hamLibs::utils::timer::progUpTime() << " seconds."
        << std::endl;
    return 0;
}
