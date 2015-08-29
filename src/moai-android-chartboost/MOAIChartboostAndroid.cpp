// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <moai-android/JniUtils.h>
#include <moai-android-chartboost/MOAIChartboostAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Chartboost.
	
	@in		string	appId			Available in Chartboost dashboard settings.
	@in 	string	appSignature	Available in Chartboost dashboard settings.
	@out 	nil
*/
int MOAIChartboostAndroid::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	cc8* signature = lua_tostring ( state, 2 );

	JNI_GET_ENV ( jvm, env );
	
	MOAIJString jidentifier = JNI_GET_JSTRING ( identifier );
	MOAIJString jsignature = JNI_GET_JSTRING ( signature );

	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartboost" );
    if ( chartboost == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartboost" );
    } else {

    	jmethodID init = env->GetStaticMethodID ( chartboost, "init", "(Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( init == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find static java method %s", "init" );
    	} else {

			env->CallStaticVoidMethod ( chartboost, init, ( jstring ) jidentifier, ( jstring ) jsignature );				
		}
	}
			
	return 0;
}

//----------------------------------------------------------------//
/**	@name	hasCachedVideo

	@out 	boolean
*/
int MOAIChartboostAndroid::_hasCachedVideo ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	MOAIJString jlocation = JNI_GET_JSTRING ( location );
	
	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartboost" ); 
    if ( chartboost == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartboost" );
    } else {

    	jmethodID hasCachedVideo = env->GetStaticMethodID ( chartboost, "hasCachedVideo", "(Ljava/lang/String;)Z" );
    	if ( hasCachedVideo == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find static java method %s", "hasCachedVideo" );
    	} else {

			lua_pushboolean ( state, env->CallStaticBooleanMethod( chartboost, hasCachedVideo, ( jstring ) jlocation ) );

			return 1;
		}
	}

	lua_pushboolean ( state, false );
			
	return 1;
}

//----------------------------------------------------------------//
/**	@name	hasCachedInterstitial

	@out 	boolean
*/
int MOAIChartboostAndroid::_hasCachedInterstitial ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	MOAIJString jlocation = JNI_GET_JSTRING ( location );
	
	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartboost" ); 
    if ( chartboost == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartboost" );
    } else {

    	jmethodID hasCachedInterstitial = env->GetStaticMethodID ( chartboost, "hasCachedInterstitial", "(Ljava/lang/String;)Z" );
    	if ( hasCachedInterstitial == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find static java method %s", "hasCachedInterstitial" );
    	} else {

			lua_pushboolean ( state, env->CallStaticBooleanMethod( chartboost, hasCachedInterstitial, ( jstring ) jlocation ) );

			return 1;
		}
	}

	lua_pushboolean ( state, false );
			
	return 1;
}

//----------------------------------------------------------------//
/**	@name	cacheVideo
	@text	Request that an video ad be cached for later display.
	
	@opt	string	locationId		Optional location ID.
	@out 	nil
*/
int MOAIChartboostAndroid::_cacheVideo ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	MOAIJString jlocation = JNI_GET_JSTRING ( location );

	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartboost" );
    if ( chartboost == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartboost" );
    } else {

    	jmethodID cacheVideo = env->GetStaticMethodID ( chartboost, "cacheVideo", "(Ljava/lang/String;)V" );
    	if ( cacheVideo == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find static java method %s", "cacheVideo" );
    	} else {

			env->CallStaticVoidMethod ( chartboost, cacheVideo, ( jstring ) jlocation );			
		}
	}
			
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadInterstitial
	@text	Request that an interstitial ad be cached for later display.
	
	@opt	string	locationId		Optional location ID.
	@out 	nil
