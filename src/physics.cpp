/* 
 * File:   physicsBase.cpp
 * Author: hammy
 * 
 * Created on May 31, 2013, 12:09 AM
 */

/*
 * NOTE:
 * A lot of the code for collision detection was adapted from the
 * "How to Create a Custom 2D Physics Engine" series found on
 * gamedev.tutsplus.com
 * 
 * http://gamedev.tutsplus.com/series/custom-game-physics-engine/
 */

#include "main.h"
#include "physics.h"

//-----------------------------------------------------------------------------
//      Update an entity's position using verlet integration
//-----------------------------------------------------------------------------
void physics::updatePosition( float deltaTime, entity& e, const math::vec3& force ) {
/*
    // Verlet integration
    math::vec3 tempAcc = e.acc * deltaTime;
    e.pos += (e.vel + (tempAcc * 0.5f)) * deltaTime;
    e.vel += tempAcc;
*/
    e.pos += (e.vel * deltaTime) + (e.acc * 0.5f * deltaTime * deltaTime);
    
    math::vec3 newAccel = force;
    newAccel *= e.massInv;
    
    math::vec3 avgAccel = e.acc;
    avgAccel += newAccel;
    avgAccel *= 0.5f;
    
    e.vel += avgAccel * deltaTime;
    e.acc += newAccel;
}

//-----------------------------------------------------------------------------
//      AABB Collision test
//-----------------------------------------------------------------------------
bool physics::checkCollision( const aabb& a, const aabb& b ) {
    if ( a.max[0] < b.min[0] || a.min[0] > b.max[0] ) return false;
    if ( a.max[1] < b.min[1] || a.min[1] > b.max[1] ) return false;
    if ( a.max[2] < b.min[2] || a.min[2] > b.max[2] ) return false;
    return true;
}

//-----------------------------------------------------------------------------
//      Spherical Collision test
//-----------------------------------------------------------------------------
bool physics::checkCollision( const entity& a, const entity& b ) {
    float radius = a.radius + b.radius;
    radius *= radius;
    
    math::vec3&& distVec = a.pos - b.pos;
    distVec *= distVec;
    
    float distance = distVec[0] + distVec[1] + distVec[2];
    
    return radius >= distance;
}

//-----------------------------------------------------------------------------
//      Entity collision resolution
//-----------------------------------------------------------------------------
void physics::applyImpulse( entity& a, entity& b ) {
    
    // calculate the relative velocity
    const math::vec3&& bounceVec = b.vel - a.vel;
    
    // Don't collide if the objects are moving away from each other
    if ( math::dot( bounceVec, math::vec3( 0.f, 1.f, 0.f ) ) > 0.f )
        return;
    
    //calculate the object's bounce
    float restitution = std::min( a.bounciness, b.bounciness );
    
    // calculate the impulse
    math::vec3&& impulse = bounceVec * -( 1.f + restitution );
    impulse /= a.massInv + b.massInv;
    
    // apply the impulse
    a.vel -= impulse * a.massInv;
    b.vel += impulse * b.massInv;
    
}