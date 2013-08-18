/* 
 * File:   physics.h
 * Author: hammy
 *
 * Created on May 31, 2013, 12:09 AM
 */

#ifndef __POING_PHYSICS_H__
#define	__POING_PHYSICS_H__

#include "main.h"
#include "objects.h"

namespace physics {

struct aabb {
    math::vec3 min;
    math::vec3 max;
};

void updatePosition( float deltaTime, entity&, const math::vec3& force = math::vec3(0.f, 0.f, 0.f) );

bool checkCollision( const aabb&, const aabb& );
bool checkCollision( const entity&, const entity& );

void applyImpulse( entity&, entity& );

} // end physics namespace

#endif	/* __POING_PHYSICS_H__ */

