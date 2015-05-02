// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_FLURRY

#include "pch.h"

#import <moai-ios-flurry/MOAIFlurryIOS.h>
#import <moai-apple/NSData+MOAILib.h>
#import <moai-apple/NSDate+MOAILib.h>
#import <moai-apple/NSDictionary+MOAILib.h>
#import <moai-apple/NSError+MOAILib.h>
#import <moai-apple/NSString+MOAILib.h>

#import "Flurry.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	setAppVersion
 @text	Set app version that will be reported by Flurry. Call before startSession.
 
 @in	string version		App version string.
 @out	nil
 */
int MOAIFlurryIOS::_setAppVersion ( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	cc8* version = state.GetValue < cc8* >( 1, "" );
	
	if ( version && version [ 0 ] != '\0' ) {
		NSString * str = [ NSString stringWithUTF8String:version ];
		[ Flurry setAppVersion:str ];
	}
	return 0;
}

/**	@name	setCrashTracking
    @text	Enable flurry's built-in crash tracking of unhandled exceptions.
 
    @in     bool tracking		Is built-in crash tracking enabled. Disabled by default.
    @out	nil
*/
int MOAIFlurryIOS::_setCrashTracking ( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	bool enable = state.GetValue < bool >( 1, true );
	
	[ Flurry setCrashReportingEnabled:enable ];
	
	return 0;
}

/**	@name	startSession
    @text	Start the Flurry session.
 
    @in     string apikey		The api key from Flurry.
    @out	nil
*/
int MOAIFlurryIOS::_startSession( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	cc8* api = state.GetValue < cc8* >( 1, "" );
	
	if ( api && api [ 0 ] != '\0' ) {
		NSString * str = [ NSString stringWithUTF8String:api ];
		[ Flurry startSession:str ];
		[ Flurry setSessionReportsOnCloseEnabled:YES ];
		[ Flurry setSessionReportsOnPauseEnabled:YES ];
	}
	return 0;
}

/**	@name	logEvent
 @text	Log an event to Flurry.
 
 @in	string eventName	The name of the event to send to Flurry
 @in	table params		(optional)The params to send to Flurry with the event or nil if not required.
 @out	nil
 */
int MOAIFlurryIOS::_logEvent( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	NSString* eventName = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	
	if ( eventName == NULL )
		return 0;

	if ( state.IsType ( 2, LUA_TTABLE )) {
		NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
		[ paramsDict initWithLua:state stackIndex:2 ];
		[ Flurry logEvent:eventName withParameters:paramsDict ];
		[ paramsDict release ];
	}
	else {
		[ Flurry logEvent:eventName ];
	}
	
	[ eventName release ];

	return 0;
}

/**	@name	logError
 @text	Log an error to Flurry.
 
 @in	string errorName	The name of the error to send to Flurry
 @in	string errorMsg		Error message
 @in	table userInfo		(optional) Custom UserInfo table for error.
 @out	nil
 */
int MOAIFlurryIOS::_logError( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	NSString* errorName = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	NSString* errorMsg = [[ NSString alloc ] initWithLua: state stackIndex: 2 ];
	
	if ( errorName == NULL )
		return 0;
	
	if ( errorMsg == NULL )
		return 0;
	
	NSError *error;
	if ( state.IsType ( 3, LUA_TTABLE )) {
		NSMutableDictionary* userInfo = [[ NSMutableDictionary alloc ] init ];
		[ userInfo initWithLua:state stackIndex:3 ];
		error = [ NSError errorWithDomain:@"moai.luaError" code:1 userInfo:userInfo ];
	}
	else {
		error = [ NSError errorWithDomain:@"moai.luaError" code:1 userInfo:nil ];
	}
	
	[ Flurry logError:errorName message:errorMsg error:error ];
	
	return 0;
}

/**	@name	logTimedEvent
    @text	Start a timed event.

    @in     string eventName	The name of the event to send to Flurry
    @in     table params		(optional)The params to send to Flurry with the event or nil if not required.
    @out	nil
 */
int MOAIFlurryIOS::_logTimedEvent( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	NSString* eventName = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	
	if ( eventName == NULL )
		return 0;
	
	if ( state.IsType ( 2, LUA_TTABLE )) {
		NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
		[ paramsDict initWithLua:state stackIndex:2 ];
		[ Flurry logEvent:eventName withParameters:paramsDict timed:YES ];
        [ paramsDict release ];
    }
	else {
		[ Flurry logEvent:eventName timed:YES ];
	}

	return 0;
}

/**	@name	endTimedEvent
    @text	End a timed event.
 
    @in     string eventName	The name of the event to send to Flurry
    @in     table params		(optional)The params to send to Flurry with the event or nil if not required.
    @out	nil
*/
int MOAIFlurryIOS::_endTimedEvent( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	NSString* eventName = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	
	if ( eventName == NULL )
		return 0;
    
    if ( state.IsType ( 2, LUA_TTABLE )) {
        NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
        [ paramsDict initWithLua:state stackIndex:2 ];
        [ Flurry endTimedEvent:eventName withParameters:paramsDict ];
        [ paramsDict release ];
    }
    else {
        [ Flurry endTimedEvent:eventName withParameters:nil ];
    }

	return 0;
}

//================================================================//
// MOAIFlurryIOS
//================================================================//

//----------------------------------------------------------------//
MOAIFlurryIOS::MOAIFlurryIOS () {
	
	RTTI_SINGLE ( MOAILuaObject )
	
}

//----------------------------------------------------------------//
MOAIFlurryIOS::~MOAIFlurryIOS () {
	
}

//----------------------------------------------------------------//
void MOAIFlurryIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "setAppVersion",		_setAppVersion},
		{ "setCrashTracking",	_setCrashTracking},
		{ "startSession",		_startSession },
		{ "logError",			_logError },
		{ "logEvent",			_logEvent },
		{ "logTimedEvent",		_logTimedEvent },
		{ "endTimedEvent",		_endTimedEvent },		
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
