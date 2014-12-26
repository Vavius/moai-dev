//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAICHARTBOOSTIOS_H
#define	MOAICHARTBOOSTIOS_H

//#if 0
//#ifndef DISABLE_CHARTBOOST

#include <moai-core/headers.h>
#import <Foundation/Foundation.h>

@class MoaiChartBoostDelegate;

//================================================================//
// MOAIChartBoostIOS
//================================================================//
class MOAIChartboostIOS :
	public MOAIGlobalClass < MOAIChartboostIOS, MOAILuaObject > {
private:

	MoaiChartBoostDelegate*	mDelegate;
    bool mPurchaseTracking;
        
	//----------------------------------------------------------------//
	static int	_hasCachedInterstitial	( lua_State* L );
	static int	_init					( lua_State* L );
	static int	_loadInterstitial		( lua_State* L );
    static int	_setListener			( lua_State* L );
    static int	_setPurchaseTracking    ( lua_State* L );
    static int	_showInterstitial		( lua_State* L );

    static int  _cacheVideo             ( lua_State* L );
    static int  _hasCachedVideo         ( lua_State* L );
    static int  _showVideo              ( lua_State* L );
	
public:
        
	DECL_LUA_SINGLETON ( MOAIChartboostIOS );

	enum {
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_DISMISSED,
		INTERSTITIAL_WILL_SHOW,
        VIDEO_WILL_SHOW,
        VIDEO_DISMISSED,
        VIDEO_REWARD,
		TOTAL
	};
	
	MOAILuaStrongRef	mListeners [ TOTAL ];
	
					MOAIChartboostIOS				();
					~MOAIChartboostIOS				();
	void 			NotifyInterstitialDismissed		();
	void 			NotifyInterstitialLoadFailed	();
	void 			NotifyInterstitialWillShow		();
    void 			NotifyVideoDismissed            ();
    void 			NotifyVideoReward               ( int reward );
    void 			NotifyVideoWillShow             ();
	void			RegisterLuaClass				( MOAILuaState& state );
    void            ReportPurchase                  ( NSData* receipt, id product );
};

//#endif  //DISABLE_CHARTBOOST

#endif  //MOAICHARTBOOSTIOS_H
