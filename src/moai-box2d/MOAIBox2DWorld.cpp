// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DDebugDraw.h>
#include <moai-box2d/MOAIBox2DDistanceJoint.h>
#include <moai-box2d/MOAIBox2DFixture.h>
#include <moai-box2d/MOAIBox2DFrictionJoint.h>
#include <moai-box2d/MOAIBox2DGearJoint.h>
#include <moai-box2d/MOAIBox2DJoint.h>
#include <moai-box2d/MOAIBox2DMotorJoint.h>
#include <moai-box2d/MOAIBox2DMouseJoint.h>
#include <moai-box2d/MOAIBox2DParticleGroup.h>
#include <moai-box2d/MOAIBox2DParticleSystem.h>
#include <moai-box2d/MOAIBox2DPrismaticJoint.h>
#include <moai-box2d/MOAIBox2DPulleyJoint.h>
#include <moai-box2d/MOAIBox2DRevoluteJoint.h>
#include <moai-box2d/MOAIBox2DRopeJoint.h>
#include <moai-box2d/MOAIBox2DWeldJoint.h>
#include <moai-box2d/MOAIBox2DWheelJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// MOAIBox2DPrim
//================================================================//

//----------------------------------------------------------------//
float MOAIBox2DPrim::GetUnitsToMeters () {

	if ( this->mWorld ) {
		return this->mWorld->GetUnitsToMeters ();
	}
	return 1.0f;
}

//----------------------------------------------------------------//
MOAIBox2DPrim::MOAIBox2DPrim () :
	mWorld ( 0 ),
	mDestroy ( false ),
	mDestroyNext ( 0 ) {
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	addBody
	@text	Create and add a body to the world.
	
	@in		MOAIBox2DWorld self
	@in		number type		One of MOAIBox2DBody.DYNAMIC, MOAIBox2DBody.KINEMATIC, MOAIBox2DBody.STATIC
	@opt	number x	in units, in world coordinates, converted to meters
	@opt	number y	in units, in world coordinates, converted to meters
	@out	MOAIBox2DBody joint
*/
int MOAIBox2DWorld::_addBody ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	u32 type	= state.GetValue < u32 >( 2, 0 );
	float x		= state.GetValue < float >( 3, 0.0f ) * self->mUnitsToMeters;
	float y		= state.GetValue < float >( 4, 0.0f ) * self->mUnitsToMeters;
	
	b2BodyDef groundBodyDef;
	groundBodyDef.type = ( b2BodyType )type;
	groundBodyDef.position.Set ( x, y );
	
	MOAIBox2DBody* body = new MOAIBox2DBody ();
	body->SetBody ( self->mWorld->CreateBody ( &groundBodyDef ));
	body->SetWorld ( self );
	self->LuaRetain ( body );
	
	body->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addDistanceJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorA_X	in units, in world coordinates, converted to meters
	@in		number anchorA_Y	in units, in world coordinates, converted to meters
	@in		number anchorB_X	in units, in world coordinates, converted to meters
	@in		number anchorB_Y	in units, in world coordinates, converted to meters
	@opt	number frequencyHz			in Hz. Default value determined by Box2D
	@opt	number dampingRatio			Default value determined by Box2D
	@opt	boolean collideConnected		Default value is false
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addDistanceJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchorA;
	anchorA.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchorA.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 anchorB;
	anchorB.x	= state.GetValue < float >( 6, 0 ) * self->mUnitsToMeters;
	anchorB.y	= state.GetValue < float >( 7, 0 ) * self->mUnitsToMeters;
	
	b2DistanceJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchorA, anchorB );
	
	jointDef.frequencyHz	= state.GetValue < float >( 8, jointDef.frequencyHz );
	jointDef.dampingRatio	= state.GetValue < float >( 9, jointDef.dampingRatio );
	jointDef.collideConnected = state.GetValue < bool >( 10, false );
	
	MOAIBox2DDistanceJoint* joint = new MOAIBox2DDistanceJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addFrictionJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorX	in units, in world coordinates, converted to meters
	@in		number anchorY	in units, in world coordinates, converted to meters
	@opt	number maxForce			in kg * units / s^2, converted to N [kg * m / s^2]. 	Default value determined by Box2D
	@opt	number maxTorque		in kg * units / s^2 * units, converted to N-m [kg * m / s^2 * m].	Default value determined by Box2D
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addFrictionJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2FrictionJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor );

	float unitsToMeters = self->GetUnitsToMeters();

	jointDef.maxForce	= state.GetValue < float >( 6, jointDef.maxForce / unitsToMeters ) * unitsToMeters;
	/* Convert to/from N-m (kg m / s^2) * m from/to (kg unit / s^2) * unit */
	jointDef.maxTorque	= state.GetValue < float >( 7, jointDef.maxTorque / ( unitsToMeters * unitsToMeters ) ) * unitsToMeters * unitsToMeters;
	
	MOAIBox2DFrictionJoint* joint = new MOAIBox2DFrictionJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addGearJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DJoint jointA
	@in		MOAIBox2DJoint jointB
	@in		number ratio
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addGearJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DJoint* jointA = state.GetLuaObject < MOAIBox2DJoint >( 2, true );
	MOAIBox2DJoint* jointB = state.GetLuaObject < MOAIBox2DJoint >( 3, true );
	
	if ( !( jointA && jointB )) return 0;
	
	b2GearJointDef jointDef;
	
	jointDef.bodyA	= jointA->mJoint->GetBodyA ();
	jointDef.bodyB	= jointB->mJoint->GetBodyB ();
	jointDef.joint1	= jointA->mJoint;
	jointDef.joint2	= jointB->mJoint;
	jointDef.ratio	= state.GetValue < float >( 4, 0.0f );
	
	MOAIBox2DGearJoint* joint = new MOAIBox2DGearJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( joint );
	
	joint->mJointA.Set ( *self, jointA );
	joint->mJointB.Set ( *self, jointB );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/** @name	addMotorJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@out	MOAIBox2DJoint joint
*/
int MOAIBox2DWorld::_addMotorJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUU" )

	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2MotorJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody );
	
	MOAIBox2DMotorJoint* joint = new MOAIBox2DMotorJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addMouseJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number targetX	in units, in world coordinates, converted to meters
	@in		number targetY	in units, in world coordinates, converted to meters
	@in		number maxForce	in kg * units / s^2, converted to N [kg * m / s^2].
	@opt	number frequencyHz			in Hz. Default value determined by Box2D
	@opt	number dampingRatio			Default value determined by Box2D
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addMouseJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 target;
	target.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	target.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2MouseJointDef jointDef;
	jointDef.bodyA			= bodyA->mBody;
	jointDef.bodyB			= bodyB->mBody;
	jointDef.target			= target;
	jointDef.maxForce		= state.GetValue < float >( 6, 0.0f ) * self->mUnitsToMeters;
	jointDef.frequencyHz	= state.GetValue < float >( 7, jointDef.frequencyHz );
	jointDef.dampingRatio	= state.GetValue < float >( 8, jointDef.dampingRatio );
	
	MOAIBox2DMouseJoint* joint = new MOAIBox2DMouseJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addParticleSystem
	@text	Create and add particle system to the world. See LiquidFun documentation.
			Parameters are passed in table. All params are optional and use meters not units:
				radius = 1.0f
				pressureStrength = 0.05f
				dampingStrength = 1.0f
				elasticStrength = 0.25f
				springStrength = 0.25f
				viscousStrength = 0.25f
				surfaceTensionPressureStrength = 0.2f
				surfaceTensionNormalStrength = 0.2f
				repulsiveStrength = 1.0f
				powderStrength = 0.5f
				ejectionStrength = 0.5f
				staticPressureStrength = 0.2f
				staticPressureRelaxation = 0.2f
				staticPressureIterations = 8
				colorMixingStrength = 0.5f
				destroyByAge = true
				lifetimeGranularity = 1.0f / 60.0f

	@in		MOAIBox2DWorld 	self
	@opt 	table 			parameters
	@out	MOAIBox2DJoint 	joint
