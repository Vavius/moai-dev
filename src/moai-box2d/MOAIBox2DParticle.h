// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBOX2DPARTICLE_H
#define MOAIBOX2DPARTICLE_H

//================================================================//
// MOAIBox2DParticle
//================================================================//
/** @name   MOAIBox2DParticle
    @text   LiquidFun particle flags
    
    @const  WATER
    @const  ZOMBIE
    @const  WALL
    @const  SPRING
    @const  ELASTIC
    @const  VISCOUS
    @const  POWDER
    @const  TENSILE
    @const  COLOR_MIXING
    @const  DESTRUCTION_LISTENER
    @const  BARRIER
    @const  STATIC_PRESSURE
    @const  REACTIVE
    @const  REPULSIVE
    @const  FIXTURE_CONTACT_LISTENER
    @const  PARTICLE_CONTACT_LISTENER
    @const  FIXTURE_CONTACT_FILTER
    @const  PARTICLE_CONTACT_FILTER
*/

class MOAIBox2DParticle :
    public MOAIGlobalClass < MOAIBox2DParticle, MOAILuaObject > {
public:
    
    DECL_LUA_SINGLETON ( MOAIBox2DParticle )
    
    //----------------------------------------------------------------//
    void                RegisterLuaClass        ( MOAILuaState& state );
};

#endif
