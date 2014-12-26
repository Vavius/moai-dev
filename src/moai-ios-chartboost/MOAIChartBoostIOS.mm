//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

//#if 0
//#ifndef DISABLE_CHARTBOOST

#import <moai-ios-chartboost/MOAIChartboostIOS.h>
#import <UIKit/UIKit.h>
#import <Chartboost/Chartboost.h>
#import <Chartboost/CBAnalytics.h>

//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@interface MoaiChartBoostDelegate : NSObject < ChartboostDelegate > {
@private
}
@end

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	hasCachedInterstitial
    @text	returns whether a cached ad is available
    
    @opt    string  locationId
    @out 	bool	True, if an ad is cached.
*/
int MOAIChartboostIOS::_hasCachedInterstitial ( lua_State* L ) {
	MOAILuaState state ( L );
    
//    u32 location = state.GetValue < cc8* >( 1, );
    bool isAdAvailable = [ Chartboost hasInterstitial: CBLocationDefault ];
    state.Push ( isAdAvailable );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize ChartBoost.
	
	@in		string	appId			Available in ChartBoost dashboard settings.
	@in 	string	appSignature	Available in ChartBoost dashboard settings.
	@out 	nil
*/
int MOAIChartboostIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
    
	cc8* identifier = lua_tostring ( state, 1 );
	cc8* signature = lua_tostring ( state, 2 );
	
    [ Chartboost startWithAppId:[ NSString stringWithUTF8String:identifier ]
                   appSignature:[ NSString stringWithUTF8String:signature ]
                       delegate:MOAIChartboostIOS::Get ().mDelegate ];
    
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadInterstitial
	@text	Request that an interstitial ad be cached for later display.
	
	@opt	string	locationId		Optional location ID.
	@out 	nil
*/
int MOAIChartboostIOS::_loadInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );
	
    [ Chartboost cacheInterstitial:CBLocationDefault ];
			
	return 0;
}

//----------------------------------------------------------------//
int MOAIChartboostIOS::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAIChartboostIOS::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPurchaseTracking
    @text	Send all successful in-app purchases to Chartboost analytics
 
    @in     bool    enabled
    @out    nil
*/
int MOAIChartboostIOS::_setPurchaseTracking ( lua_State* L ) {
    
	MOAILuaState state ( L );
    
    bool enabled = state.GetValue < bool >( 1, true );
    MOAIChartboostIOS::Get ().mPurchaseTracking = enabled;

    return 0;
}

//----------------------------------------------------------------//
/**	@name	showInterstitial
	@text	Request an interstitial ad display if a cached ad is available.
	
	@opt	string	locationId		Optional location ID.
	@out 	bool					True, if an ad is cached and will be displayed.
*/
int MOAIChartboostIOS::_showInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );
    
    bool result = false;
    
    if ([ Chartboost hasInterstitial:CBLocationDefault ]) {
    
        [ Chartboost showInterstitial:CBLocationDefault ];
        result = true;
    }
    
    state.Push ( result );
	return 1;
}

//----------------------------------------------------------------//
int MOAIChartboostIOS::_cacheVideo ( lua_State* L ) {
    
	MOAILuaState state ( L );
    
    [ Chartboost cacheRewardedVideo:CBLocationDefault ];
    
    return 0;
}

//----------------------------------------------------------------//
int MOAIChartboostIOS::_hasCachedVideo ( lua_State* L ) {
    
	MOAILuaState state ( L );
    
    bool result = [ Chartboost hasRewardedVideo:CBLocationDefault ];
    state.Push ( result );
    return 1;
}

//----------------------------------------------------------------//
int MOAIChartboostIOS::_showVideo ( lua_State* L ) {
    
    MOAILuaState state ( L );
    
    [[ UIDevice currentDevice ] setValue: [ NSNumber numberWithInteger: UIInterfaceOrientationLandscapeLeft ] forKey:@"orientation" ];
    [ Chartboost showRewardedVideo:CBLocationDefault ];
    
    return 0;
}

//================================================================//
// MOAIChartBoostIOS
//================================================================//