*/
int	MOAIBox2DWorld::_addParticleSystem ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	b2ParticleSystemDef def;

	if ( state.IsType ( 2, LUA_TTABLE )) {
		def.radius 							= state.GetField < float >( 2, "radius", 1.0f ) * self->mUnitsToMeters;
		def.pressureStrength 				= state.GetField < float >( 2, "pressureStrength", 0.05f );
		def.dampingStrength 				= state.GetField < float >( 2, "dampingStrength", 1.0f );
		def.elasticStrength 				= state.GetField < float >( 2, "elasticStrength", 0.25f );
		def.springStrength 					= state.GetField < float >( 2, "springStrength", 0.25f );
		def.viscousStrength 				= state.GetField < float >( 2, "viscousStrength", 0.25f );
		def.surfaceTensionPressureStrength 	= state.GetField < float >( 2, "surfaceTensionPressureStrength", 0.2f );
		def.surfaceTensionNormalStrength 	= state.GetField < float >( 2, "surfaceTensionNormalStrength", 0.2f );
		def.repulsiveStrength 				= state.GetField < float >( 2, "repulsiveStrength", 1.0f );
		def.powderStrength 					= state.GetField < float >( 2, "powderStrength", 0.5f );
		def.ejectionStrength 				= state.GetField < float >( 2, "ejectionStrength", 0.5f );
		def.staticPressureStrength 			= state.GetField < float >( 2, "staticPressureStrength", 0.2f );
		def.staticPressureRelaxation 		= state.GetField < float >( 2, "staticPressureRelaxation", 0.2f );
		def.staticPressureIterations 		= state.GetField < float >( 2, "staticPressureIterations", 8 );
		def.colorMixingStrength 			= state.GetField < float >( 2, "colorMixingStrength", 0.5f );
		def.lifetimeGranularity 			= state.GetField < float >( 2, "lifetimeGranularity", 1.0f / 60.0f );
		def.destroyByAge 					= state.GetField < bool >( 2, "destroyByAge", true );
	}

	MOAIBox2DParticleSystem* ps = new MOAIBox2DParticleSystem ();
	ps->SetParticleSystem ( self->mWorld->CreateParticleSystem ( &def ) );
	ps->SetWorld ( self );
	self->LuaRetain ( ps );

	ps->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addPrismaticJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorA	in units, in world coordinates, converted to meters
	@in		number anchorB	in units, in world coordinates, converted to meters
	@in		number axisA	translation axis vector X component (no units)
	@in		number axisB	translation axis vector Y component (no units)
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addPrismaticJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 axis;
	axis.x		= state.GetValue < float >( 6, 0 );
	axis.y		= state.GetValue < float >( 7, 0 );
	
	b2PrismaticJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor, axis );
	
	MOAIBox2DPrismaticJoint* joint = new MOAIBox2DPrismaticJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addPulleyJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number groundAnchorA_X	in units, in world coordinates, converted to meters
	@in		number groundAnchorA_Y	in units, in world coordinates, converted to meters
	@in		number groundAnchorB_X	in units, in world coordinates, converted to meters
	@in		number groundAnchorB_Y	in units, in world coordinates, converted to meters
	@in		number anchorA_X	in units, in world coordinates, converted to meters
	@in		number anchorA_Y	in units, in world coordinates, converted to meters
	@in		number anchorB_X	in units, in world coordinates, converted to meters
	@in		number anchorB_Y	in units, in world coordinates, converted to meters
	@in		number ratio
	@in		number maxLengthA	in units, converted to meters
	@in		number maxLengthB	in units, converted to meters
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addPulleyJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNNNNNNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 groundAnchorA;
	groundAnchorA.x		= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	groundAnchorA.y		= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 groundAnchorB;
	groundAnchorB.x		= state.GetValue < float >( 6, 0 ) * self->mUnitsToMeters;
	groundAnchorB.y		= state.GetValue < float >( 7, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 anchorA;
	anchorA.x			= state.GetValue < float >( 8, 0 ) * self->mUnitsToMeters;
	anchorA.y			= state.GetValue < float >( 9, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 anchorB;
	anchorB.x			= state.GetValue < float >( 10, 0 ) * self->mUnitsToMeters;
	anchorB.y			= state.GetValue < float >( 11, 0 ) * self->mUnitsToMeters;
	
	float ratio			= state.GetValue < float >( 12, 0 );
	
	b2PulleyJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, groundAnchorA, groundAnchorB, anchorA, anchorB, ratio );
	
	jointDef.lengthA	= state.GetValue < float >( 13, 0 ) * self->mUnitsToMeters;
	jointDef.lengthB	= state.GetValue < float >( 14, 0 ) * self->mUnitsToMeters;
	
	MOAIBox2DPulleyJoint* joint = new MOAIBox2DPulleyJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addRevoluteJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorX	in units, in world coordinates, converted to meters
	@in		number anchorY	in units, in world coordinates, converted to meters
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addRevoluteJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2RevoluteJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor );
	
	MOAIBox2DRevoluteJoint* joint = new MOAIBox2DRevoluteJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addRopeJoint
 @text	Create and add a rope joint to the world. See Box2D documentation.
 
 @in		MOAIBox2DWorld self
 @in		MOAIBox2DBody bodyA
 @in		MOAIBox2DBody bodyB 
 @in		number maxLength	in units, converted to meters
 @opt		number anchorAX		in units, in world coordinates, converted to meters
 @opt		number anchorAY		in units, in world coordinates, converted to meters
 @opt		number anchorBX		in units, in world coordinates, converted to meters
 @opt		number anchorBY		in units, in world coordinates, converted to meters
 @opt		boolean collideConnected		Default value is false		
 @out	MOAIBox2DJoint joint
 */
