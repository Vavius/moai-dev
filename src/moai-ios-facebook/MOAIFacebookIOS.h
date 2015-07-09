//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIFACEBOOKIOS_H
#define MOAIFACEBOOKIOS_H

#ifndef DISABLE_FACEBOOK

#import <Foundation/Foundation.h> 
#import <moai-core/headers.h>
#import <FacebookSDK/FacebookSDK.h>

@class MOAIFacebookLuaCallback;

//================================================================//
// MOAIFacebookIOS
//================================================================//
/**	@name	MOAIFacebookIOS
	@text	Wrapper for Facebook integration on iOS devices.
			Facebook provides social integration for sharing on
			www.facebook.com. Exposed to lua via MOAIFacebook on 
			all mobile platforms.

	@const	DIALOG_DID_COMPLETE			Event code for a successfully completed Facebook dialog.
	@const	DIALOG_DID_NOT_COMPLETE		Event code for a failed (or canceled) Facebook dialog.
	@const	SESSION_DID_LOGIN			Event code for a successfully completed Facebook login.
	@const	SESSION_DID_NOT_LOGIN		Event code for a failed (or canceled) Facebook login.
	@const	REQUEST_RESPONSE			Event code for a seccessfully completed graph request.
	@const	PERMISSIONS_DENIED
	@const	PERMISSIONS_GRANTED
	@const	TOTAL_EVENTS

*/
class MOAIFacebookIOS :
	public MOAIGlobalClass < MOAIFacebookIOS, MOAILuaObject >,
	public MOAIGlobalEventSource {
private:
		
	STLString						mAppId;
	FBFrictionlessRecipientCache* 	mFriendsCache;
	
		
	//----------------------------------------------------------------//
    static int  _declinedPermissions        ( lua_State* L );
	static int	_extendToken				( lua_State* L );
    static int	_getExpirationDate			( lua_State* L );
	static int	_getToken					( lua_State* L );
	static int	_graphRequest				( lua_State* L );
    static int  _hasGranted                 ( lua_State* L );
	static int	_init						( lua_State* L );
	static int	_logEvent					( lua_State* L );
    static int  _logPurchase                ( lua_State* L );
	static int	_login						( lua_State* L );
	static int	_logout						( lua_State* L );
	static int	_postToFeed					( lua_State* L );
    static int  _refreshPermissions         ( lua_State* L );
	static int	_restoreSession				( lua_State* L );
	static int	_requestPublishPermissions	( lua_State* L );
	static int	_requestReadPermissions		( lua_State* L );
	static int	_sendRequest				( lua_State* L );
	static int	_sessionValid				( lua_State* L );
	
public:
    
	DECL_LUA_SINGLETON ( MOAIFacebookIOS );

	STLString						mExpirationDate;
	STLString						mToken;
		
	enum {
		DIALOG_DID_COMPLETE,
		DIALOG_DID_NOT_COMPLETE,
		PERMISSIONS_DENIED,
		PERMISSIONS_GRANTED,
		REQUEST_RESPONSE,
		SESSION_DID_LOGIN,
		SESSION_DID_NOT_LOGIN,
		TOTAL_EVENTS,
	};
	
    bool    ActiveSessionHasPermissions ( NSArray* permissions );
    		MOAIFacebookIOS			();
			~MOAIFacebookIOS		();
	void	DialogDidNotComplete	();
	void	DialogDidComplete		();
	void	DialogDidComplete		( NSURL* result );
	void	DialogResult			( bool success, NSURL* result, int callbackRef );
	void	HandleOpenURL			( NSURL* url, NSString* sourceApplication );
    void    Logout                  ();
	void	PermissionsDenied		( NSString* error );
	void	PermissionsGranted		();
	void	RegisterLuaClass		( MOAILuaState& state );
	void	ReceivedRequestResponse	( NSError* error, id result, u32 callbackIdx );
	void	SessionDidLogin			();
	void	SessionDidNotLogin		();
};

//================================================================//
// MOAIFacebookLuaCallback
//================================================================//
//
// This class is for use with async objective-C block callbacks.
// It should be retained by block and will ensure that Lua callback ref
// will have the same lifetime as enclosing obj-C block.
//
@interface MOAIFacebookLuaCallback : NSObject {
@private
	int mRef;
}

//----------------------------------------------------------------//
-( id )	initWithRef :( int )ref;
-( int ) ref;
@end

#endif  //DISABLE_FACEBOOK

#endif // MOAIFACEBOOK_H
