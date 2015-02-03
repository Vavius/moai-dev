// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android-flurry/MOAIFlurryAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIFlurryAndroid::_startSession ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	jstring japikey = self->GetJString ( state.GetValue < cc8* >( 1, "" ));
	jmethodID startSession = self->GetStaticMethod ( "startSession", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( startSession, japikey );

	return 0;
}


//----------------------------------------------------------------//
int MOAIFlurryAndroid::_setAppVersion ( lua_State * L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	jstring jversion = self->GetJString ( state.GetValue < cc8* >( 1, "" ));
	jmethodID setAppVersion = self->GetStaticMethod ( "setAppVersion", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( setAppVersion, jversion );

	return 0;
}



//----------------------------------------------------------------//
int MOAIFlurryAndroid::_logEvent ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	jstring eventName = self->GetJString ( state.GetValue < cc8* >( 1, "" ));
	jobject map = 0;

	if ( state.IsType ( 2, LUA_TTABLE )) {
		map = self->HashMapFromLua ( L, 2 );
	}

	if ( map == NULL ) {
		jmethodID logEvent = self->GetStaticMethod ( "logEvent", "(Ljava/lang/String;)V" );
		self->CallStaticVoidMethod ( logEvent, eventName );
	} else {
		jmethodID logEvent = self->GetStaticMethod ( "logEvent", "(Ljava/lang/String;Ljava/util/Map;)V" );
		self->CallStaticVoidMethod ( logEvent, eventName, map );
	}

	return 0;
}

//----------------------------------------------------------------//
int MOAIFlurryAndroid::_logTimedEvent ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	jstring eventName = self->GetJString ( state.GetValue < cc8* >( 1, "" ));
	jobject map = 0;

	if ( state.IsType ( 2, LUA_TTABLE )) {
		map = self->HashMapFromLua ( L, 2 );
	}

	if ( map == NULL ) {
		bool timed = state.GetValue < bool >( 2, false );
		jmethodID logEvent = self->GetStaticMethod ( "logEvent", "(Ljava/lang/String;Z)V" );
		self->CallStaticVoidMethod ( logEvent, eventName, timed );
	} else {
		bool timed = state.GetValue < bool >( 3, false );
		jmethodID logEvent = self->GetStaticMethod ( "logEvent", "(Ljava/lang/String;Ljava/util/Map;Z)V" );
		self->CallStaticVoidMethod ( logEvent, eventName, map, timed );
	}

	return 0;
}


//----------------------------------------------------------------//
int MOAIFlurryAndroid::_endTimedEvent ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	jstring eventId = self->GetJString ( state.GetValue < cc8* >( 1, "" ));
	jmethodID logEvent = self->GetStaticMethod ( "endTimedEvent", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( logEvent, eventId );
	
	return 0;
}


//================================================================//
// MOAIFlurryAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIFlurryAndroid::MOAIFlurryAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->SetClass ( "com/ziplinegames/moai/MoaiFlurry" );
}

//----------------------------------------------------------------//
MOAIFlurryAndroid::~MOAIFlurryAndroid () {
}

//----------------------------------------------------------------//
void MOAIFlurryAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "startSession",	_startSession },
		{ "setAppVersion",	_setAppVersion },
		{ "logEvent",		_logEvent },
		{ "logTimedEvent",	_logTimedEvent },
		{ "endTimedEvent",	_endTimedEvent },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}