int	MOAIBox2DWorld::_addRopeJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUU" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	float maxLength = state.GetValue < float >( 4, 1 ) * self->mUnitsToMeters;
	
	b2RopeJointDef jointDef;
	jointDef.localAnchorA.x	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	jointDef.localAnchorA.y	= state.GetValue < float >( 6, 0 ) * self->mUnitsToMeters;

	jointDef.localAnchorB.x	= state.GetValue < float >( 7, 0 ) * self->mUnitsToMeters;
	jointDef.localAnchorB.y	= state.GetValue < float >( 8, 0 ) * self->mUnitsToMeters;

	jointDef.collideConnected = state.GetValue < bool >( 9, false );
	
	jointDef.bodyA = bodyA->mBody;
	jointDef.bodyB = bodyB->mBody;
	jointDef.maxLength = maxLength;
	
	MOAIBox2DRopeJoint* joint = new MOAIBox2DRopeJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}


//----------------------------------------------------------------//
/**	@name	addWeldJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorX	in units, in world coordinates, converted to meters
	@in		number anchorY	in units, in world coordinates, converted to meters
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addWeldJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2WeldJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor );
	
	MOAIBox2DWeldJoint* joint = new MOAIBox2DWeldJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name addWheelJoint
	@text	Create and add a joint to the world. See Box2D documentation.
 
 @in		MOAIBox2DWorld self
 @in		MOAIBox2DBody bodyA
 @in		MOAIBox2DBody bodyB
 @in		number anchorX	in units, in world coordinates, converted to meters
 @in		number anchorY	in units, in world coordinates, converted to meters
 @in		number axisX	translation axis vector X component (no units)
 @in		number axisY	translation axis vector Y component (no units)
 @out	MOAIBox2DJoint joint
 */
