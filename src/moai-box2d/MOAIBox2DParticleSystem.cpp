// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>

#include <moai-box2d/MOAIBox2DParticleSystem.h>

//----------------------------------------------------------------//
/** TODO doxygen
	void (int32 firstIndex, int32 lastIndex, const b2Vec2& force);
*/
int MOAIBox2DParticleSystem::_applyForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "UNNN" )

	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}
	
	b2Vec2 force;
	force.x = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	force.y = state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	u32 firstIndex = state.GetValue < u32 >( 4, 0 );
	u32 lastIndex = state.GetValue < u32 >( 5, firstIndex + 1 );

	self->mParticleSystem->ApplyForce ( firstIndex, lastIndex, force );
	return 0;
}


//----------------------------------------------------------------//
/** TODO doxygen
	void (int32 firstIndex, int32 lastIndex, const b2Vec2& impulse)
*/
int MOAIBox2DParticleSystem::_applyLinearImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "UNNN" )

	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}
	
	b2Vec2 impulse;
	impulse.x = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	impulse.y = state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	u32 firstIndex = state.GetValue < u32 >( 4, 0 );
	u32 lastIndex = state.GetValue < u32 >( 5, firstIndex + 1 );

	self->mParticleSystem->ApplyLinearImpulse ( firstIndex, lastIndex, impulse );
	return 0;	
}


//----------------------------------------------------------------//
/** TODO doxygen
	void (b2AABB* const aabb)
*/
int MOAIBox2DParticleSystem::_computeAABB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}
	
	b2AABB aabb;
	self->mParticleSystem->ComputeAABB ( &aabb );

	state.Push ( aabb.lowerBound.x / unitsToMeters );
	state.Push ( aabb.lowerBound.y / unitsToMeters );
	state.Push ( aabb.upperBound.x / unitsToMeters );
	state.Push ( aabb.upperBound.y / unitsToMeters );

	return 4;
}


//----------------------------------------------------------------//
/** TODO doxygen
	float32 ()
*/
int MOAIBox2DParticleSystem::_computeCollisionEnergy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->ComputeCollisionEnergy () );
	return 1;
}


//----------------------------------------------------------------//
/** TODO doxygen
	int32 ()

	flags = 0;
	position = b2Vec2_zero;
	velocity = b2Vec2_zero;
	color = b2ParticleColor_zero;
	lifetime = 0.0f;
	userData = NULL;
	group = NULL;
*/
int MOAIBox2DParticleSystem::_createParticle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	if ( self->mWorld->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}

	b2ParticleDef def;

	def.flags		= state.GetValue < u32 >( 2, 0 );
	def.position.x	= state.GetValue < float >( 3, 0 ) * unitsToMeters;
	def.position.y	= state.GetValue < float >( 4, 0 ) * unitsToMeters;
	def.velocity.x	= state.GetValue < float >( 5, 0 ) * unitsToMeters;
	def.velocity.y	= state.GetValue < float >( 6, 0 ) * unitsToMeters;
	def.lifetime	= state.GetValue < float >( 7, 0 );
	float r			= state.GetValue < float >( 8, 1 ) * 255;
	float g			= state.GetValue < float >( 9, 1 ) * 255;
	float b			= state.GetValue < float >( 10, 1 ) * 255;
	float a			= state.GetValue < float >( 11, 1 ) * 255;
	
	def.color.Set ( (u8) r, (u8) g, (u8) b, (u8) a );

	int particleIdx = self->mParticleSystem->CreateParticle ( def );
	state.Push ( particleIdx );
	return 1;
}


//----------------------------------------------------------------//
/** TODO implement

*/
int MOAIBox2DParticleSystem::_createParticleGroup ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen
 
 */
int MOAIBox2DParticleSystem::_destroy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}
	
	if ( self->mWorld ) {
		self->mWorld->ScheduleDestruction ( *self );
	}
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/	
int MOAIBox2DParticleSystem::_destroyOldestParticle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	u32 idx = state.GetValue < u32 >( 2, 0 );
	self->mParticleSystem->DestroyOldestParticle ( idx, false );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_destroyParticle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "UN" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}
	
	u32 idx = state.GetValue < u32 >( 2, 0 );
	self->mParticleSystem->DestroyParticle ( idx, false );
	return 0;
}

//----------------------------------------------------------------//
/** TODO implement

*/
int MOAIBox2DParticleSystem::_destroyParticlesInShape ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getDamping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetDamping () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getDensity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetDensity () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getDestructionByAge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetDestructionByAge () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getGravityScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetGravityScale () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getMaxParticleCount ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetMaxParticleCount() );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getParticleCount ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetParticleCount () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getParticleFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "UN" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	u32 idx = state.GetValue < u32 >( 2, 0 );
	state.Push ( self->mParticleSystem->GetParticleFlags ( idx ) );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getParticleGroupCount ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetParticleGroupCount () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO implement

