// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/JniUtils.h>
#include <moai-android-crashlytics/MOAICrashlyticsAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAICrashlyticsAndroid::_init ( lua_State* L ) {
	
	MOAICrashlyticsAndroid::Get ().Init ();
	return 0;
}

//----------------------------------------------------------------//
int MOAICrashlyticsAndroid::_log ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* msg = state.GetValue < cc8* >( 1, 0 );
	if ( msg ) {
		MOAICrashlyticsAndroid::Get ().Log ( msg );	
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAICrashlyticsAndroid::_set ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* key = state.GetValue < cc8* >( 1, 0 );
	cc8* val = state.GetValue < cc8* >( 2, 0 );
	if ( key && val ) {
		MOAICrashlyticsAndroid::Get ().Set ( key, val );
	}

	MOAICrashlyticsAndroid::Get ().Init ();
	return 0;
}


//================================================================//
// MOAICrashlyticsAndroid
//================================================================//

//----------------------------------------------------------------//
void MOAICrashlyticsAndroid::Init () {

	if ( !this->mCtx ) {
		this->mCtx = crashlytics_init ();
		if ( this->mCtx == 0 ) { 
			MOAIPrint ( "\n\n\n\n\n" );
			MOAIPrint ( "=====================================================\n" );
			MOAIPrint ( "Crashlytics initialization error\n" );
			MOAIPrint ( "\n\n\n\n\n" );
		}
	}
}

//----------------------------------------------------------------//
void MOAICrashlyticsAndroid::Log ( cc8* message ) {

	if ( this->mCtx ) {
		this->mCtx->log ( this->mCtx, message );
	}
}

//----------------------------------------------------------------//
MOAICrashlyticsAndroid::MOAICrashlyticsAndroid ():
	mCtx ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
	
}

//----------------------------------------------------------------//
MOAICrashlyticsAndroid::~MOAICrashlyticsAndroid () {

	if ( this->mCtx ) {
		crashlytics_free ( &mCtx );
		this->mCtx = 0;
	}
}

//----------------------------------------------------------------//
void MOAICrashlyticsAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "init",	_init },
		{ "log",	_log },
		{ "set",	_set },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICrashlyticsAndroid::Set ( cc8* key, cc8* value ) {

	if ( this->mCtx ) {
		this->mCtx->set ( this->mCtx, key, value );
	}
}
