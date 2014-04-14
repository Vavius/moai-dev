// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBOX2DPARTICLESYSTEM_H
#define MOAIBOX2DPARTICLESYSTEM_H

#include <moai-box2d/MOAIBox2DWorld.h>


//================================================================//
// MOAIBox2DParticleSystem
//================================================================//
/** @name   MOAIBox2DParticleSystem
	@text   Box2D liquid fun particle system.
	
	@const  WATER_PARTICLE
	@const  ZOMBIE_PARTICLE
	@const  WALL_PARTICLE
	@const  SPRING_PARTICLE
	@const  ELASTIC_PARTICLE
	@const  VISCOUS_PARTICLE
	@const  POWDER_PARTICLE
	@const  TENSILE_PARTICLE
	@const  COLOR_MIXING_PARTICLE
	@const  DESTRUCTION_LISTENER_PARTICLE
	@const  BARRIER_PARTICLE
	@const  STATIC_PRESSURE_PARTICLE
	@const  REACTIVE_PARTICLE
	@const  REPULSIVE_PARTICLE
	@const  FIXTURE_CONTACT_LISTENER_PARTICLE
	@const  PARTICLE_CONTACT_LISTENER_PARTICLE
	@const  FIXTURE_CONTACT_FILTER_PARTICLE
	@const  PARTICLE_CONTACT_FILTER_PARTICLE
*/
class MOAIBox2DParticleSystem :
	public MOAIBox2DPrim {
private:
	
	b2ParticleSystem*			mParticleSystem;

	//----------------------------------------------------------------//
	static int 		_applyForce 					( lua_State* L );
	static int 		_applyLinearImpulse 			( lua_State* L );
	static int 		_computeAABB 					( lua_State* L );
	static int 		_computeCollisionEnergy 		( lua_State* L );
	static int 		_createParticle 				( lua_State* L );
	static int 		_createParticleGroup 			( lua_State* L );
	static int 		_destroy 						( lua_State* L );
	static int 		_destroyOldestParticle 			( lua_State* L );
	static int 		_destroyParticle 				( lua_State* L );
	static int 		_destroyParticlesInShape 		( lua_State* L );
	static int 		_getDamping 					( lua_State* L );
	static int 		_getDensity 					( lua_State* L );
	static int 		_getDestructionByAge 			( lua_State* L );
	static int 		_getGravityScale 				( lua_State* L );
	static int 		_getMaxParticleCount 			( lua_State* L );
	static int 		_getParticleCount 				( lua_State* L );
	static int 		_getParticleFlags 				( lua_State* L );
	static int 		_getParticleGroupCount 			( lua_State* L );
	static int 		_getParticleGroupList 			( lua_State* L );
	static int 		_getParticleLifetime 			( lua_State* L );
	static int 		_getPaused 						( lua_State* L );
	static int 		_getRadius 						( lua_State* L );
	static int 		_getStaticPressureIterations 	( lua_State* L );
	static int 		_getStrictContactCheck 			( lua_State* L );
	static int 		_getStuckCandidates 			( lua_State* L );
	static int 		_joinParticleGroups 			( lua_State* L );
	static int 		_queryAABB 						( lua_State* L );
	static int 		_rayCast 						( lua_State* L );
	static int 		_setDamping 					( lua_State* L );
	static int 		_setDensity 					( lua_State* L );
	static int 		_setDestructionByAge 			( lua_State* L );
	static int 		_setGravityScale 				( lua_State* L );
	static int 		_setMaxParticleCount 			( lua_State* L );
	static int 		_setParticleFlags 				( lua_State* L );
	static int 		_setParticleLifetime 			( lua_State* L );
	static int 		_setPaused 						( lua_State* L );
	static int 		_setRadius 						( lua_State* L );
	static int 		_setStaticPressureIterations 	( lua_State* L );
	static int 		_setStrictContactCheck 			( lua_State* L );
	static int 		_setStuckThreshold 				( lua_State* L );
	//----------------------------------------------------------------//
	void			SetParticleSystem				( b2ParticleSystem* ps );

public:

	friend class MOAIBox2DWorld;
	
	DECL_LUA_FACTORY ( MOAIBox2DParticleSystem )
		
	enum {
		WATER_PARTICLE						= b2_waterParticle,
		ZOMBIE_PARTICLE						= b2_zombieParticle,
		WALL_PARTICLE						= b2_wallParticle,
		SPRING_PARTICLE						= b2_springParticle,
		ELASTIC_PARTICLE					= b2_elasticParticle,
		VISCOUS_PARTICLE					= b2_viscousParticle,
		POWDER_PARTICLE						= b2_powderParticle,
		TENSILE_PARTICLE					= b2_tensileParticle,
		COLOR_MIXING_PARTICLE				= b2_colorMixingParticle,
		DESTRUCTION_LISTENER_PARTICLE		= b2_destructionListenerParticle,
		BARRIER_PARTICLE					= b2_barrierParticle,
		STATIC_PRESSURE_PARTICLE			= b2_staticPressureParticle,
		REACTIVE_PARTICLE					= b2_reactiveParticle,
		REPULSIVE_PARTICLE					= b2_repulsiveParticle,
		FIXTURE_CONTACT_LISTENER_PARTICLE	= b2_fixtureContactListenerParticle,
		PARTICLE_CONTACT_LISTENER_PARTICLE	= b2_particleContactListenerParticle,
		FIXTURE_CONTACT_FILTER_PARTICLE		= b2_fixtureContactFilterParticle,
		PARTICLE_CONTACT_FILTER_PARTICLE	= b2_particleContactFilterParticle
	};
	
	//----------------------------------------------------------------//
	void			Destroy						();
					MOAIBox2DParticleSystem		();
					~MOAIBox2DParticleSystem	();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};


#endif