int	MOAIBox2DWorld::_addWheelJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 axis;
	axis.x		= state.GetValue < float >( 6, 0 );
	axis.y      = state.GetValue < float >( 7, 0 );
	
	b2WheelJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor, axis );
	
	MOAIBox2DWheelJoint* joint = new MOAIBox2DWheelJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	joint->LuaRetain ( bodyA );
	joint->LuaRetain ( bodyB );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name   calculateParticleIterations
 
	@text	Get the estimate particle iterations for particle numerical stability
			based on particle radius and gravity
 
	@in		MOAIBox2DWorld self
	@in     number  gravity     in units, converted to m/s^2. Default is 10 m/s^2
	@in     number  radius      in units, converted to m. Default is 1 m
	@in     number  timestep    Default is 1/60
	@out    number  iterations	estimate particleIterations
*/
int MOAIBox2DWorld::_calculateParticleIterations ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	float gravity = state.GetValue < float >( 2, 10.0f / self->mUnitsToMeters ) * self->mUnitsToMeters;
	float radius = state.GetValue < float >( 3, 1.0 / self->mUnitsToMeters ) * self->mUnitsToMeters;
	float timestep = state.GetValue < float >( 4, 1.0 / 60.f );
	
	float iter = b2CalculateParticleIterations ( gravity, radius, timestep );

	state.Push ( iter );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAngularSleepTolerance
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	number angularSleepTolerance	in degrees/s, converted from radians/s
*/
int MOAIBox2DWorld::_getAngularSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	float tolerance = self->mWorld->GetAngularSleepTolerance();
	tolerance *= ( float )R2D;
	lua_pushnumber ( state, tolerance);
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAutoClearForces
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	boolean autoClearForces
*/
int MOAIBox2DWorld::_getAutoClearForces ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	bool autoClearForces = self->mWorld->GetAutoClearForces ();
	lua_pushboolean ( L, autoClearForces );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getGravity
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	number gravityX		in units/s^2, converted from m/s^2
	@out	number gravityY		in units/s^2, converted from m/s^2
*/
int MOAIBox2DWorld::_getGravity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	b2Vec2 gravity = self->mWorld->GetGravity ();
	
	lua_pushnumber ( L, gravity.x / self->mUnitsToMeters );
	lua_pushnumber ( L, gravity.y / self->mUnitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getLinearSleepTolerance
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	number linearSleepTolerance	in units/s, converted from m/s
*/
int MOAIBox2DWorld::_getLinearSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	lua_pushnumber ( state, self->mWorld->GetLinearSleepTolerance () / self->mUnitsToMeters );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getTimeToSleep
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	number timeToSleep
*/
int MOAIBox2DWorld::_getTimeToSleep ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	lua_pushnumber ( state, self->mWorld->GetTimeToSleep ());
	return 0;
}

//----------------------------------------------------------------//
/** @name   queryAABB
	@text   Perform AABB query. Gather all fixtures in given AABB
 
	@in     MOAIBox2DWorld self
	@in     number  xMin, in units, converted to meters
	@in     number  yMin, in units, converted to meters
	@in     number  xMax, in units, converted to meters
	@in     number  yMax, in units, converted to meters
	@out    ...     fixtures
 */
int MOAIBox2DWorld::_queryAABB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UNNNN" )
	
	ZLRect rect = state.GetRect < float >( 2 );
	rect.Bless ();
	
	b2AABB aabb;
	aabb.lowerBound = self->mUnitsToMeters * b2Vec2 ( rect.mXMin, rect.mYMin );
	aabb.upperBound = self->mUnitsToMeters * b2Vec2 ( rect.mXMax, rect.mYMax );
	
	MOAIBox2DQueryCallback callback ( state );
	self->mWorld->QueryAABB ( &callback, aabb );
	
	return callback.GetCount ();
}

//----------------------------------------------------------------//
/** @name   rayCast
	@text   Perform raycast

	@in     MOAIBox2DWorld self
	@in     number  p1x, in units, converted to meters
	@in     number  p1y, in units, converted to meters
	@in     number  p2x, in units, converted to meters
	@in     number  p2y, in units, converted to meters
	@in     number  type    one of RAYCAST_NEAREST, RAYCAST_ALL, RAYCAST_ANY. Default is RAYCAST_NEAREST
	@in     number  flags   what results to return. Any combination of: RAYCAST_FIXTURE, RAYCAST_POINT, RAYCAST_NORMAL
							default is ( RAYCAST_FIXTURE + RAYCAST_POINT )
 
	@out    ...     All results in the folowing format: fixture1, x1, y1, xn1, yn1, fixture2, x2, y2, xn2, yn2 ...
					Only values specified in raycast flags are present. Position values are in units. Results are not sorted
 */
int MOAIBox2DWorld::_rayCast ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	b2Vec2 start;
	b2Vec2 end;
	start.x = state.GetValue < float >( 2, 0.0f ) * self->mUnitsToMeters;
	start.y = state.GetValue < float >( 3, 0.0f ) * self->mUnitsToMeters;
	end.x   = state.GetValue < float >( 4, 0.0f ) * self->mUnitsToMeters;
	end.y   = state.GetValue < float >( 5, 0.0f ) * self->mUnitsToMeters;
	u32 type = state.GetValue < u32 > ( 6, MOAIBox2DRayCastCallback::RAYCAST_NEAREST );
	u32 flags = state.GetValue < u32 > ( 7, MOAIBox2DRayCastCallback::RAYCAST_FIXTURE | MOAIBox2DRayCastCallback::RAYCAST_POINT );
	
	MOAIBox2DRayCastCallback callback ( state, self->mUnitsToMeters );
	callback.SetType ( type );
	callback.SetFlags ( flags );
	
	self->mWorld->RayCast ( &callback, start, end );
	
	if ( type == MOAIBox2DRayCastCallback::RAYCAST_NEAREST ) {
		callback.PushResult ();
	}
	
	return callback.GetCount();
}


