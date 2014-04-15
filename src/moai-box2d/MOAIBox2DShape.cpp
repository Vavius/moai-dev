// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DWeldJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/** @name 	inirChain
	@text 	Initialize shape as chain
	
	@in		MOAIBox2DShape self
	@in		table verts					Array containing vertex coordinate components ( t[1] = x0, t[2] = y0, t[3] = x1, t[4] = y1... )
	@opt	boolean closeChain			Default value is false.
	@out	nil
*/
int MOAIBox2DShape::_initChain ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )

	float unitsToMeters = self->GetUnitsToMeters ();
	
	u32 totalCoords = lua_objlen ( state, 2 );
	if ( totalCoords < 4 || totalCoords % 2 != 0 ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_InvalidVertexCount_D, totalCoords );
		return 0;
	}
 
	u32 totalVerts = totalCoords / 2;
	b2Vec2 * verts = (b2Vec2 *)alloca(sizeof(b2Vec2) * totalVerts);
	int numVerts = MOAIBox2DFixture::LoadVerts( state, 2, verts, totalVerts, unitsToMeters );
	
	bool closeChain = state.GetValue < bool >( 3, false );
	b2ChainShape* chainShape = new b2ChainShape ();
	if( closeChain ) {
		chainShape->CreateLoop(verts, numVerts);
	}
	else {
		chainShape->CreateChain(verts, numVerts);
	}
	self->SetShape ( chainShape );

	return 0;

}

//----------------------------------------------------------------//
/**	@name	initCircle
	@text 	Initialize shape as circle

	@in		MOAIBox2DShape 	self
	@in		number x		in units, world coordinates, converted to meters
	@in		number y		in units, world coordinates, converted to meters
	@in		number radius	in units, converted to meters
	@out	nil
*/
int MOAIBox2DShape::_initCircle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UNNN" )

	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2CircleShape* circleShape = new b2CircleShape ();

	circleShape->m_p.x		= state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	circleShape->m_p.y		= state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	circleShape->m_radius	= state.GetValue < float >( 4, 1.0f ) * unitsToMeters;
	
	self->SetShape ( circleShape );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	initPolygon
	@text	Initialize shape as polygon
	
	@in		MOAIBox2DBody self
	@in		table verts					Array containing vertex coordinate components in units, world coordinates, converted to meters. ( t[1] = x0, t[2] = y0, t[3] = x1, t[4] = y1... )
	@out	nil
*/
int MOAIBox2DShape::_initPolygon ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "U" )

	float unitsToMeters = self->GetUnitsToMeters ();

	b2Vec2 verts [ b2_maxPolygonVertices ];
	
	int numVerts = MOAIBox2DFixture::LoadVerts ( state, 2, verts, b2_maxPolygonVertices, unitsToMeters );
	if (( numVerts < 3 ) || ( numVerts > b2_maxPolygonVertices )) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DBody_InvalidVertexCount_D, numVerts );
		return 0;
	}
	
	b2PolygonShape* polyShape = new b2PolygonShape ();
	polyShape->Set ( verts, numVerts );
	
	self->SetShape ( polyShape );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	initRect
	@text	Initialize shape as a rect
	
	@in		MOAIBox2DShape 	self
	@in		number xMin		in units, world coordinates, converted to meters
	@in		number yMin		in units, world coordinates, converted to meters
	@in		number xMax		in units, world coordinates, converted to meters
	@in		number yMax		in units, world coordinates, converted to meters
	@in		number angle
	@out	nil
*/
int MOAIBox2DShape::_initRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DBody, "UNNNN" )
	
	float unitsToMeters = self->GetUnitsToMeters ();
	
	ZLRect rect = state.GetRect < float >( 2 );
	rect.Bless ();
	
	float angle = state.GetValue < float >( 6, 0.0f );
	
	float hx = rect.Width () * 0.5f * unitsToMeters;
	float hy = rect.Height () * 0.5f * unitsToMeters;
	
	b2Vec2 center;
	center.x = ( rect.mXMin * unitsToMeters ) + hx;
	center.y = ( rect.mYMin * unitsToMeters ) + hy;
	
	b2PolygonShape* polyShape = new b2PolygonShape ();
	polyShape->SetAsBox ( hx, hy, center, angle );
	
	self->SetShape ( polyShape );
	return 0;
}


//================================================================//
// MOAIBox2DShape
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DShape::Clear () {

	if ( this->mShape ) {
		delete this->mShape;
		this->mShape = 0;
	}
}

//----------------------------------------------------------------//
MOAIBox2DShape::MOAIBox2DShape () :
	mShape ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DShape::~MOAIBox2DShape () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIBox2DShape::RegisterLuaClass ( MOAILuaState& state ) {

}

//----------------------------------------------------------------//
void MOAIBox2DShape::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = { 
		{ "initChain",          _initChain },
		{ "initCircle", 		_initCircle },
		{ "initPolygon", 		_initPolygon },
		{ "initRect", 			_initRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DShape::SetShape ( b2Shape* shape ) {

	if ( this->mShape ) {
		self->Clear ();
	}
	self->mShape = shape;
}


