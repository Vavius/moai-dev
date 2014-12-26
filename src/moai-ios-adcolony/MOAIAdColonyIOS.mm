//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef DISABLE_ADCOLONY

#import <moai-ios-adcolony/MOAIAdColonyIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
cc8* MOAIAdColonyIOS::_luaParseTable ( lua_State* L, int idx ) {

	switch ( lua_type ( L, idx )) {

		case LUA_TSTRING: {

			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}

//----------------------------------------------------------------//
/**	@name	getDeviceID
	@text	Request a unique ID for the device.
	
	@out 	string	id			The device ID. Always returns nil.
*/
int MOAIAdColonyIOS::_getDeviceID ( lua_State *L ) {

	MOAILuaState state ( L );

	lua_pushnil ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize AdColony.
	
	@in		string	appId			Available in AdColony dashboard settings.
	@in 	table	zones			A list of zones to configure. Available in AdColony dashboard settings.
	@out 	nil
*/
int MOAIAdColonyIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
    
	cc8* appID = state.GetValue < cc8* >( 1, "" );

	NSMutableArray * zones = [[ NSMutableArray alloc ] init ];
	if ( state.IsType ( 2, LUA_TTABLE )) {
	
		for ( int key = 1; ; ++key ) {
	
			state.GetField ( 2, key );
			cc8* value = _luaParseTable ( state, -1 );
			lua_pop ( state, 1 );
	
			if ( value ) {
				
				[ zones addObject:[ NSString stringWithUTF8String:value ] ];
			}
			else {
				
				break;
			}	
		}
	}
    
	[ AdColony configureWithAppID: [ NSString stringWithUTF8String:appID ] zoneIDs:zones
						 delegate:MOAIAdColonyIOS::Get ().mAdColonyDelegate logging:YES];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	playVideo
	@text	Play an AdColony video ad.
	
	@in 	string	zone			The zone from which to play a video ad.
	@opt	bool	prompt			Determines whether the user is asked whether they want to play a video ad or not. Default is true.
	@opt	bool	confirm			Determines whether the user is presented with a confirmation dialog after video ad playback completes. Default is true.
	@out 	nil
*/
int MOAIAdColonyIOS::_playVideo ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* zone = lua_tostring ( state, 1 );
	
	bool prompt = state.GetValue < bool >( 2, true );
	bool confirmation = state.GetValue < bool >( 3, true );
	
	id delegate = MOAIAdColonyIOS::Get ().mTakeoverDelegate;
	[ AdColony playVideoAdForZone:[ NSString stringWithUTF8String:zone ] withDelegate:delegate withV4VCPrePopup:prompt andV4VCPostPopup:confirmation ];

	return 0;
}

//----------------------------------------------------------------//
/**	@name	videoReadyForZone
	@text	Check the readiness of a video ad for a given zone.
	
	@in 	string	zone			The zone from which to check for a video ad.
	@out 	bool					True, if a video ad is ready to play.
*/
int MOAIAdColonyIOS::_videoReadyForZone ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	cc8* zone = lua_tostring ( state, 1 );
	
	int result = [ AdColony zoneStatusForZone:[ NSString stringWithUTF8String:zone ]];
	if ( result == ADCOLONY_ZONE_STATUS_ACTIVE ) {
		
		lua_pushboolean ( L, true );
	}
	else {
		
		lua_pushboolean ( L, false );
	}
	
	return 1;
}
	
//================================================================//
// MOAIAdColonyIOS
//================================================================//

//----------------------------------------------------------------//
void MOAIAdColonyIOS::NotifyTakeoverEventOccurred ( int event, cc8* zone ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( event, state )) {
		
		state.Push ( zone );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAdColonyIOS::NotifyAvailabilityChange ( BOOL available, cc8 *zone ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( AVAILABILITY_CHANGE, state )) {
		
		state.Push ( available );
		state.Push ( zone );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAdColonyIOS::NotifyV4VCReward ( BOOL success, cc8 *currencyName, int amount, cc8 *zone ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( REWARD, state )) {
		
		state.Push ( success );
		state.Push ( currencyName );
		state.Push ( amount );
		state.Push ( zone );
		state.DebugCall ( 4, 0 );
	}
}

//----------------------------------------------------------------//
MOAIAdColonyIOS::MOAIAdColonyIOS () {
    
	RTTI_SINGLE ( MOAILuaObject )
	
	mAdColonyDelegate = [[ MOAIAdColonyIOSDelegate alloc ] init];
	mTakeoverDelegate = [[ MOAIAdColonyIOSTakeoverDelegate alloc ] init];
}

//----------------------------------------------------------------//
MOAIAdColonyIOS::~MOAIAdColonyIOS () {
    
	[ mAdColonyDelegate release ];
	[ mTakeoverDelegate release ];
}

//----------------------------------------------------------------//
void MOAIAdColonyIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "VIDEO_BEGAN_IN_ZONE", 	( u32 )VIDEO_BEGAN_IN_ZONE );
	state.SetField ( -1, "VIDEO_ENDED_IN_ZONE", 	( u32 )VIDEO_ENDED_IN_ZONE );
	state.SetField ( -1, "VIDEO_FAILED_IN_ZONE", 	( u32 )VIDEO_FAILED_IN_ZONE );
	state.SetField ( -1, "VIDEO_PAUSED_IN_ZONE", 	( u32 )VIDEO_PAUSED_IN_ZONE );
	state.SetField ( -1, "VIDEO_RESUMED_IN_ZONE",	( u32 )VIDEO_RESUMED_IN_ZONE );
	state.SetField ( -1, "AVAILABILITY_CHANGE",		( u32 )AVAILABILITY_CHANGE );
	state.SetField ( -1, "REWARD",					( u32 )REWARD );
    
	luaL_Reg regTable [] = {
		{ "getDeviceID",		_getDeviceID },
		{ "getListener",		&MOAIGlobalEventSource::_getListener < MOAIAdColonyIOS > },
		{ "init",				_init },
		{ "playVideo",			_playVideo },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIAdColonyIOS > },
		{ "videoReadyForZone",	_videoReadyForZone },
		{ NULL, NULL }	
	};
    
	luaL_register( state, 0, regTable );
}