*/
int MOAIBox2DParticleSystem::_getParticleGroupList ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getParticleLifetime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "UN" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	u32 idx = state.GetValue < u32 >( 2, 0 );
	state.Push ( self->mParticleSystem->GetParticleLifetime ( idx ) );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetPaused () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getRadius ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetRadius () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getStaticPressureIterations ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetStaticPressureIterations () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_getStrictContactCheck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleSystem->GetStrictContactCheck () );
	return 1;
}


//----------------------------------------------------------------//
/** TODO implement

*/
int MOAIBox2DParticleSystem::_getStuckCandidates ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	return 0;
}

//----------------------------------------------------------------//
/** TODO implement

*/
int MOAIBox2DParticleSystem::_joinParticleGroups ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	return 0;
}

//----------------------------------------------------------------//
/** TODO implement

*/
int MOAIBox2DParticleSystem::_queryAABB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	return 0;
}

//----------------------------------------------------------------//
/** TODO implement

*/
int MOAIBox2DParticleSystem::_rayCast ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

    return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setDamping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	float damping = state.GetValue < float >( 2, self->mParticleSystem->GetDamping () );
	self->mParticleSystem->SetDamping ( damping );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setDensity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	float density = state.GetValue < float >( 2, self->mParticleSystem->GetDensity() );
	self->mParticleSystem->SetDensity ( density );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setDestructionByAge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	bool destroyByAge = state.GetValue < bool >( 2, true );
	self->mParticleSystem->SetDestructionByAge ( destroyByAge );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setGravityScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	float gravScale = state.GetValue < float >( 2, self->mParticleSystem->GetGravityScale () );
	self->mParticleSystem->SetGravityScale ( gravScale );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setMaxParticleCount ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	u32 max = state.GetValue < u32 >( 2, self->mParticleSystem->GetMaxParticleCount () );
	self->mParticleSystem->SetMaxParticleCount ( max );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setParticleFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "UNN" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	u32 idx = state.GetValue < u32 > ( 2, 0 );
	u32 flags = state.GetValue < u32 >( 3, 0 );
	self->mParticleSystem->SetParticleFlags ( idx, flags );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setParticleLifetime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "UNN" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	u32 idx = state.GetValue < u32 >( 2, 0 );
	float lifetime = state.GetValue < float >( 3, 0.f );
	self->mParticleSystem->SetParticleLifetime ( idx, lifetime );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	bool paused = state.GetValue < bool >( 2, true );
	self->mParticleSystem->SetPaused ( paused );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setRadius ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	float radius = state.GetValue < float >( 2, self->mParticleSystem->GetRadius() );
	self->mParticleSystem->SetRadius ( radius );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setStaticPressureIterations ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	u32 iterations = state.GetValue < u32 >( 2, self->mParticleSystem->GetStaticPressureIterations () );
	self->mParticleSystem->SetStaticPressureIterations ( iterations );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setStrictContactCheck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	bool flag = state.GetValue < bool >( 2, true );
	self->mParticleSystem->SetStrictContactCheck ( flag );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleSystem::_setStuckThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleSystem, "U" )

	if ( !self->mParticleSystem ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleSystem_MissingInstance );
		return 0;
	}

	u32 threshold = state.GetValue < float >( 2, 0 );
	self->mParticleSystem->SetStuckThreshold ( threshold );
	return 0;
}




//================================================================//
// MOAIBox2DParticleSystem
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DParticleSystem::Destroy () {

	if ( this->mParticleSystem ) {
		b2World* world = this->mWorld->mWorld;
		world->DestroyParticleSystem ( this->mParticleSystem );
		this->mParticleSystem = 0;
	}
}

