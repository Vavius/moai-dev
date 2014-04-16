// Copyright (c) 2010-2014 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBOX2DSHAPE_H
#define MOAIBOX2DSHAPE_H


//================================================================//
// MOAIBox2DShape
//================================================================//
/** @name MOAIBox2DShape
	@text Box2D shape wrapper
*/
class MOAIBox2DShape :
	public virtual MOAILuaObject {
private:

	b2Shape*	mShape;

	//----------------------------------------------------------------//
	static int			_initChain				( lua_State* L );
	static int			_initCircle				( lua_State* L );
	static int			_initPolygon			( lua_State* L );
	static int			_initRect				( lua_State* L );

public:
	
	friend class MOAIBox2DParticleSystem;

	DECL_LUA_FACTORY ( MOAIBox2DShape )
	
	//----------------------------------------------------------------//
    void            Clear                   ();
					MOAIBox2DShape          ();
					~MOAIBox2DShape         ();
	void            RegisterLuaClass        ( MOAILuaState& state );
	void            RegisterLuaFuncs        ( MOAILuaState& state );
    void            SetShape                ( b2Shape* shape );
};

#endif