*/
int MOAIChartboostAndroid::_loadInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	MOAIJString jlocation = JNI_GET_JSTRING ( location );

	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartboost" );
    if ( chartboost == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartboost" );
    } else {

    	jmethodID loadInterstitial = env->GetStaticMethodID ( chartboost, "loadInterstitial", "(Ljava/lang/String;)V" );
    	if ( loadInterstitial == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find static java method %s", "loadInterstitial" );
    	} else {

			env->CallStaticVoidMethod ( chartboost, loadInterstitial, ( jstring ) jlocation );			
		}
	}
			
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPurchaseTracking
    @text	Send all successful in-app purchases to Chartboost analytics
 
    @in     bool    enabled
    @out    nil
*/
int MOAIChartboostAndroid::_setPurchaseTracking ( lua_State* L ) {
    
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );
    
	bool enabled = lua_toboolean ( state, 1 );

	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartboost" );
    if ( chartboost == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartboost" );
    } else {

    	jmethodID setPurchaseTracking = env->GetStaticMethodID ( chartboost, "setPurchaseTracking", "(Z)V" );
    	if ( setPurchaseTracking == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find static java method %s", "setPurchaseTracking" );
    	} else {

			env->CallStaticVoidMethod ( chartboost, setPurchaseTracking, enabled );			
		}
	}

    return 0;
}

//----------------------------------------------------------------//
int MOAIChartboostAndroid::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAIChartboostAndroid::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showVideo
	@text	Request an video ad display if a cached ad is available.
	
	@opt	string	locationId		Optional location ID.
	@out 	nil
*/
int MOAIChartboostAndroid::_showVideo ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	MOAIJString jlocation = JNI_GET_JSTRING ( location );

	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartboost" );
    if ( chartboost == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartboost" );
    } else {

    	jmethodID showVideo = env->GetStaticMethodID ( chartboost, "showVideo", "(Ljava/lang/String;)V" );
    	if ( showVideo == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find static java method %s", "showVideo" );
    	} else {

			env->CallStaticVoidMethod ( chartboost, showVideo, ( jstring ) jlocation );				
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	showInterstitial
	@text	Request an interstitial ad display if a cached ad is available.
	
	@opt	string	locationId		Optional location ID.
	@out 	nil
*/
int MOAIChartboostAndroid::_showInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	MOAIJString jlocation = JNI_GET_JSTRING ( location );

	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartboost" );
    if ( chartboost == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartboost" );
    } else {

    	jmethodID showInterstitial = env->GetStaticMethodID ( chartboost, "showInterstitial", "(Ljava/lang/String;)V" );
    	if ( showInterstitial == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartboostAndroid: Unable to find static java method %s", "showInterstitial" );
    	} else {

			env->CallStaticVoidMethod ( chartboost, showInterstitial, ( jstring ) jlocation );				
		}
	}

	return 0;
}

//================================================================//
// MOAIChartboostAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIChartboostAndroid::MOAIChartboostAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIChartboostAndroid::~MOAIChartboostAndroid () {

}

