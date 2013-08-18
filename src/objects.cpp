
#include <iostream>
#include "objects.h"
#include "physics.h"
#include "logicApp.h"


arenaObjMgr::arenaObjMgr() {
    randGenerator.seed();
    enemyList.resize( MAX_ENEMIES );
    
    for ( int i = 0; i < MAX_ENEMIES; ++i ) {
        enemy& e = enemyList[ i ];

        e.bounciness = ENEMY_BOUNCINESS;
        e.massInv = 1.f / ENEMY_MASS;
        e.radius = 0.5f;
        e.pos = math::vec3( 0.f, MAX_LEVEL_DEPTH, 0.f );
        e.vel = math::vec3( 0.f );
        e.acc = math::vec3( 0.f );
        e.isActive = false;
    }
}

//-----------------------------------------------------------------------------
//      Launching a bullet and placing it into a bullet list
//-----------------------------------------------------------------------------
void arenaObjMgr::launchBullet( const math::vec3& pos, const math::vec3& dir ) {
    // launch a bullet if the left mouse button was clicked
    bullet b;
    
    b.radius = BULLET_SCALE_FACTOR;
    b.massInv = 1.f / BULLET_MASS;
    b.bounciness = BULLET_BOUNCINESS;
    
    math::vec3 spawnPos = pos;
    spawnPos[1] = BULLET_SCALE_FACTOR;
        
    b.pos = spawnPos * math::vec3( ARENA_SIZE / math::magnitude( spawnPos ) );
    b.vel = math::normalize( dir ) * -BULLET_VELOCITY;
    b.vel[1] = 0.f;
    b.acc = math::vec3( 0.f );
    
    b.trans.setScaleUniform( BULLET_SCALE_FACTOR );
    b.trans.setPos( pos );
    b.trans.update();
    
    bulletList.push_back( b );
}

//-----------------------------------------------------------------------------
//      Launching Enemies
//-----------------------------------------------------------------------------
void arenaObjMgr::emitEnemy() {
    // get the next available enemy from the pool
    int i = 0;
    while ( i < MAX_ENEMIES ) {
        if ( !enemyList[ i ].isActive ) {
            break;
        }
        ++i;
    }
    if ( i == MAX_ENEMIES )
        return;
    
    enemy& e = enemyList[ i ];
    
    float xDir = randGenerator.randRangeF( -1.f, 1.f );
    float zDir = randGenerator.randRangeF( -1.f, 1.f );
    
    e.pos = math::vec3( 0.f );
    e.vel = math::vec3( xDir, 0.f, zDir );
    e.vel *= ENEMY_VELOCITY;
    e.isActive = true;
}

//-----------------------------------------------------------------------------
//      updating the bullets over time within a bullet list
//-----------------------------------------------------------------------------
void arenaObjMgr::updateBulletPositions( float timeDelta ) {
    for ( unsigned b = 0; b < bulletList.size(); ++b ) {
        
        bullet& pBullet = bulletList[ b ];
        math::vec3& pPos = pBullet.pos;
        
        physics::updatePosition( timeDelta, pBullet );
        
        // if the bullet is in the arena, don't accelerate due to gravity
        float bulletArenaPos = (pPos[0] * pPos[0]) + (pPos[2] * pPos[2]);
        // Use the bullet's radius to make sure the bullets don't clip
        // through the arena floor. Let gravity take over otherwise
        if ( bulletArenaPos + pBullet.radius < (ARENA_SIZE*ARENA_SIZE) ) {
            pPos[1] = pBullet.radius;
            pBullet.acc[1] = 0.f;
        }
        else {
            pBullet.acc[1] += GRAVITY_FACTOR;
        }
        
        //resolve all bullet collisions
        for ( unsigned i = 0; i < bulletList.size(); ++i ) {
            if ( i != b ) {
                if ( physics::checkCollision( pBullet, bulletList[ i ] ) ) {
                    physics::applyImpulse( pBullet, bulletList[ i ] );
                }
            }
        }
        
        // Determine if the bullet should be deleted
        if ( pPos[1] > MAX_LEVEL_DEPTH ) {
            pBullet.trans.setPos( pPos );
            pBullet.trans.update();
        }
        else {
            bulletList.erase( bulletList.begin() + b );
        }
    }
}

//-----------------------------------------------------------------------------
//      updating the enemy list
//-----------------------------------------------------------------------------
void arenaObjMgr::updateEnemyPositions( float timeDelta ) {
    for ( unsigned i = 0; i < enemyList.size(); ++i ) {
        if ( !enemyList[ i ].isActive )
            continue;
        
        enemy& pEnemy = enemyList[ i ];
        math::vec3& pPos = pEnemy.pos;
        
        physics::updatePosition( timeDelta, pEnemy );
        
        // if the enemy is out of the arena, move it out of the camera's view
        float enemyDist = (pPos[0] * pPos[0]) + (pPos[2] * pPos[2]);
        
        if ( enemyDist >= (ARENA_SIZE*ARENA_SIZE) ) {
            pPos = math::vec3( 0.f, MAX_LEVEL_DEPTH, 0.f );
            pEnemy.vel = math::vec3( 0.f, 0.f, 0.f );
            pEnemy.isActive = false;
            continue;
        }
        
        //resolve all bullet collisions
        for ( unsigned j = 0; j < bulletList.size(); ++j ) {
            if ( physics::checkCollision( pEnemy, bulletList[ j ] ) ) {
                physics::applyImpulse( pEnemy, bulletList[ j ] );
                pGameLogic->setScore( pGameLogic->getScore() + 1 );
                pEnemy.isActive = false;
                pEnemy.pos[1] = MAX_LEVEL_DEPTH;
                pEnemy.vel = 0.f;
                //bulletList.erase( bulletList.begin() + j );
            }
        }
    }
}
