// Copyright (c) 2010-2014 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>

#include <moai-box2d/MOAIBox2DParticleGroup.h>


//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_applyForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "UNN" )

	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}
	
	b2Vec2 force;
	force.x = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	force.y = state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	self->mParticleGroup->ApplyForce ( force );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_applyLinearImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "UNN" )

	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}
	
	b2Vec2 impulse;
	impulse.x = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	impulse.y = state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	self->mParticleGroup->ApplyLinearImpulse ( impulse );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_containsParticle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}
	
	u32 index = state.GetValue < u32 >( 2, 0 );
	
	state.Push ( self->mParticleGroup->ContainsParticle ( index ) );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_destroyParticles ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}

	if ( self->mWorld->IsLocked () ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}

	self->mParticleGroup->mWorld->DestroyParticles ( self );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_getAngularVelocity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleGroup->GetAngularVelocity () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_getBufferIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleGroup->GetBufferIndex () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_getCenter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}

	b2Vec2 center = self->mParticleGroup->GetCenter ();
	state.Push ( center.x );
	state.Push ( center.y );
	return 2;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_getGroupFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleGroup->GetGroupFlags () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_getInertia ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleGroup->GetInertia () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_getLinearVelocity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}

	b2Vec2 velocity = self->mParticleGroup->GetLinearVelocity ();
	state.Push ( velocity.x );
	state.Push ( velocity.y );
	return 2;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_getMass ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleGroup->GetMass () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_getParticleCount ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )

	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}

	state.Push ( self->mParticleGroup->GetParticleCount () );
	return 1;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_setGroupFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )
	
	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}
	
	u32 flags = state.GetValue < float >( 2, 0 );
	
	self->mParticleGroup->SetGroupFlags ( flags );
	return 0;
}

//----------------------------------------------------------------//
/** TODO doxygen

*/
int MOAIBox2DParticleGroup::_toggleGroupFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DParticleGroup, "U" )
	
	if ( !self->mParticleGroup ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DParticleGroup_MissingInstance );
		return 0;
	}
	
	u32 flags = state.GetValue < float >( 2, 0 );
	flags = flags ^ self->mParticleGroup->GetGroupFlags ();

	self->mParticleGroup->SetGroupFlags ( flags );
	return 0;
}

//================================================================//
// MOAIBox2DParticleGroup
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DParticleGroup::Destroy () {

	if ( this->mParticleGroup ) {
		// particle Groups removed automatically from ParticleSystem
		// need not to do anything here
		this->mParticleGroup = 0;
	}
}

//----------------------------------------------------------------//
MOAIBox2DParticleGroup::MOAIBox2DParticleGroup () :
	mParticleGroup ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransformBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DParticleGroup::~MOAIBox2DParticleGroup () {

	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIBox2DParticleGroup::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITransformBase::RegisterLuaClass ( state );
	
	state.SetField ( -1, "SOLID",			( u32 )b2_solidParticleGroup );
	state.SetField ( -1, "RIGID",			( u32 )b2_rigidParticleGroup );
	state.SetField ( -1, "CAN_BE_EMPTY",	( u32 )b2_particleGroupCanBeEmpty );

}

//----------------------------------------------------------------//
void MOAIBox2DParticleGroup::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "applyForce",                 _applyForce },
		{ "applyLinearImpulse",         _applyLinearImpulse },
		{ "containsParticle",			_containsParticle },
		{ "destroyParticles",			_destroyParticles },
		{ "getAngularVelocity",         _getAngularVelocity },
		{ "getBufferIndex",             _getBufferIndex },
		{ "getCenter",					_getCenter },
		{ "getGroupFlags",				_getGroupFlags },
		{ "getInertia",                 _getInertia },
		{ "getLinearVelocity",			_getLinearVelocity },
		{ "getMass",					_getMass },
		{ "getParticleCount",			_getParticleCount },
		{ "setGroupFlags",				_setGroupFlags },
		{ "toggleGroupFlags",			_toggleGroupFlags },
		{ NULL, NULL }
	};
		
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DParticleGroup::SetParticleGroup ( b2ParticleGroup* particleGroup ) {
	this->mParticleGroup = particleGroup;
}