//----------------------------------------------------------------//
MOAIChartboostIOS::MOAIChartboostIOS () :
    mPurchaseTracking ( false ) {
    
	RTTI_SINGLE ( MOAILuaObject )	
    
	mDelegate = [[ MoaiChartBoostDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIChartboostIOS::~MOAIChartboostIOS () {

	[ mDelegate release ];
}

//----------------------------------------------------------------//
void MOAIChartboostIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "INTERSTITIAL_LOAD_FAILED",	( u32 )INTERSTITIAL_LOAD_FAILED );
	state.SetField ( -1, "INTERSTITIAL_DISMISSED", 		( u32 )INTERSTITIAL_DISMISSED );
	state.SetField ( -1, "INTERSTITIAL_WILL_SHOW", 		( u32 )INTERSTITIAL_WILL_SHOW );
    state.SetField ( -1, "VIDEO_WILL_SHOW",             ( u32 )VIDEO_WILL_SHOW );
    state.SetField ( -1, "VIDEO_DISMISSED",             ( u32 )VIDEO_DISMISSED );
    state.SetField ( -1, "VIDEO_REWARD",                ( u32 )VIDEO_REWARD );
    
    
	luaL_Reg regTable [] = {
		{ "hasCachedInterstitial",	_hasCachedInterstitial },
		{ "init",					_init },
		{ "loadInterstitial",		_loadInterstitial },
        { "setPurchaseTracking",    _setPurchaseTracking },
		{ "setListener",			_setListener },
		{ "showInterstitial",		_showInterstitial },
        { "cacheVideo",             _cacheVideo },
        { "hasCachedVideo",         _hasCachedVideo },
        { "showVideo",              _showVideo },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIChartboostIOS::NotifyInterstitialDismissed () {
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_DISMISSED ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostIOS::NotifyInterstitialLoadFailed () {
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_LOAD_FAILED ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostIOS::NotifyInterstitialWillShow () {
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_WILL_SHOW ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostIOS::NotifyVideoDismissed () {
	
	MOAILuaRef& callback = this->mListeners [ VIDEO_DISMISSED ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostIOS::NotifyVideoReward ( int reward ) {
	
	MOAILuaRef& callback = this->mListeners [ VIDEO_REWARD ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		state.Push ( reward );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostIOS::NotifyVideoWillShow () {
	
	MOAILuaRef& callback = this->mListeners [ VIDEO_WILL_SHOW ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostIOS::ReportPurchase ( NSData* receipt, id product ) {
    
    if ( this->mPurchaseTracking ) {
        [ CBAnalytics trackInAppPurchaseEvent:receipt product:product ];
    }
}


//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@implementation MoaiChartBoostDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MoaiChartBoostDelegate
	//================================================================//

	- ( BOOL ) shouldRequestInterstitial:( CBLocation )location {
		
		return YES;
	}

    - ( void ) didFailToLoadInterstitial:( CBLocation )location withError:( CBLoadError )error {
		
		MOAIChartboostIOS::Get ().NotifyInterstitialLoadFailed ();
	}

	- ( BOOL ) shouldDisplayInterstitial:( CBLocation )location {
		
		MOAIChartboostIOS::Get ().NotifyInterstitialWillShow ();
		return YES;
	}

	- ( void ) didDismissInterstitial:( CBLocation )location {
		
		MOAIChartboostIOS::Get ().NotifyInterstitialDismissed ();
	}

	- ( BOOL ) shouldDisplayMoreApps:( CBLocation )location {
		
		return YES;
	}


    - ( BOOL ) shouldDisplayRewardedVideo:( CBLocation )location {
        
        MOAIChartboostIOS::Get ().NotifyVideoWillShow ();
        return YES;
    }

    - ( void ) didDismissRewardedVideo:( CBLocation )location {
        
        MOAIChartboostIOS::Get ().NotifyVideoDismissed ();
    }

    - ( void ) didCompleteRewardedVideo:( CBLocation )location withReward:( int )reward {
        
        MOAIChartboostIOS::Get ().NotifyVideoReward ( reward );
    }
	
@end

//#endif
