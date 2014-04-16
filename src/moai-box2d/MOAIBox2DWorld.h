// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DWORLD_H
#define	MOAIBOX2DWORLD_H

#include <Box2D/Box2D.h>

class b2World;
class MOAIBox2DArbiter;
class MOAIBox2DBody;
class MOAIBox2DDebugDraw;
class MOAIBox2DFixture;
class MOAIBox2DJoint;
class MOAIBox2DParticleSystem;
class MOAIBox2DWorld;
class MOAIBox2DRayCastCallback;

//================================================================//
// MOAIBox2DPrim
//================================================================//
class MOAIBox2DPrim :
	public virtual MOAILuaObject  {
protected:

	MOAIBox2DWorld* mWorld;
	
	bool			mDestroy;
	MOAIBox2DPrim*	mDestroyNext;

	//----------------------------------------------------------------//
	
public:

	friend class MOAIBox2DWorld;

	GET_SET ( MOAIBox2DWorld*, World, mWorld )

	//----------------------------------------------------------------//
	virtual void	Destroy					() = 0;
	float			GetUnitsToMeters		();
					MOAIBox2DPrim			();
	
	//----------------------------------------------------------------//
	inline bool IsDestroyed () {
		return this->mDestroy;
	}
};

//================================================================//
// MOAIBox2DWorld
//================================================================//
/**	@name	MOAIBox2DWorld
	@text	Box2D world.
	
	@const DEBUG_DRAW_SHAPES
	@const DEBUG_DRAW_JOINTS
	@const DEBUG_DRAW_BOUNDS
	@const DEBUG_DRAW_PAIRS
	@const DEBUG_DRAW_CENTERS
	@const DEBUG_DRAW_DEFAULT
	@const DEBUG_DRAW_PARTICLES
*/
class MOAIBox2DWorld :
	public MOAIAction,
	public b2DestructionListener {
private:

	bool						mLock;

	b2World*					mWorld;
	MOAIBox2DDebugDraw*			mDebugDraw;
	
	MOAILuaSharedPtr < MOAIBox2DArbiter > mArbiter;

	u32		mVelocityIterations;
	u32		mPositionIterations;
	u32		mParticleIterations;
	
	float	mUnitsToMeters; // maps world space units to meters

	MOAIBox2DPrim*		mDestroyBodies;
	MOAIBox2DPrim*		mDestroyFixtures;
	MOAIBox2DPrim*		mDestroyJoints;
	MOAIBox2DPrim*		mDestroyParticleSystems;

	//----------------------------------------------------------------//
	static int		_addBody					( lua_State* L );
	static int		_addDistanceJoint			( lua_State* L );
	static int		_addFrictionJoint			( lua_State* L );
	static int		_addGearJoint				( lua_State* L );
	static int 		_addMotorJoint				( lua_State* L );
	static int		_addMouseJoint				( lua_State* L );
	static int 		_addParticleSystem 			( lua_State* L );
	static int		_addPrismaticJoint			( lua_State* L );
	static int		_addPulleyJoint				( lua_State* L );
	static int		_addRevoluteJoint			( lua_State* L );
	static int		_addRopeJoint				( lua_State* L );
	static int		_addWeldJoint				( lua_State* L );
	static int		_addWheelJoint				( lua_State* L );
	static int 		_calculateParticleIterations( lua_State* L );
	static int		_getAngularSleepTolerance	( lua_State* L );
	static int		_getAutoClearForces			( lua_State* L );
	static int		_getGravity					( lua_State* L );
	static int		_getLinearSleepTolerance	( lua_State* L );
	static int		_getTimeToSleep				( lua_State* L );
	static int		_setAngularSleepTolerance	( lua_State* L );
	static int		_setAutoClearForces			( lua_State* L );
	static int		_setDebugDrawEnabled		( lua_State* L );
	static int		_setDebugDrawFlags			( lua_State* L );
	static int		_setGravity					( lua_State* L );
	static int		_setIterations				( lua_State* L );
	static int		_setLinearSleepTolerance	( lua_State* L );
	static int		_setTimeToSleep				( lua_State* L );
	static int		_setUnitsToMeters			( lua_State* L );
	static int		_getRayCast					( lua_State* L );
	
	//----------------------------------------------------------------//
	void			Destroy					();
	void			SayGoodbye				( b2Fixture* fixture ); 
	void			SayGoodbye				( b2Joint* joint );
    void            SayGoodbye              ( b2ParticleGroup* group );
	void			ScheduleDestruction		( MOAIBox2DBody& body );
	void			ScheduleDestruction		( MOAIBox2DFixture& fixture );
	void			ScheduleDestruction		( MOAIBox2DJoint& joint );
	void			ScheduleDestruction		( MOAIBox2DParticleSystem& particleSystem );

public:
	
	friend class MOAIBox2DBody;
	friend class MOAIBox2DFixture;
	friend class MOAIBox2DJoint;
    friend class MOAIBox2DParticleSystem;
	
	DECL_LUA_FACTORY ( MOAIBox2DWorld )
	
	GET_SET ( float, UnitsToMeters, mUnitsToMeters )
	GET_CONST ( float, UnitsToMeters, mUnitsToMeters )
	
	// aliases for box2D bitflags
	static const u32 DEBUG_DRAW_SHAPES		= b2Draw::e_shapeBit;
	static const u32 DEBUG_DRAW_JOINTS		= b2Draw::e_jointBit;
	static const u32 DEBUG_DRAW_BOUNDS		= b2Draw::e_aabbBit;
	static const u32 DEBUG_DRAW_PAIRS		= b2Draw::e_pairBit;
	static const u32 DEBUG_DRAW_CENTERS		= b2Draw::e_centerOfMassBit;
    static const u32 DEBUG_DRAW_PARTICLES   = b2Draw::e_particleBit;
	
	static const u32 DEBUG_DRAW_DEFAULT = DEBUG_DRAW_SHAPES | DEBUG_DRAW_JOINTS | DEBUG_DRAW_CENTERS | DEBUG_DRAW_PARTICLES;
	
	//----------------------------------------------------------------//
	void			DrawDebug				();
	bool			IsDone					();
	bool			IsLocked				();
					MOAIBox2DWorld			();
					~MOAIBox2DWorld			();
	void			OnUpdate				( float step );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

//================================================================//
// MOAIBox2DRayCastCallback
//================================================================//
class MOAIBox2DRayCastCallback : public b2RayCastCallback
{
public:
  MOAIBox2DRayCastCallback() {
     m_fixture = NULL;
     m_point.SetZero();
     m_normal.SetZero();
  }
 
  float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
     m_fixture = fixture;
     m_point = point;
     m_normal = normal;
     
     return fraction;
  }
 
  b2Fixture* m_fixture;
  b2Vec2 m_point;
  b2Vec2 m_normal;
};

#endif
