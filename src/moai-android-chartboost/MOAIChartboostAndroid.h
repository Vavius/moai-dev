// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICHARTBOOSTANDROID_H
#define	MOAICHARTBOOSTANDROID_H

#include <moai-core/headers.h>

#include <jni.h>

//================================================================//
// MOAIChartboostAndroid
//================================================================//
class MOAIChartboostAndroid :
	public MOAIGlobalClass < MOAIChartboostAndroid, MOAILuaObject > {
private:
	//----------------------------------------------------------------//
    static int  _cacheVideo             ( lua_State* L );
	static int	_hasCachedInterstitial 	( lua_State* L );
    static int  _hasCachedVideo         ( lua_State* L );
	static int	_init 					( lua_State* L );
	static int	_loadInterstitial 		( lua_State* L );
	static int	_setListener			( lua_State* L );
    static int	_setPurchaseTracking    ( lua_State* L );
    static int	_showInterstitial		( lua_State* L );
    static int  _showVideo              ( lua_State* L );


public:

	DECL_LUA_SINGLETON ( MOAIChartboostAndroid );

	enum {
		INTERSTITIAL_DISMISSED,
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_WILL_SHOW,
        VIDEO_DISMISSED,
        VIDEO_WILL_SHOW,
        VIDEO_REWARD,
		TOTAL
	};

	MOAILuaStrongRef		mListeners [ TOTAL ];

				MOAIChartboostAndroid			();
				~MOAIChartboostAndroid			();
	void 		NotifyInterstitialDismissed		( cc8* location );
	void 		NotifyInterstitialLoadFailed	( cc8* location );
	void 		NotifyInterstitialWillShow		( cc8* location );
    void 		NotifyVideoDismissed            ( cc8* location );
    void 		NotifyVideoReward               ( cc8* location, int reward );
    void 		NotifyVideoWillShow             ( cc8* location );
	void		RegisterLuaClass				( MOAILuaState& state );
    void        ReportPurchase                  ( jobject receipt, cc8* product );
};

#endif  //MOAICHARTBOOST_H