//----------------------------------------------------------------//
/**	@name	setAngularSleepTolerance
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number angularSleepTolerance		in degrees/s, converted to radians/s. Default value is 0.0f.
	@out	nil
*/
int MOAIBox2DWorld::_setAngularSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	float tolerance = state.GetValue< float >( 2, 0.0f );
	tolerance *= ( float )R2D;
	self->mWorld->SetAngularSleepTolerance ( tolerance );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAutoClearForces
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	boolean autoClearForces		Default value is 'true'
	@out	nil
*/
int MOAIBox2DWorld::_setAutoClearForces ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	bool autoClearForces = state.GetValue < bool >( 2, true );
	
	self->mWorld->SetAutoClearForces ( autoClearForces );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDebugDrawEnabled
	@text	enable/disable debug drawing.
 
	@in		MOAIBox2DWorld self
	@in		boolean enable
	@out	nil
*/
int MOAIBox2DWorld::_setDebugDrawEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
 
	bool enabled = state.GetValue < bool >( 2, false );
 
	if(enabled)
	{
		self->mDebugDraw->SetFlags ( DEBUG_DRAW_DEFAULT );
	}
	else
	{
		self->mDebugDraw->ClearFlags(0);
		self->mDebugDraw->SetFlags(0);
	}
 
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDebugDrawFlags
	@text	Sets mask for debug drawing.
	
	@in		MOAIBox2DWorld self
	@opt	number flags		One of MOAIBox2DWorld.DEBUG_DRAW_SHAPES, MOAIBox2DWorld.DEBUG_DRAW_JOINTS,
								MOAIBox2DWorld.DEBUG_DRAW_BOUNDS, MOAIBox2DWorld.DEBUG_DRAW_PAIRS,
								MOAIBox2DWorld.DEBUG_DRAW_CENTERS. Default value is MOAIBox2DWorld.DEBUG_DRAW_DEFAULT.
	@out	nil
