//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef DISABLE_ADTOAPP

#import <moai-ios-adtoapp/MOAIAdToAppIOS.h>
#import <AdToAppSDK.h>
#import <ATALog.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize AdToApp.
	
	@in		string	appId			Available on AdToApp.com
	@opt 	boolean	verbose 		Debug log. Default is false
	@out 	nil
*/
int MOAIAdToAppIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* identifier = state.GetValue < cc8* >( 1, 0 );
	bool verbose 	= state.GetValue < bool >( 2, false );
	
	if ( verbose ) {
		ataloglevel = ATALogFatal | ATALogError | ATALogWarn | ATALogDebug | ATALogVerbose;
	}
	
	if ( identifier ) {
		NSString* appId = [ NSString stringWithUTF8String:identifier ];
		[ AdToAppSDK startWithAppId: appId ];
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	showInterstitial
	@text	Request an interstitial ad display if a cached ad is available.
	
	@out 	bool					True, if an ad is cached and will be displayed.
*/
int MOAIAdToAppIOS::_showInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	[ AdToAppSDK showInterstitial ];

	return 0;
}

//================================================================//
// MOAIAdToAppIOS
//================================================================//

//----------------------------------------------------------------//
MOAIAdToAppIOS::MOAIAdToAppIOS () {

	RTTI_SINGLE ( MOAILuaObject )

}

//----------------------------------------------------------------//
MOAIAdToAppIOS::~MOAIAdToAppIOS () {

}

//----------------------------------------------------------------//
void MOAIAdToAppIOS::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "init",					_init },
		{ "showInterstitial",		_showInterstitial },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

#endif