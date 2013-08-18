/* 
 * File:   objects.h
 * Author: hammy
 *
 * Created on July 14, 2013, 4:35 PM
 */

#ifndef __POING_OBJECTS_H__
#define	__POING_OBJECTS_H__

#include <vector>
#include "utils/randomNum.h"
#include "main.h"
/*
 * GAME OBJECTS
 * 
 * All game objects with resources (files) contain pointers to data held in
 * a resource manager. No game object should have any memory dynamically
 * allocated to it.
 */

struct entity {
    float       radius;
    float       massInv;
    float       bounciness;
    math::vec3  pos;
    math::vec3  vel;
    math::vec3  acc;
};

// Basic Enemy types
struct enemy : entity {
    bool isActive;
};

// basic bullet types
struct bullet : entity {
    hge::drawTransform  trans;
};

typedef std::vector< bullet >   bulletList_t;
typedef std::vector< enemy >    enemyList_t;

class arenaObjMgr {
    friend class arenaRenderApp;
    friend class arenaLogicApp;
    
    private:
        bulletList_t    bulletList;
        enemyList_t     enemyList;
        utils::randomNum randGenerator;
        
    public:
        arenaObjMgr();
        ~arenaObjMgr() {}
        
        void    emitEnemy();
        
        const bulletList_t& getBulletList() const { return bulletList; }
        const enemyList_t& getEnemyList() const { return enemyList; }
        
        void    launchBullet        ( const math::vec3& pos, const math::vec3& dir );
        void    updateBulletPositions( float timeDelta );
        void    updateEnemyPositions( float timeDelta );

};
#endif	/* __POING_OBJECTS_H__ */