//----------------------------------------------------------------//
MOAIBox2DParticleSystem::MOAIBox2DParticleSystem () :
	mParticleSystem ( 0 ) {
	
	RTTI_BEGIN
		// RTTI_EXTEND (  )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DParticleSystem::~MOAIBox2DParticleSystem () {

	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIBox2DParticleSystem::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "WATER_PARTICLE", 						( u32 )WATER_PARTICLE );
	state.SetField ( -1, "ZOMBIE_PARTICLE", 					( u32 )ZOMBIE_PARTICLE );
	state.SetField ( -1, "WALL_PARTICLE", 						( u32 )WALL_PARTICLE );
	state.SetField ( -1, "SPRING_PARTICLE", 					( u32 )SPRING_PARTICLE );
	state.SetField ( -1, "ELASTIC_PARTICLE", 					( u32 )ELASTIC_PARTICLE );
	state.SetField ( -1, "VISCOUS_PARTICLE", 					( u32 )VISCOUS_PARTICLE );
	state.SetField ( -1, "POWDER_PARTICLE", 					( u32 )POWDER_PARTICLE );
	state.SetField ( -1, "TENSILE_PARTICLE", 					( u32 )TENSILE_PARTICLE );
	state.SetField ( -1, "COLOR_MIXING_PARTICLE", 				( u32 )COLOR_MIXING_PARTICLE );
	state.SetField ( -1, "DESTRUCTION_LISTENER_PARTICLE", 		( u32 )DESTRUCTION_LISTENER_PARTICLE );
	state.SetField ( -1, "BARRIER_PARTICLE", 					( u32 )BARRIER_PARTICLE );
	state.SetField ( -1, "STATIC_PRESSURE_PARTICLE", 			( u32 )STATIC_PRESSURE_PARTICLE );
	state.SetField ( -1, "REACTIVE_PARTICLE", 					( u32 )REACTIVE_PARTICLE );
	state.SetField ( -1, "REPULSIVE_PARTICLE", 					( u32 )REPULSIVE_PARTICLE );
	state.SetField ( -1, "FIXTURE_CONTACT_LISTENER_PARTICLE", 	( u32 )FIXTURE_CONTACT_LISTENER_PARTICLE );
	state.SetField ( -1, "PARTICLE_CONTACT_LISTENER_PARTICLE", 	( u32 )PARTICLE_CONTACT_LISTENER_PARTICLE );
	state.SetField ( -1, "FIXTURE_CONTACT_FILTER_PARTICLE", 	( u32 )FIXTURE_CONTACT_FILTER_PARTICLE );
	state.SetField ( -1, "PARTICLE_CONTACT_FILTER_PARTICLE", 	( u32 )PARTICLE_CONTACT_FILTER_PARTICLE );

}

//----------------------------------------------------------------//
void MOAIBox2DParticleSystem::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "_applyForce",					_applyForce 					},
		{ "_applyLinearImpulse",			_applyLinearImpulse 			},
		{ "_computeAABB",					_computeAABB 					},
		{ "_computeCollisionEnergy",		_computeCollisionEnergy 		},
		{ "_createParticle",				_createParticle 				},
		{ "_createParticleGroup",			_createParticleGroup 			},
		{ "_destroy",						_destroy 						},
		{ "_destroyOldestParticle",			_destroyOldestParticle 			},
		{ "_destroyParticle",				_destroyParticle 				},
		{ "_destroyParticlesInShape",		_destroyParticlesInShape 		},
		{ "_getDamping",					_getDamping 					},
		{ "_getDensity",					_getDensity 					},
		{ "_getDestructionByAge",			_getDestructionByAge 			},
		{ "_getGravityScale",				_getGravityScale 				},
		{ "_getMaxParticleCount",			_getMaxParticleCount 			},
		{ "_getParticleCount",				_getParticleCount 				},
		{ "_getParticleFlags",				_getParticleFlags 				},
		{ "_getParticleGroupCount",			_getParticleGroupCount 			},
		{ "_getParticleGroupList",			_getParticleGroupList 			},
		{ "_getParticleLifetime",			_getParticleLifetime 			},
		{ "_getPaused",						_getPaused 						},
		{ "_getRadius",						_getRadius 						},
		{ "_getStaticPressureIterations",	_getStaticPressureIterations 	},
		{ "_getStrictContactCheck",			_getStrictContactCheck 			},
		{ "_getStuckCandidates",			_getStuckCandidates 			},
		{ "_joinParticleGroups",			_joinParticleGroups 			},
		{ "_queryAABB",						_queryAABB 						},
		{ "_rayCast",						_rayCast 						},
		{ "_setDamping",					_setDamping 					},
		{ "_setDensity",					_setDensity 					},
		{ "_setDestructionByAge",			_setDestructionByAge 			},
		{ "_setGravityScale",				_setGravityScale 				},
		{ "_setMaxParticleCount",			_setMaxParticleCount 			},
		{ "_setParticleFlags",				_setParticleFlags 				},
		{ "_setParticleLifetime",			_setParticleLifetime 			},
		{ "_setPaused",						_setPaused 						},
		{ "_setRadius",						_setRadius 						},
		{ "_setStaticPressureIterations",	_setStaticPressureIterations 	},
		{ "_setStrictContactCheck",			_setStrictContactCheck 			},
		{ "_setStuckThreshold",				_setStuckThreshold 				},
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DParticleSystem::SetParticleSystem ( b2ParticleSystem* ps ) {
	this->mParticleSystem = ps;
}

