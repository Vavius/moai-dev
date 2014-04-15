// Copyright (c) 2010-2014 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-box2d/MOAIBox2DParticle.h>
#include <Box2D/Particle/b2Particle.h>

//================================================================//
// MOAIBox2DParticle
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DParticle::RegisterLuaClass ( MOAILuaState& state ) {

    state.SetField ( -1, "WATER",                       ( u32 )b2_waterParticle );
    state.SetField ( -1, "ZOMBIE",                      ( u32 )b2_zombieParticle );
    state.SetField ( -1, "WALL",                        ( u32 )b2_wallParticle );
    state.SetField ( -1, "SPRING",                      ( u32 )b2_springParticle );
    state.SetField ( -1, "ELASTIC",                     ( u32 )b2_elasticParticle );
    state.SetField ( -1, "VISCOUS",                     ( u32 )b2_viscousParticle );
    state.SetField ( -1, "POWDER",                      ( u32 )b2_powderParticle );
    state.SetField ( -1, "TENSILE",                     ( u32 )b2_tensileParticle );
    state.SetField ( -1, "COLOR_MIXING",                ( u32 )b2_colorMixingParticle );
    state.SetField ( -1, "DESTRUCTION_LISTENER",        ( u32 )b2_destructionListenerParticle );
    state.SetField ( -1, "BARRIER",                     ( u32 )b2_barrierParticle );
    state.SetField ( -1, "STATIC_PRESSURE",             ( u32 )b2_staticPressureParticle );
    state.SetField ( -1, "REACTIVE",                    ( u32 )b2_reactiveParticle );
    state.SetField ( -1, "REPULSIVE",                   ( u32 )b2_repulsiveParticle );
    state.SetField ( -1, "FIXTURE_CONTACT_LISTENER",    ( u32 )b2_fixtureContactListenerParticle );
    state.SetField ( -1, "PARTICLE_CONTACT_LISTENER",   ( u32 )b2_particleContactListenerParticle );
    state.SetField ( -1, "FIXTURE_CONTACT_FILTER",      ( u32 )b2_fixtureContactFilterParticle );
    state.SetField ( -1, "PARTICLE_CONTACT_FILTER",     ( u32 )b2_particleContactFilterParticl );
    
}
