//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef DISABLE_VUNGLE

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <VungleSDK/VungleSDK.h>
#import <moai-ios-vungle/MOAIVungleIOS.h>

//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@interface MoaiVungleDelegate : NSObject < VungleSDKDelegate > {
@private
}
@end

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	adIsAvailable
	@text	Detects if an add has been downloaded
 
	@out 	bool	True, if an ad is cached and will be displayed.
 */
int MOAIVungleIOS::_adIsAvailable ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool isAdAvailable = [[ VungleSDK sharedSDK ] isCachedAdAvailable ];
	
	state.Push ( isAdAvailable );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	playModalAd
	@text	Play a video ad
 
    @opt    showClose
    @opt    orientation
	@out 	nil
*/
int	MOAIVungleIOS::_playModalAd	( lua_State* L ) {
	MOAILuaState state ( L );
	
    NSMutableDictionary* options = [ NSMutableDictionary dictionary ];
    
    if ( state.IsType ( 1, LUA_TBOOLEAN )) {
        [ options setObject:[ NSNumber numberWithBool: state.GetValue < bool >( 1, true )]
                     forKey:VunglePlayAdOptionKeyShowClose ];
    }
    
    if ( state.IsType ( 2, LUA_TNUMBER )) {
        [ options setObject:@( state.GetValue < u32 >( 2, UIInterfaceOrientationMaskAll ))
                     forKey:VunglePlayAdOptionKeyOrientations ];
    }
    
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
    
    [[ VungleSDK sharedSDK ] playAd:rootVC withOptions:options ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	playIncentivizedAd
	@text	Plays an incentivised ad. You can use the listeners to determine if the full movie was watched by the user.
    
    @opt    userId
    @opt    showClose
    @opt    orientation
	@out 	nil
*/
int	MOAIVungleIOS::_playIncentivizedAd ( lua_State* L ) {
	MOAILuaState state ( L );
	
    NSMutableDictionary* options = [ NSMutableDictionary dictionaryWithObject:@(YES)
                                                                       forKey:VunglePlayAdOptionKeyIncentivized ];

    if ( state.IsType ( 1, LUA_TSTRING )) {
        [ options setObject:[ NSString stringWithUTF8String: state.GetValue < cc8* >( 1, "" )]
                     forKey:VunglePlayAdOptionKeyUser ];
    }
    
    if ( state.IsType ( 2, LUA_TBOOLEAN )) {
        [ options setObject:@( state.GetValue < bool >( 2, true ))
                     forKey:VunglePlayAdOptionKeyShowClose ];
    }
    
    if ( state.IsType ( 3, LUA_TNUMBER )) {
        [ options setObject:@( state.GetValue < u32 >( 3, UIInterfaceOrientationMaskAll ))
                     forKey:VunglePlayAdOptionKeyOrientations ];
    }
    
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
	
	[[ VungleSDK sharedSDK ] playAd:rootVC withOptions:options ];
	
	return 0;
}
	
//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Vungle.
	
	@in		string	appId			Available in Vungle dashboard settings.
	@out 	nil
*/
int MOAIVungleIOS::_init ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	
	if ( identifier ) {
        
        VungleSDK* vungle = [ VungleSDK sharedSDK ];
        [ vungle setDelegate: MOAIVungleIOS::Get ().mDelegate ];
        [ vungle startWithAppId:[ NSString stringWithUTF8String:identifier ]];
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVungleIOS::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAIVungleIOS::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}
	
	return 0;
}


//================================================================//
// MOAIVungleIOS
//================================================================//

//----------------------------------------------------------------//
MOAIVungleIOS::MOAIVungleIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	mDelegate = [[ MoaiVungleDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIVungleIOS::~MOAIVungleIOS () {
	[ mDelegate release ];
}

//----------------------------------------------------------------//
void MOAIVungleIOS::NotifyMovieCached () {
    
	MOAILuaRef& callback = this->mListeners [ MOVIE_CACHED ];
	
	if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIVungleIOS::NotifyMoviePlayed ( bool willShowProductSheet, bool playedFull, float playTime, bool didDownload ) {	
	
	MOAILuaRef& callback = this->mListeners [ MOVIE_PLAYED ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		
		state.Push ( willShowProductSheet );
        state.Push ( playedFull );
        state.Push ( playTime );
        state.Push ( didDownload );
		
		state.DebugCall ( 4, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIVungleIOS::NotifyMovieWillAppear () {

    MOAILuaRef& callback = this->mListeners [ MOVIE_WILL_APPEAR ];
	
	if ( callback ) {
        
		MOAIScopedLuaState state = callback.GetSelf ();
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIVungleIOS::NotifyProductSheetClosed () {
    
    MOAILuaRef& callback = this->mListeners [ PRODUCT_SHEET_CLOSED ];
	
	if ( callback ) {
        
		MOAIScopedLuaState state = callback.GetSelf ();
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIVungleIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "MOVIE_PLAYED",            ( u32 )MOVIE_PLAYED );
    state.SetField ( -1, "MOVIE_CACHED",            ( u32 )MOVIE_CACHED );
	state.SetField ( -1, "MOVIE_WILL_APPEAR",       ( u32 )MOVIE_WILL_APPEAR );
	state.SetField ( -1, "PRODUCT_SHEET_CLOSED",    ( u32 )PRODUCT_SHEET_CLOSED );
    state.SetField ( -1, "ORIENTATION_ALL",         ( u32 )UIInterfaceOrientationMaskAll );
    state.SetField ( -1, "ORIENTATION_LANDSCAPE",   ( u32 )UIInterfaceOrientationMaskLandscape );
    state.SetField ( -1, "ORIENTATION_PORTRAIT",    ( u32 )UIInterfaceOrientationMaskPortrait );

	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ "adIsAvailable",		_adIsAvailable },
		{ "playModalAd",		_playModalAd },
		{ "playIncentivizedAd", _playIncentivizedAd },
		{ "setListener",		_setListener },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// MoaiVungleDelegate
//================================================================//
@implementation MoaiVungleDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MoaiVungleDelegate
	//================================================================//

    - ( void) vungleSDKwillCloseAdWithViewInfo:(NSDictionary *)viewInfo willPresentProductSheet:(BOOL)willPresentProductSheet {
        
        bool completedView = [ viewInfo valueForKey:@"completedView" ];
        bool playTime = [ viewInfo valueForKey:@"playTime" ];
        bool didDownload = [ viewInfo valueForKey:@"didDownload" ];
        
        MOAIVungleIOS::Get ().NotifyMoviePlayed ( willPresentProductSheet, completedView, playTime, didDownload );
    }

    - ( void ) vungleSDKwillCloseProductSheet:(id)productSheet {
        MOAIVungleIOS::Get ().NotifyProductSheetClosed();
    }

    - ( void ) vungleSDKwillShowAd {
        MOAIVungleIOS::Get ().NotifyMovieWillAppear();
    }

    - ( void ) vungleSDKhasCachedAdAvailable {
        MOAIVungleIOS::Get ().NotifyMovieCached ();
    }

@end

#endif