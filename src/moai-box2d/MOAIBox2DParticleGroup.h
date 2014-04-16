// Copyright (c) 2010-2014 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBOX2DPARTICLEGROUP_H
#define MOAIBOX2DPARTICLEGROUP_H

#include <moai-box2d/MOAIBox2DWorld.h>


//================================================================//
// MOAIBox2DParticleGroup
//================================================================//
/** @name   MOAIBox2DParticleGroup
	@text   Box2D liquid fun particle system.
	
	@const	SOLID
	@const	RIGID
	@const	CAN_BE_EMPTY

*/
class MOAIBox2DParticleGroup :
	public MOAIBox2DPrim,
	public MOAITransform {
private:
	
	b2ParticleGroup*           mParticleGroup;

	//----------------------------------------------------------------//
	static int      _applyForce                 ( lua_State* L );
	static int      _applyLinearImpulse         ( lua_State* L );
	static int      _containsParticle           ( lua_State* L );
	static int      _destroyParticles           ( lua_State* L );
	static int      _getAngularVelocity         ( lua_State* L );
	static int      _getBufferIndex             ( lua_State* L );
	static int      _getCenter                  ( lua_State* L );
	static int      _getGroupFlags              ( lua_State* L );
	static int      _getInertia                 ( lua_State* L );
	static int      _getLinearVelocity          ( lua_State* L );
	static int      _getMass                    ( lua_State* L );
	static int      _getParticleCount           ( lua_State* L );
	static int      _setGroupFlags              ( lua_State* L );
	static int      _toggleGroupFlags           ( lua_State* L );

	//----------------------------------------------------------------//
	void            SetParticleGroup            ( b2ParticleGroup* particleGroup );

public:

	friend class MOAIBox2DParticleSystem;
    friend class MOAIBox2DWorld;
	
	DECL_LUA_FACTORY ( MOAIBox2DParticleGroup )


	//----------------------------------------------------------------//
    bool            ApplyAttrOp                 ( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	void            Destroy                     ();
    void            OnDepNodeUpdate             ();
					MOAIBox2DParticleGroup      ();
					~MOAIBox2DParticleGroup     ();
	void            RegisterLuaClass            ( MOAILuaState& state );
	void            RegisterLuaFuncs            ( MOAILuaState& state );
};


#endif