*/
int MOAIBox2DWorld::_setDebugDrawFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	u32 flags = state.GetValue < u32 >( 2, DEBUG_DRAW_DEFAULT );
	if ( self->mDebugDraw ) {
		self->mDebugDraw->SetFlags ( flags );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGravity
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number gravityX			in units/s^2, converted to m/s^2. Default value is 0.
	@opt	number gravityY			in units/s^2, converted to m/s^2. Default value is 0.
	@out	nil
*/
int MOAIBox2DWorld::_setGravity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	b2Vec2 gravity;
	
	gravity.x	= state.GetValue < float >( 2, 0.0f ) * self->mUnitsToMeters;
	gravity.y	= state.GetValue < float >( 3, 0.0f ) * self->mUnitsToMeters;
	
	self->mWorld->SetGravity ( gravity );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setIterations
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number velocityIteratons			Default value is current value of velocity iterations.
	@opt	number positionIterations			Default value is current value of positions iterations.
	@opt	number particleIterations			Default value is current value of particle iterations.
	@out	nil
*/
int MOAIBox2DWorld::_setIterations ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	self->mVelocityIterations = state.GetValue < u32 >( 2, self->mVelocityIterations );
	self->mPositionIterations = state.GetValue < u32 >( 3, self->mPositionIterations );
	self->mParticleIterations = state.GetValue < u32 >( 4, self->mParticleIterations );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLinearSleepTolerance
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number linearSleepTolerance		in units/s, converted to m/s. Default value is 0.0f.
	@out	nil
*/
int MOAIBox2DWorld::_setLinearSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	self->mWorld->SetLinearSleepTolerance ( state.GetValue < float >( 2, 0.0f ) * self->mUnitsToMeters );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTimeToSleep
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number timeToSleep				Default value is 0.0f.
	@out	nil
*/
int MOAIBox2DWorld::_setTimeToSleep ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	self->mWorld->SetTimeToSleep ( state.GetValue < float >( 2, 0.0f ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUnitsToMeters
	@text	Sets a scale factor for converting game world units to Box2D meters.
	
	@in		MOAIBox2DWorld self
	@opt	number unitsToMeters			Default value is 1.
	@out	nil
*/
int MOAIBox2DWorld::_setUnitsToMeters ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	self->mUnitsToMeters = state.GetValue ( 2, 1.0f );
	
	return 0;
}

//================================================================//
// MOAIBox2DWorld
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DWorld::Destroy () {

	if ( this->mLock ) return;
	this->mLock = true;

	while ( this->mDestroyFixtures ) {
		MOAIBox2DPrim* prim = this->mDestroyFixtures;
		this->mDestroyFixtures = this->mDestroyFixtures->mDestroyNext;
		prim->Destroy ();
		
		prim->SetWorld ( 0 );
		this->LuaRelease ( prim );
	}
	
	while ( this->mDestroyJoints ) {
		MOAIBox2DPrim* prim = this->mDestroyJoints;
		this->mDestroyJoints = this->mDestroyJoints->mDestroyNext;
		prim->Destroy ();
		
		prim->SetWorld ( 0 );
		this->LuaRelease ( prim );
	}
	
	while ( this->mDestroyBodies ) {
		MOAIBox2DPrim* prim = this->mDestroyBodies;
		this->mDestroyBodies = this->mDestroyBodies->mDestroyNext;
		prim->Destroy ();
		
		prim->SetWorld ( 0 );
		this->LuaRelease ( prim );
	}

	while ( this->mDestroyParticleSystems ) {
		MOAIBox2DPrim* prim = this->mDestroyParticleSystems;
		this->mDestroyParticleSystems = this->mDestroyParticleSystems->mDestroyNext;
		prim->Destroy ();
		
		prim->SetWorld ( 0 );
		this->LuaRelease ( prim );
	}
	
	this->mLock = false;
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::DrawDebug () {

	if ( this->mDebugDraw ) {
		
		MOAIDraw::Bind ();
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		
		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_WORLD, MOAIGfxDevice::VTX_STAGE_PROJ );
		gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
		
		this->mDebugDraw->mScale = 1.0f / this->mUnitsToMeters;
		this->mWorld->DrawDebugData ();
	}
}

//----------------------------------------------------------------//
bool MOAIBox2DWorld::IsDone () {

	return false;
}

//----------------------------------------------------------------//
bool MOAIBox2DWorld::IsLocked () {

	if ( this->mWorld ) {
		return this->mWorld->IsLocked ();
	}
	return false;
}

//----------------------------------------------------------------//
MOAIBox2DWorld::MOAIBox2DWorld () :
	mLock ( false ),
	mVelocityIterations ( 10 ),
	mPositionIterations ( 10 ),
	mParticleIterations ( 1 ),
	mUnitsToMeters ( 1.0f ),
	mDestroyBodies ( 0 ),
	mDestroyFixtures ( 0 ),
	mDestroyJoints ( 0 ),
	mDestroyParticleSystems ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mArbiter.Set ( *this, new MOAIBox2DArbiter ( *this ));
	
	b2Vec2 gravity ( 0.0f, 0.0f );
	this->mWorld = new b2World ( gravity);
	this->mWorld->SetContactListener ( this->mArbiter );
	this->mWorld->SetDestructionListener ( this );
	this->mWorld->SetAllowSleeping ( true );
	this->mDebugDraw = new MOAIBox2DDebugDraw ();
	this->mWorld->SetDebugDraw ( this->mDebugDraw );
	
	this->mDebugDraw->SetFlags ( DEBUG_DRAW_DEFAULT );
}

//----------------------------------------------------------------//
MOAIBox2DWorld::~MOAIBox2DWorld () {

	this->mWorld->SetContactListener ( 0 );

	while ( b2Body* body = this->mWorld->GetBodyList ()) {
		MOAIBox2DBody* moaiBody = ( MOAIBox2DBody* )body->GetUserData ();
		
		this->mWorld->DestroyBody ( body );
		moaiBody->mBody = 0;
		moaiBody->SetWorld ( 0 );
		this->LuaRelease ( moaiBody );
	}
	
	this->mArbiter.Set ( *this, 0 );
	
	delete ( this->mDebugDraw );
	delete ( this->mWorld );
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::OnUpdate ( float step ) {
	
	this->mLock = true;
	this->mWorld->Step ( step, this->mVelocityIterations, this->mPositionIterations, this->mParticleIterations );
	this->mLock = false;
	
	this->Destroy ();
	
	b2Body* body = this->mWorld->GetBodyList ();
	for ( ; body; body = body->GetNext ()) {
		if ( body->IsActive () && body->IsAwake ()) {
			MOAIBox2DBody* moaiBody = ( MOAIBox2DBody* )body->GetUserData ();
			moaiBody->ScheduleUpdate ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
	
	state.SetField ( -1, "DEBUG_DRAW_SHAPES", ( u32 )DEBUG_DRAW_SHAPES );
	state.SetField ( -1, "DEBUG_DRAW_JOINTS", ( u32 )DEBUG_DRAW_JOINTS );
	state.SetField ( -1, "DEBUG_DRAW_BOUNDS", ( u32 )DEBUG_DRAW_BOUNDS );
	state.SetField ( -1, "DEBUG_DRAW_PAIRS", ( u32 )DEBUG_DRAW_PAIRS );
	state.SetField ( -1, "DEBUG_DRAW_CENTERS", ( u32 )DEBUG_DRAW_CENTERS );
	state.SetField ( -1, "DEBUG_DRAW_PARTICLES", ( u32 )DEBUG_DRAW_PARTICLES );
	
	state.SetField ( -1, "DEBUG_DRAW_DEFAULT", ( u32 )DEBUG_DRAW_DEFAULT );
	
	state.SetField ( -1, "RAYCAST_NEAREST", ( u32 )MOAIBox2DRayCastCallback::RAYCAST_NEAREST );
	state.SetField ( -1, "RAYCAST_ALL",     ( u32 )MOAIBox2DRayCastCallback::RAYCAST_ALL );
	state.SetField ( -1, "RAYCAST_ANY",     ( u32 )MOAIBox2DRayCastCallback::RAYCAST_ANY );
	state.SetField ( -1, "RAYCAST_FIXTURE", ( u32 )MOAIBox2DRayCastCallback::RAYCAST_FIXTURE );
	state.SetField ( -1, "RAYCAST_POINT",   ( u32 )MOAIBox2DRayCastCallback::RAYCAST_POINT );
	state.SetField ( -1, "RAYCAST_NORMAL",  ( u32 )MOAIBox2DRayCastCallback::RAYCAST_NORMAL );
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "addBody",					_addBody },
		{ "addDistanceJoint",			_addDistanceJoint },
		{ "addFrictionJoint",			_addFrictionJoint },
		{ "addGearJoint",				_addGearJoint },
		{ "addMotorJoint", 				_addMotorJoint },
		{ "addMouseJoint",				_addMouseJoint },
		{ "addParticleSystem",			_addParticleSystem },
		{ "addPrismaticJoint",			_addPrismaticJoint },
		{ "addPulleyJoint",				_addPulleyJoint },
		{ "addRevoluteJoint",			_addRevoluteJoint },
		{ "addRopeJoint",				_addRopeJoint },
		{ "addWeldJoint",				_addWeldJoint },
		{ "addWheelJoint",				_addWheelJoint },
		{ "calculateParticleIterations",_calculateParticleIterations},
		{ "getAngularSleepTolerance",	_getAngularSleepTolerance },
		{ "getAutoClearForces",			_getAutoClearForces },
		{ "getGravity",					_getGravity },
		{ "getLinearSleepTolerance",	_getLinearSleepTolerance },
		{ "getTimeToSleep",				_getTimeToSleep },
		{ "queryAABB",                  _queryAABB },
		{ "rayCast",                    _rayCast },
		{ "setAngularSleepTolerance",	_setAngularSleepTolerance },
		{ "setAutoClearForces",			_setAutoClearForces },
		{ "setDebugDrawEnabled",		_setDebugDrawEnabled },
		{ "setDebugDrawFlags",			_setDebugDrawFlags },
		{ "setGravity",					_setGravity },
		{ "setIterations",				_setIterations },
		{ "setLinearSleepTolerance",	_setLinearSleepTolerance },
		{ "setTimeToSleep",				_setTimeToSleep },
		{ "setUnitsToMeters",			_setUnitsToMeters },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::SayGoodbye ( b2Fixture* fixture ) {

	MOAIBox2DFixture* moaiFixture = ( MOAIBox2DFixture* )fixture->GetUserData ();
	if ( moaiFixture->mFixture ) {
		moaiFixture->mFixture = 0;
		moaiFixture->SetWorld ( 0 );
		this->LuaRelease ( moaiFixture );
	}
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::SayGoodbye ( b2Joint* joint ) {

	MOAIBox2DJoint* moaiJoint = ( MOAIBox2DJoint* )joint->GetUserData ();
	if ( moaiJoint->mJoint ) {
		moaiJoint->mJoint = 0;
		moaiJoint->SetWorld ( 0 );
		this->LuaRelease ( moaiJoint );
	}
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::SayGoodbye ( b2ParticleGroup* group ) {
	
	MOAIBox2DParticleGroup* moaiGroup = ( MOAIBox2DParticleGroup* )group->GetUserData ();
	if ( moaiGroup->mParticleGroup ) {
		moaiGroup->mParticleGroup = 0;
		moaiGroup->SetWorld ( 0 );
		this->LuaRelease ( moaiGroup );
	}
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::ScheduleDestruction ( MOAIBox2DBody& body ) {

	if ( !body.mDestroy ) {
		body.mDestroyNext = this->mDestroyBodies;
		this->mDestroyBodies = &body;
		body.mDestroy = true;
	}
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::ScheduleDestruction ( MOAIBox2DFixture& fixture ) {

	if ( !fixture.mDestroy ) {
		fixture.mDestroyNext = this->mDestroyFixtures;
		this->mDestroyFixtures = &fixture;
		fixture.mDestroy = true;
	}
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::ScheduleDestruction ( MOAIBox2DJoint& joint ) {

	if ( !joint.mDestroy ) {
		joint.mDestroyNext = this->mDestroyJoints;
		this->mDestroyJoints = &joint;
		joint.mDestroy = true;
	}
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::ScheduleDestruction ( MOAIBox2DParticleSystem& particleSystem ) {

	if ( !particleSystem.mDestroy ) {
		particleSystem.mDestroyNext = this->mDestroyParticleSystems;
		this->mDestroyParticleSystems = &particleSystem;
		particleSystem.mDestroy = true;
	}
	this->Destroy ();
}



//================================================================//
// MOAIBox2DRayCastCallback
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DRayCastCallback::MOAIBox2DRayCastCallback ( MOAILuaState& state, float unitsToMeters ):
	mType ( RAYCAST_NEAREST ),
	mFlags ( RAYCAST_FIXTURE | RAYCAST_PARTICLE ),
	mCount ( 0 ),
	mParticleIdx ( b2_invalidParticleIndex ),
	mUnitsToMeters ( unitsToMeters ),
	mPoint ( 0, 0 ),
	mNormal ( 0, 0 ),
	mFixture ( 0 ),
	mState ( state ) {
}

//----------------------------------------------------------------//
MOAIBox2DRayCastCallback::~MOAIBox2DRayCastCallback() {
	
}

//----------------------------------------------------------------//
void MOAIBox2DRayCastCallback::PushResult () {

	if ( this->mFixture ) {
		this->PushResult ( this->mFixture, this->mPoint, this->mNormal );
	}
	
	if ( this->mParticleIdx != b2_invalidParticleIndex ) {
		this->PushResult ( this->mParticleIdx, this->mPoint, this->mNormal );
	}
}

//----------------------------------------------------------------//
void MOAIBox2DRayCastCallback::PushResult ( b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal ) {
	
	lua_checkstack ( this->mState, 10 );
	
	MOAIBox2DFixture* moaiFixture = ( MOAIBox2DFixture* ) fixture->GetUserData ();
	if ( moaiFixture && ( this->mFlags & RAYCAST_FIXTURE ) ) {
		moaiFixture->PushLuaUserdata ( this->mState );
		this->mCount++;
	}
	
	if ( this->mFlags & RAYCAST_POINT ) {
		this->mState.Push ( point.x / this->mUnitsToMeters );
		this->mState.Push ( point.y / this->mUnitsToMeters );
		this->mCount += 2;
	}
	
	if ( this->mFlags & RAYCAST_NORMAL ) {
		this->mState.Push ( normal.x );
		this->mState.Push ( normal.y );
		this->mCount += 2;
	}
}


//----------------------------------------------------------------//
void MOAIBox2DRayCastCallback::PushResult ( int particleIdx, const b2Vec2 &point, const b2Vec2 &normal ) {
	
	lua_checkstack ( this->mState, 10 );
	
	if ( this->mFlags & RAYCAST_PARTICLE ) {
		this->mState.Push ( particleIdx );
		this->mCount++;
	}
	
	if ( this->mFlags & RAYCAST_POINT ) {
		this->mState.Push ( point.x / this->mUnitsToMeters );
		this->mState.Push ( point.y / this->mUnitsToMeters );
		this->mCount += 2;
	}
	
	if ( this->mFlags & RAYCAST_NORMAL ) {
		this->mState.Push ( normal.x );
		this->mState.Push ( normal.y );
		this->mCount += 2;
	}
}


//----------------------------------------------------------------//
float32 MOAIBox2DRayCastCallback::ReportFixture ( b2Fixture *fixture, const b2Vec2 &point,
												 const b2Vec2 &normal, float32 fraction ) {
	
	switch ( this->mType ) {
			
		case RAYCAST_NEAREST:
			this->mFixture = fixture;
			this->mPoint = point;
			this->mNormal = normal;
			return fraction;
		
		case RAYCAST_ALL:
			this->PushResult ( fixture, point, normal );
			return 1.0f;

		case RAYCAST_ANY:
			this->PushResult ( fixture, point, normal );
			return 0.0f;
	}
	
	return 0;
}


//----------------------------------------------------------------//
float32 MOAIBox2DRayCastCallback::ReportParticle ( const b2ParticleSystem *particleSystem, int32 index,
												  const b2Vec2 &point, const b2Vec2 &normal, float32 fraction ) {
	
	switch ( this->mType ) {
		case RAYCAST_NEAREST:
			this->mParticleIdx = index;
			this->mPoint = point;
			this->mNormal = normal;
			return fraction;
			
		case RAYCAST_ALL:
			this->PushResult ( index, point, normal );
			return 1.0f;
			
		case RAYCAST_ANY:
			this->PushResult ( index, point, normal );
			return 0.0f;
	}
	
	return 0;
}


//----------------------------------------------------------------//
bool MOAIBox2DRayCastCallback::ShouldQueryParticleSystem ( const b2ParticleSystem *particleSystem ) {
	
	// Always return false so that particles do not participate in b2World raycasts.
	// Particle raycasts can be performed with MOAIBox2DParticleSystem
	// which ignores value returned by ShouldQueryParticleSystem ()
	return false;
}



//================================================================//
// MOAIBox2DQueryCallback
//================================================================//
MOAIBox2DQueryCallback::MOAIBox2DQueryCallback ( MOAILuaState& state ):
	mCount ( 0 ) {
	
	mState = state;
}


//----------------------------------------------------------------//
MOAIBox2DQueryCallback::~MOAIBox2DQueryCallback () {
	
}

//----------------------------------------------------------------//
bool MOAIBox2DQueryCallback::ReportFixture ( b2Fixture *fixture ) {
	
	MOAIBox2DFixture* moaiFixture = ( MOAIBox2DFixture* ) fixture->GetUserData();
	if ( moaiFixture ) {
		lua_checkstack ( this->mState, 5 );
		moaiFixture->PushLuaUserdata ( this->mState );
		this->mCount++;
	}
	
	return true;
}

//----------------------------------------------------------------//
bool MOAIBox2DQueryCallback::ReportParticle ( const b2ParticleSystem *particleSystem, int32 index ) {
	
	lua_checkstack ( this->mState, 5 );
	
	this->mState.Push ( index );
	this->mCount++;
	
	return true;
}

//----------------------------------------------------------------//
bool MOAIBox2DQueryCallback::ShouldQueryParticleSystem ( const b2ParticleSystem *particleSystem ) {
	
	return false;
}