//================================================================//
// MOAIAdColonyIOSDelegate
//================================================================//
@implementation MOAIAdColonyIOSDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MOAIAdColonyIOSDelegate
	//================================================================//

	- ( void ) onAdColonyAdAvailabilityChange:(BOOL)available inZone:(NSString*) zoneID {
		
		MOAIAdColonyIOS::Get ().NotifyAvailabilityChange ( available, [ zoneID UTF8String ] );
	}

	- ( void ) onAdColonyV4VCReward:(BOOL)success currencyName:(NSString*)currencyName currencyAmount:(int)amount inZone:(NSString*)zoneID {
		
		MOAIAdColonyIOS::Get ().NotifyV4VCReward ( success, [ currencyName UTF8String ], amount, [ zoneID UTF8String ] );
	}

@end

//================================================================//
// MOAIAdColonyIOSTakeoverDelegate
//================================================================//
@implementation MOAIAdColonyIOSTakeoverDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MOAIAdColonyIOSTakeoverDelegate
	//================================================================//

	- (void) onAdColonyAdStartedInZone:( NSString * )zone {
		
		MOAIAdColonyIOS::Get ().NotifyTakeoverEventOccurred ( MOAIAdColonyIOS::Get ().VIDEO_BEGAN_IN_ZONE, [ zone UTF8String ]);
	}

	- (void) onAdColonyAdAttemptFinished:(BOOL)shown inZone:( NSString * )zone {
		
		if ( shown ) {
			MOAIAdColonyIOS::Get ().NotifyTakeoverEventOccurred ( MOAIAdColonyIOS::Get ().VIDEO_ENDED_IN_ZONE, [ zone UTF8String ]);
		}
		else {
			MOAIAdColonyIOS::Get ().NotifyTakeoverEventOccurred ( MOAIAdColonyIOS::Get ().VIDEO_FAILED_IN_ZONE, [ zone UTF8String ]);
		}
	}

@end

#endif