//----------------------------------------------------------------//
void MOAIChartboostAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "INTERSTITIAL_DISMISSED", 		( u32 )INTERSTITIAL_DISMISSED );
	state.SetField ( -1, "INTERSTITIAL_LOAD_FAILED",	( u32 )INTERSTITIAL_LOAD_FAILED );
	state.SetField ( -1, "INTERSTITIAL_WILL_SHOW", 		( u32 )INTERSTITIAL_WILL_SHOW );
    state.SetField ( -1, "VIDEO_DISMISSED",             ( u32 )VIDEO_DISMISSED );
    state.SetField ( -1, "VIDEO_REWARD",                ( u32 )VIDEO_REWARD );
    state.SetField ( -1, "VIDEO_WILL_SHOW",             ( u32 )VIDEO_WILL_SHOW );

	luaL_Reg regTable [] = {
        { "cacheVideo",             _cacheVideo },
		{ "hasCachedInterstitial",	_hasCachedInterstitial },
        { "hasCachedVideo",         _hasCachedVideo },
		{ "init",					_init },
		{ "loadInterstitial",		_loadInterstitial },
		{ "setListener",			_setListener },
		{ "setPurchaseTracking",    _setPurchaseTracking },
		{ "showInterstitial",		_showInterstitial },
        { "showVideo",              _showVideo },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIChartboostAndroid::NotifyInterstitialDismissed ( cc8* location ) {	
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_DISMISSED ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		
		state.Push ( location );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostAndroid::NotifyInterstitialLoadFailed ( cc8* location ) {	
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_LOAD_FAILED ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		
		state.Push ( location );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostAndroid::NotifyInterstitialWillShow ( cc8* location ) {	
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_WILL_SHOW ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		
		state.Push ( location );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostAndroid::NotifyVideoDismissed ( cc8* location ) {	
	
	MOAILuaRef& callback = this->mListeners [ VIDEO_DISMISSED ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		
		state.Push ( location );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostAndroid::NotifyVideoReward ( cc8* location, int reward ) {	
	
	MOAILuaRef& callback = this->mListeners [ VIDEO_REWARD ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();

		state.Push ( reward );
		state.Push ( location );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostAndroid::NotifyVideoWillShow ( cc8* location ) {	
	
	MOAILuaRef& callback = this->mListeners [ VIDEO_WILL_SHOW ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		
		state.Push ( location );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartboostAndroid::ReportPurchase ( jobject receipt, cc8* product ) {

	MOAILuaRef& callback = this->mListeners [ VIDEO_WILL_SHOW ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();

		//Zaglushka
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// Chartboost JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiChartboost_AKUNotifyChartboostInterstitialDismissed ( JNIEnv* env, jclass obj, jstring jlocation ) {

    JNI_GET_CSTRING ( jlocation, location );

	MOAIChartboostAndroid::Get ().NotifyInterstitialDismissed ( location );

	JNI_RELEASE_CSTRING ( jlocation, location );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiChartboost_AKUNotifyChartboostInterstitialLoadFailed ( JNIEnv* env, jclass obj, jstring jlocation ) {

    JNI_GET_CSTRING ( jlocation, location );

	MOAIChartboostAndroid::Get ().NotifyInterstitialLoadFailed ( location );

	JNI_RELEASE_CSTRING ( jlocation, location );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiChartboost_AKUNotifyChartboostInterstitialWillShow ( JNIEnv* env, jclass obj, jstring jlocation ) {

    JNI_GET_CSTRING ( jlocation, location );

	MOAIChartboostAndroid::Get ().NotifyInterstitialWillShow ( location );

	JNI_RELEASE_CSTRING ( jlocation, location );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiChartboost_AKUNotifyChartboostVideoDismissed ( JNIEnv* env, jclass obj, jstring jlocation ) {

	JNI_GET_CSTRING ( jlocation, location );

	MOAIChartboostAndroid::Get ().NotifyVideoDismissed ( location );

	JNI_RELEASE_CSTRING ( jlocation, location );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiChartboost_AKUNotifyChartboostVideoReward ( JNIEnv* env, jclass obj, jstring jlocation, jint reward ) {

	JNI_GET_CSTRING ( jlocation, location );

	MOAIChartboostAndroid::Get ().NotifyVideoReward ( location, reward );

	JNI_RELEASE_CSTRING ( jlocation, location );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiChartboost_AKUNotifyChartboostVideoWillShow ( JNIEnv* env, jclass obj, jstring jlocation ) {

	JNI_GET_CSTRING ( jlocation, location );

	MOAIChartboostAndroid::Get ().NotifyVideoWillShow ( location );

	JNI_RELEASE_CSTRING ( jlocation, location );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiChartboost_AKUNotifyChartboostReportPurchase ( JNIEnv* env, jclass obj, jobject receipt, jstring jproduct ) {

	JNI_GET_CSTRING ( jproduct, product );

	MOAIChartboostAndroid::Get ().ReportPurchase ( receipt, product );

	JNI_RELEASE_CSTRING ( jproduct, product );
}