// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android-facebook/MOAIFacebookAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIFacebookAndroid::_declinedPermissions ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	jobjectArray jpermissions = ( jobjectArray )self->CallStaticObjectMethod ( self->mJava_DeclinedPermissions );

	if ( jpermissions ) {
		int stringCount = self->Env ()->GetArrayLength ( jpermissions );
		lua_newtable ( state );

		for ( int i=0; i<stringCount; i++ ) {
			jstring jpermission = ( jstring )self->Env ()->GetObjectArrayElement ( jpermissions, i );
			lua_pushnumber ( state, i+1 );
			cc8* permission = self->GetCString ( jpermission );
			lua_pushstring ( state, permission );
			self->ReleaseCString ( jpermission, permission );
			lua_settable ( state, -3 );
		}
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	graphRequest
    @text	Make a request on Facebook's Graph API

	@in		string  path
	@opt	table  parameters
    @out	nil
*/
int MOAIFacebookAndroid::_graphRequest ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	jstring jmethod = self->GetJString ( state.GetValue < cc8* >( 1, "GET" ) );
	jstring jpath = self->GetJString ( state.GetValue < cc8* >( 2, "" ) );
    jobject jbundle;
    if ( state.IsType ( 3, LUA_TTABLE ) ) {
       jbundle = self->BundleFromLua( L, 3 );
    }
	int eventIdx = state.GetValue < int >( 4, REQUEST_RESPONSE );
	self->CallStaticVoidMethod ( self->mJava_GraphRequest, jmethod, jpath, jbundle, eventIdx );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	hasGranted
	@text	Determine whether or not the current permission is granted.
				
	@out 	boolean	valid
*/
int MOAIFacebookAndroid::_hasGranted ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	cc8* permission = state.GetValue < cc8* > ( 1, 0 );
	
	if ( permission ) {
		
		jstring jpermission = self->GetJString ( permission );
		state.Push ( self->CallStaticBooleanMethod ( self->mJava_HasGranted, jpermission ));
	}
	else {
	
		state.Push ( false );
	}

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize Facebook.
				
	@in		string	appId			Available in Facebook developer settings.
	@out 	nil
*/
int MOAIFacebookAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	jstring jidentifier = self->GetJString ( lua_tostring ( state, 1 ));
	self->CallStaticVoidMethod ( self->mJava_Init, jidentifier );		
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	logEvent
    @text	Log event

 	@in 	string 	eventName
	@in		double 	valueToSum
	@opt	table 	parameters
    @out	nil
*/
int MOAIFacebookAndroid::_logEvent ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	jstring jeventName = self->GetJString ( lua_tostring ( state, 1 ));
	double valueToSum = state.GetValue < double >( 2, 0 );
    jobject jbundle;
    if ( state.IsType ( 3, LUA_TTABLE ) ) {
       jbundle = self->BundleFromLua( L, 3 );
    }
	self->CallStaticVoidMethod ( self->mJava_LogEvent, jeventName, valueToSum, jbundle );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	logPurchase
    @text	Log purchase

 	@in 	string 	eventName
	@in		double 	valueToSum
	@opt	table 	parameters
    @out	nil
*/
int MOAIFacebookAndroid::_logPurchase ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

    double amount = state.GetValue < double >( 1, 0.0f );
	jstring jcurrency = self->GetJString ( state.GetValue < cc8* >( 2, "USD" ) );

    jobject jbundle;
    if ( state.IsType ( 3, LUA_TTABLE ) ) {
       jbundle = self->BundleFromLua( L, 3 );
    }

	self->CallStaticVoidMethod ( self->mJava_LogPurchase, amount, jcurrency, jbundle );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	login
	@text	Prompt the user to login to Facebook.
				
	@opt	table	permissions			Optional set of required permissions. See Facebook documentation for a full list. Default is nil.
	@out 	nil
*/
int MOAIFacebookAndroid::_login ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	jobjectArray jpermissions = NULL;
	if ( state.IsType ( 1, LUA_TTABLE )) {
        jpermissions = self->StringArrayFromLua ( L, 1 );
	}
	if ( jpermissions == NULL ) {
		jpermissions = self->Env ()->NewObjectArray ( 0, self->Env ()->FindClass( "java/lang/String" ), 0 );
	}
	bool allowUI = state.GetValue < bool >( 2, true );
	self->CallStaticVoidMethod ( self->mJava_Login, jpermissions, allowUI );				
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	logout
	@text	Log the user out of Facebook.
				
	@out 	nil
*/
int MOAIFacebookAndroid::_logout ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	self->CallStaticVoidMethod ( self->mJava_Logout );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	postToFeed
	@text	Post a message to the logged in users' news feed.
				
	@in		string	link			The URL that the post links to. See Facebook documentation.
	@in		string	picture			The URL of an image to include in the post. See Facebook documentation.
	@in		string	name			The name of the link. See Facebook documentation.
	@in		string	caption			The caption of the link. See Facebook documentation.
	@in		string	description		The description of the link. See Facebook documentation.
	@in		string	message			The message for the post. See Facebook documentation.
	@out 	nil
*/
int MOAIFacebookAndroid::_postToFeed ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	jstring jlink			= self->GetJString ( lua_tostring ( state, 1 ));
	jstring jpicture		= self->GetJString ( lua_tostring ( state, 2 ));
	jstring jname			= self->GetJString ( lua_tostring ( state, 3 ));
	jstring jcaption		= self->GetJString ( lua_tostring ( state, 4 ));
	jstring jdescription	= self->GetJString ( lua_tostring ( state, 5 ));
	jstring jmessage		= self->GetJString ( lua_tostring ( state, 6 ));
	
	self->CallStaticVoidMethod ( self->mJava_PostToFeed, jlink, jpicture, jname, jcaption, jdescription, jmessage );	
		
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebookAndroid::_refreshPermissions ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	requestPublishPermissions
	@text	
				
	@in	table	permissions		Set of required permissions. See Facebook documentation for a full list
	@out 	nil
*/
int MOAIFacebookAndroid::_requestPublishPermissions ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	jobjectArray jpermissions = NULL;
	if ( state.IsType ( 1, LUA_TTABLE )) {
        jpermissions = self->StringArrayFromLua ( L, 1 );
	}
	if ( jpermissions == NULL ) {
		jpermissions = self->Env ()->NewObjectArray ( 0, self->Env ()->FindClass( "java/lang/String" ), 0 );
	}
	bool allowUI = state.GetValue < bool >( 2, true );
	self->CallStaticVoidMethod ( self->mJava_RequestPublishPermissions, jpermissions );				
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	requestReadPermissions
	@text	
				
	@in	table	permissions		Set of required permissions. See Facebook documentation for a full list
	@out 	nil
*/
int MOAIFacebookAndroid::_requestReadPermissions ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	jobjectArray jpermissions = NULL;
	if ( state.IsType ( 1, LUA_TTABLE )) {
        jpermissions = self->StringArrayFromLua ( L, 1 );
	}
	if ( jpermissions == NULL ) {
		jpermissions = self->Env ()->NewObjectArray ( 0, self->Env ()->FindClass( "java/lang/String" ), 0 );
	}
	self->CallStaticVoidMethod ( self->mJava_RequestReadPermissions, jpermissions );				
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFacebookAndroid::_restoreSession ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	lua_pushboolean ( state, self->CallStaticBooleanMethod ( self->mJava_RestoreSession ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	sendRequest
	@text	Send an app request to the logged in users' friends.
				
	@opt	string	message			The message for the request. See Facebook documentation. Default is nil.
	@out 	nil
*/
int MOAIFacebookAndroid::_sendRequest ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	jstring jmessage = self->GetJString ( lua_tostring ( state, 1 ));
	jobjectArray jparams = NULL;
	jobject jbundle;
    if ( state.IsType ( 2, LUA_TTABLE ) ) {
       jbundle = self->BundleFromLua( L, 2 );
    }
	self->CallStaticVoidMethod ( self->mJava_SendRequest, jmessage, jbundle );		
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	sessionValid
	@text	Determine whether or not the current Facebook session is valid.
				
	@out 	boolean	valid
*/
int MOAIFacebookAndroid::_sessionValid ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	lua_pushboolean ( state, self->CallStaticBooleanMethod ( self->mJava_SessionValid ));
	return 1;
}

// //----------------------------------------------------------------//
// int MOAIFacebookAndroid::_setListener ( lua_State* L ) {
// 	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
// 	u32 idx = state.GetValue < u32 >( 1, TOTAL_EVENTS );

// 	if ( idx < TOTAL_EVENTS ) {
// 		MOAIFacebookAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
// 	}
// 	return 0;
// }

//================================================================//
// MOAIFacebookAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIFacebookAndroid::MOAIFacebookAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
	// RTTI_SINGLE ( MOAIGlobalEventSource )
		
	this->SetClass ( "com/ziplinegames/moai/MoaiFacebook" );
	
	this->mJava_AppEventsConstants 			= this->GetClass 		( "com/facebook/AppEventsConstants" );
	// this->mJava_DeclinedPermissions			= this->GetStaticMethod ( "declinedPermissions", "()[Ljava/lang/String;" );
	this->mJava_GraphRequest				= this->GetStaticMethod ( "graphRequest", "(Ljava/lang/String;Ljava/lang/String;Landroid/os/Bundle;I)V" );
	this->mJava_HasGranted					= this->GetStaticMethod ( "hasGranted", "(Ljava/lang/String;)Z" );
	this->mJava_Init						= this->GetStaticMethod ( "init", "(Ljava/lang/String;)V" );
	this->mJava_LogEvent					= this->GetStaticMethod ( "logEvent", "(Ljava/lang/String;DLandroid/os/Bundle;)V" );
	this->mJava_LogPurchase					= this->GetStaticMethod ( "logPurchase", "(DLjava/lang/String;Landroid/os/Bundle;)V" );
	this->mJava_Login						= this->GetStaticMethod ( "login", "([Ljava/lang/String;Z)V" );
	this->mJava_Logout						= this->GetStaticMethod ( "logout", "()V" );
	this->mJava_PostToFeed					= this->GetStaticMethod ( "postToFeed", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
	// this->mJava_RefreshPermissions			= this->GetStaticMethod ( "refreshPermissions", "" );
	this->mJava_RequestPublishPermissions	= this->GetStaticMethod ( "requestPublishPermissions", "([Ljava/lang/String;)V" );
	this->mJava_RequestReadPermissions		= this->GetStaticMethod ( "requestReadPermissions", "([Ljava/lang/String;)V" );
	this->mJava_RestoreSession				= this->GetStaticMethod ( "restoreSession", "()Z" );
	this->mJava_SendRequest					= this->GetStaticMethod ( "sendRequest", "(Ljava/lang/String;Landroid/os/Bundle;)V" );
	this->mJava_SessionValid				= this->GetStaticMethod ( "sessionValid", "()Z" );
}

//----------------------------------------------------------------//
MOAIFacebookAndroid::~MOAIFacebookAndroid () {

}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIFacebookAndroid* self = &MOAIFacebookAndroid::Get ();
	
	state.SetField ( -1, "DIALOG_DID_COMPLETE",		( u32 ) DIALOG_DID_COMPLETE );
	state.SetField ( -1, "DIALOG_DID_NOT_COMPLETE",	( u32 ) DIALOG_DID_NOT_COMPLETE );
	state.SetField ( -1, "PERMISSIONS_DENIED",		( u32 ) PERMISSIONS_DENIED );
	state.SetField ( -1, "PERMISSIONS_GRANTED",		( u32 ) PERMISSIONS_GRANTED );
	state.SetField ( -1, "REQUEST_RESPONSE", 		( u32 ) REQUEST_RESPONSE );
	state.SetField ( -1, "REQUEST_RESPONSE_FAILED", ( u32 ) REQUEST_RESPONSE_FAILED );
	state.SetField ( -1, "SESSION_DID_LOGIN",		( u32 ) SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN",	( u32 ) SESSION_DID_NOT_LOGIN );
	state.SetField ( -1, "TOTAL_EVENTS",			( u32 ) TOTAL_EVENTS );

	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_ACHIEVED_LEVEL", 						"EVENT_NAME_ACHIEVED_LEVEL" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_ACTIVATED_APP", 						"EVENT_NAME_ACTIVATED_APP" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_ADDED_PAYMENT_INFO", 					"EVENT_NAME_ADDED_PAYMENT_INFO" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_ADDED_TO_CART", 						"EVENT_NAME_ADDED_TO_CART" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_ADDED_TO_WISHLIST", 					"EVENT_NAME_ADDED_TO_WISHLIST" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_COMPLETED_REGISTRATION", 				"EVENT_NAME_COMPLETED_REGISTRATION" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_COMPLETED_TUTORIAL", 					"EVENT_NAME_COMPLETED_TUTORIAL" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_INITIATED_CHECKOUT", 					"EVENT_NAME_INITIATED_CHECKOUT" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_PURCHASED", 							"EVENT_NAME_PURCHASED" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_RATED", 								"EVENT_NAME_RATED" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_SEARCHED", 							"EVENT_NAME_SEARCHED" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_SPENT_CREDITS", 						"EVENT_NAME_SPENT_CREDITS" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_UNLOCKED_ACHIEVEMENT", 				"EVENT_NAME_UNLOCKED_ACHIEVEMENT" );
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "NAME_VIEWED_CONTENT", 						"EVENT_NAME_VIEWED_CONTENT" );

	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_CONTENT_ID", 				"EVENT_PARAM_CONTENT_ID");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_CONTENT_TYPE", 				"EVENT_PARAM_CONTENT_TYPE");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_CURRENCY", 					"EVENT_PARAM_CURRENCY");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_DESCRIPTION", 				"EVENT_PARAM_DESCRIPTION");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_LEVEL", 					"EVENT_PARAM_LEVEL");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_MAX_RATING_VALUE", 			"EVENT_PARAM_MAX_RATING_VALUE");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_NUM_ITEMS", 				"EVENT_PARAM_NUM_ITEMS");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_PAYMENT_INFO_AVAILABLE", 	"EVENT_PARAM_PAYMENT_INFO_AVAILABLE");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_REGISTRATION_METHOD", 		"EVENT_PARAM_REGISTRATION_METHOD");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_SEARCH_STRING", 			"EVENT_PARAM_SEARCH_STRING");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_NAME_SUCCESS", 					"EVENT_PARAM_SUCCESS");
	
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_VALUE_NO", 						"EVENT_PARAM_VALUE_NO");
	this->RegisterStringConstant ( state, mJava_AppEventsConstants, "PARAMETER_VALUE_YES", 						"EVENT_PARAM_VALUE_YES");

	luaL_Reg regTable [] = {
    	// { "declinedPermissions",		_declinedPermissions },
		{ "graphRequest",				_graphRequest },
    	{ "hasGranted",					_hasGranted },
		{ "init",						_init },
		{ "logEvent",					_logEvent },
    	{ "logPurchase",				_logPurchase },
		{ "login",						_login },
		{ "logout",						_logout },
		{ "postToFeed",					_postToFeed },
    	{ "refreshPermissions",			_refreshPermissions },
		{ "restoreSession",				_restoreSession },
		{ "requestPublishPermissions",	_requestPublishPermissions },
		{ "requestReadPermissions",		_requestReadPermissions },
		{ "sendRequest",				_sendRequest },
		{ "sessionValid",				_sessionValid },
		// { "setListener",				_setListener },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIFacebookAndroid > },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::DialogDidNotComplete () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_NOT_COMPLETE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::DialogDidComplete () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_COMPLETE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::DialogDidComplete ( cc8* result ) {
		
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_COMPLETE, state )) {
		
		if ( result ) {
			state.Push ( result );
			state.DebugCall ( 1, 0 );
		}
		else {
			state.DebugCall ( 0, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::PermissionsDenied ( cc8* error ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( PERMISSIONS_DENIED, state )) {
		
		if ( error ) {
			
			state.Push ( error );
			state.DebugCall ( 1, 0 );
		}
		else {
			state.DebugCall ( 0, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::PermissionsGranted ( ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( PERMISSIONS_GRANTED, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::ReceivedRequestResponse ( bool error, cc8* result, int callbackIdx ) {
	MOAIFacebookAndroid* self = &MOAIFacebookAndroid::Get ();
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( callbackIdx, state )) {
		
		if ( error ) {
			state.Push(false);
			state.Push(result);
		} else {
			state.Push(true);
			json_error_t error;
	     json_t *json = json_loads ( result, 0, &error );
			if ( json ) {
				self->JsonToLua ( state, json );
				json_decref ( json );
			}
		}
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::SessionDidLogin () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_LOGIN, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::SessionDidNotLogin () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_NOT_LOGIN, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// Facebook JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUDialogDidNotComplete ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().DialogDidNotComplete ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUDialogDidComplete ( JNIEnv* env, jclass obj, jstring jresult ) {

	if ( jresult == NULL ) {

		MOAIFacebookAndroid::Get ().DialogDidComplete ();
	} else {

		JNI_GET_CSTRING ( jresult, result );

		MOAIFacebookAndroid::Get ().DialogDidComplete ( result );

		JNI_RELEASE_CSTRING ( jresult, result );	
	}
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUPermissionsDenied ( JNIEnv* env, jclass obj, jstring jerror ) {

	JNI_GET_CSTRING ( jerror, error );
	
	MOAIFacebookAndroid::Get ().PermissionsDenied ( error );
	
	JNI_RELEASE_CSTRING ( jerror, error );	
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUPermissionsGranted ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().PermissionsGranted ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUReceivedRequestResponse ( JNIEnv* env, jclass obj, jboolean jerror, jstring jresult, jint callbackIdx ) { 

	JNI_GET_CSTRING ( jresult, result );
	
	MOAIFacebookAndroid::Get ().ReceivedRequestResponse ( (bool)jerror, result, callbackIdx ); 
	
	JNI_RELEASE_CSTRING ( jresult, result );	

}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUSessionDidLogin ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().SessionDidLogin ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUSessionDidNotLogin ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().SessionDidNotLogin ();
}