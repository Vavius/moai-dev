//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-ios-facebook/MOAIFacebookIOS.h>
#import <moai-ios/NSDictionary+MOAILib.h>
#import <moai-ios/NSString+MOAILib.h>


// Facebook SDK starting from 3.14 not showing native login dialog be default
// The simpliest workaround is to call a private method on FBSession...
@interface FBSession ()
+ (BOOL)openActiveSessionWithPermissions:(NSArray *)permissions
                            allowLoginUI:(BOOL)allowLoginUI
                      allowSystemAccount:(BOOL)allowSystemAccount
                                  isRead:(BOOL)isRead
                         defaultAudience:(FBSessionDefaultAudience)defaultAudience
                       completionHandler:(FBSessionStateHandler)handler;
@end


//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	TODO
 */
int MOAIFacebookIOS::_declinedPermissions ( lua_State* L ) {
    
    MOAILuaState state ( L );
    
    NSArray *permissions = [ FBSession.activeSession declinedPermissions ];
    [ permissions toLua: L ];
    
    return 1;
}

//----------------------------------------------------------------//
/**	@name	extendToken
 @text	Extends the life of an active token. Should be called on app resume/start.
 
 @in		nil
 @out	nil
 */
int MOAIFacebookIOS::_extendToken ( lua_State* L ) {
	
	printf ( "extending token....\n" );
	// [ MOAIFacebookIOS::Get ().mFacebook extendAccessTokenIfNeeded ];
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getExpirationDate
 @text	Retrieve the Facebook login token expiration date.
 
 @in		nil
 @out	string	token expiration date
 */
int MOAIFacebookIOS::_getExpirationDate ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSDateFormatter *formatter = [[[ NSDateFormatter alloc ] init ] autorelease];
	[ formatter setDateFormat:@"dd-MM-yyyy HH:mm:ss" ];

	NSString *dateString = [ formatter stringFromDate:FBSession.activeSession.accessTokenData.expirationDate ];
	
	if ( dateString ) {

		lua_pushstring ( L, [ dateString UTF8String ] );
	} else {
		
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getToken
 @text	Retrieve the Facebook login token.
 
 @in		nil
 @out	string	token
 */
int MOAIFacebookIOS::_getToken ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* token = FBSession.activeSession.accessTokenData.accessToken;

	if ( token ) {
		lua_pushstring ( L, [ token UTF8String ]);
	} else {
		lua_pushnil ( L );
	}

	return 1;
}

//----------------------------------------------------------------//
/**	@name	graphRequest
 @text	Make a request on Facebook's Graph API
 
 @in	string		method, default is GET
 @in	string		graph path, required
 @opt	table		parameters, default is nil
 @opt	number		event index to raise when request is complete, default is REQUEST_RESPONSE
 @out	nil
 */
int MOAIFacebookIOS::_graphRequest ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* method = state.GetValue < cc8* >( 1, "GET" );
    cc8* path   = state.GetValue < cc8* >( 2, "" );
	
	NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
	if ( state.IsType ( 3, LUA_TTABLE )) {
		[ paramsDict initWithLua:state stackIndex:3 ];
	}
	
	u32 eventIdx = state.GetValue < u32 >( 4, REQUEST_RESPONSE );
	
	NSString* nsMethod = [[[ NSString alloc ] initWithUTF8String:method ] autorelease ];
	NSString* nsPath = [[[ NSString alloc ] initWithUTF8String:path ] autorelease ];
	
	[ FBRequestConnection startWithGraphPath:nsPath
		parameters:paramsDict
		HTTPMethod:nsMethod
		completionHandler:^( FBRequestConnection* connection, id result, NSError* error ) {
			
			UNUSED ( connection );
			
			MOAIFacebookIOS::Get ().ReceivedRequestResponse ( error, result, eventIdx );
		}
	];
	
	return 0;
}

//----------------------------------------------------------------//
/**	TODO
 */
int MOAIFacebookIOS::_hasGranted ( lua_State* L ) {
    
    MOAILuaState state ( L );
    
    cc8* permission = state.GetValue < cc8* >( 1, 0 );
    
    if ( permission ) {
        
        BOOL result = [ FBSession.activeSession hasGranted: [ NSString stringWithUTF8String:permission ]];
        state.Push (( bool )result );
        return 1;
    }
    
    return 0;
}

//----------------------------------------------------------------//
/**	@name	init
 @text	Initialize Facebook.
 
 @in		string	appId			Available in Facebook developer settings.
 @out 	nil
 */
int MOAIFacebookIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	[ FBSettings setDefaultAppID:[ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )]];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	logEvent
 @text	Log event to Facebook.
 
 @in	string	eventName		Name of the event
 @opt	number	valueToSum		Amount to be aggregated into all events of this eventName, and App Insights will report
								the cumulative and average value of this amount.
 @opt	table	parameters		Arbitrary parameter dictionary of characteristics. Keys are strings, values are strings or numbers
 @out 	nil
 */
int MOAIFacebookIOS::_logEvent ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	NSString* eventName = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	
	if ( eventName == NULL ) {
		return 0;
	}
	
	double valueToSum = state.GetValue < double >(2, 0);
	
	NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
	if ( state.IsType( 3, LUA_TTABLE )) {
		[ paramsDict initWithLua:state stackIndex:3 ];
	}
	
	[ FBAppEvents logEvent:eventName valueToSum:valueToSum parameters:paramsDict ];
	[ paramsDict release ];
	[ eventName release ];
	
	return 0;
}

int MOAIFacebookIOS::_logPurchase ( lua_State *L ) {
    
    MOAILuaState state ( L );
    
    double amount = state.GetValue < double >( 1, 0.0f );
    cc8* currency = state.GetValue < cc8* >( 2, "USD" );
    
    NSMutableDictionary* params = [[ NSMutableDictionary alloc ] init ];
    if ( state.IsType( 3, LUA_TTABLE )) {
        [ params initWithLua:state stackIndex:3 ];
    }
    
    [ FBAppEvents logPurchase:amount currency:[ NSString stringWithUTF8String:currency] parameters:params ];
    [ params release ];
    
    return 0;
}


//----------------------------------------------------------------//
/**	@name	login
 @text	Prompt the user to login to Facebook.
 
 @opt	table	permissions		Optional set of required permissions. See Facebook documentation for a full list. Default is nil.
 @opt	bool	allowLoginUI	Whether to show user dialog. Default is True
 @out 	nil
 */
int MOAIFacebookIOS::_login ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
	NSArray* permissions = nil;
    
	if ( state.IsType ( 1, LUA_TTABLE )) {	
		[ paramsDict initWithLua:state stackIndex:1 ];
        permissions = [ paramsDict allValues ];
	}
    else {
        permissions = [ NSArray arrayWithObject:@"public_profile" ];
    }
	
	bool allowLoginUI = state.GetValue < bool >( 2, true );
    
    BOOL result = [ FBSession openActiveSessionWithPermissions:permissions
        allowLoginUI:allowLoginUI
        allowSystemAccount:NO
        isRead:YES
        defaultAudience:FBSessionDefaultAudienceNone
        completionHandler:^( FBSession *session, FBSessionState sessionState, NSError *error ) {
            UNUSED ( session );

            if ( error ) {
                cc8* message = [[ error description ] UTF8String ];
                MOAIPrint ( message );
                MOAIFacebookIOS::Get ().SessionDidNotLogin ();
                return;
            }

            switch ( sessionState ) {
                case FBSessionStateOpen:
                case FBSessionStateOpenTokenExtended: {
					
						[ FBSession setActiveSession:session ];
						MOAIFacebookIOS::Get ().SessionDidLogin ();
						FBFrictionlessRecipientCache* cache = MOAIFacebookIOS::Get ().mFriendsCache;
						[ cache prefetchAndCacheForSession:nil ];
						break;
                    }

                case FBSessionStateClosed:
                case FBSessionStateClosedLoginFailed:
					
					[ FBSession.activeSession closeAndClearTokenInformation ];
                    MOAIFacebookIOS::Get ().SessionDidNotLogin ();
                    break;

                default:
                    break;
            }
        }
   ];
    
//	BOOL result = [ FBSession openActiveSessionWithReadPermissions:permissions
//		allowLoginUI:allowLoginUI
//		completionHandler:^( FBSession *session, FBSessionState sessionState, NSError *error ) {
//			UNUSED ( session );
//			
//            if ( error ) {
//                cc8* message = [[ error description ] UTF8String ];
//                MOAIPrint ( message );
//				MOAIFacebookIOS::Get ().SessionDidNotLogin ();
//                return;
//            }
//            
//			switch ( sessionState ) {
//				case FBSessionStateOpen:
//				case FBSessionStateOpenTokenExtended:
//					
//					MOAIFacebookIOS::Get ().SessionDidLogin ();
//					break;
//					
//				case FBSessionStateClosed:
//				case FBSessionStateClosedLoginFailed:
//					
////					[ FBSession.activeSession closeAndClearTokenInformation ];
//					MOAIFacebookIOS::Get ().SessionDidNotLogin ();
//					break;
//					
//				default:
//					break;
//			}
//		}
//	];
	
	// initiate callbacks when openActiveSessionWithReadPermissions: executed synchronously
	if ( !allowLoginUI ) {
		
		if ( result ) {
			MOAIFacebookIOS::Get ().SessionDidLogin ();
		}
		else {
			MOAIFacebookIOS::Get ().SessionDidNotLogin ();
		}
	}
	
	[paramsDict release];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	logout
 @text	Log the user out of Facebook.
 
 @in		nil
 @out 	nil
 */
int MOAIFacebookIOS::_logout ( lua_State* L ) {
	
	MOAILuaState state ( L );
    MOAIFacebookIOS::Get ().Logout ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	postToFeed
 @text	Post a message to the logged in users' news feed.
 
 @in		string	link			The URL that the post links to. See Facebook documentation.
 @in		string	picture			The URL of an image to include in the post. See Facebook documentation.
 @in		string	name			The name of the link. See Facebook documentation.
 @in		string	caption			The caption of the link. See Facebook documentation.
 @in		string	description		The description of the link. See Facebook documentation.
 @in		string	message			The message for the post. See Facebook documentation.
 @out 	nil
 */
int MOAIFacebookIOS::_postToFeed ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* link				= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )];
	NSString* picture			= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 2, "" )];
	NSString* name				= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 3, "" )];
	NSString* caption			= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 4, "" )];
	NSString* description		= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 5, "" )];
	
	NSDictionary *params = @{
		@"name":name,
		@"caption":caption,
		@"description":description,
		@"picture":picture,
		@"link":link,
	};

    // Invoke the dialog
	[ FBWebDialogs presentFeedDialogModallyWithSession:nil
		parameters:params
		handler:^( FBWebDialogResult result, NSURL* resultURL, NSError* error ) {
			UNUSED ( resultURL );
			
			if (error) {
				MOAIFacebookIOS::Get ().DialogDidNotComplete ();
			}
			else {
				if ( result == FBWebDialogResultDialogNotCompleted ) {
					MOAIFacebookIOS::Get ().DialogDidNotComplete ();
				}
				else {
					MOAIFacebookIOS::Get ().DialogDidComplete ( resultURL );
				}
			}
		}
	];
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFacebookIOS::_restoreSession ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	BOOL result = [ FBSession openActiveSessionWithAllowLoginUI:NO ];
	lua_pushboolean ( L, result );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFacebookIOS::_requestPublishPermissions( lua_State *L ) {
	
	MOAILuaState state ( L );
	
    if ( ![ FBSession.activeSession isOpen ]) {
        MOAIPrint ( "MOAIFacebookIOS: session is not initialized\n" );
        return 0;
    }
    
	NSMutableDictionary* permissions = nil;
	
	if ( state.IsType ( 1, LUA_TTABLE )) {
		
		permissions = [[[ NSMutableDictionary alloc ] init ] autorelease ];
		[ permissions initWithLua:state stackIndex:1 ];
	}
	
	if ( permissions ) {
        NSArray* requested = [ permissions allValues ];
		[ FBSession.activeSession requestNewPublishPermissions:requested
		   defaultAudience:FBSessionDefaultAudienceEveryone
		 completionHandler:^(FBSession *session, NSError *error) {
			 
			 if ( !error && MOAIFacebookIOS::Get ().ActiveSessionHasPermissions ( requested )) {
				 MOAIFacebookIOS::Get ().PermissionsGranted ();
			 }
			 else {
				 MOAIFacebookIOS::Get ().PermissionsDenied ( [ error localizedDescription ] );
			 }
		 }];
	}
    
    return 0;
}


//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFacebookIOS::_requestReadPermissions( lua_State *L ) {
	
	MOAILuaState state ( L );
	
    if ( ![ FBSession.activeSession isOpen ]) {
        MOAIPrint ( "MOAIFacebookIOS: session is not initialized\n" );
        return 0;
    }

    NSMutableDictionary* permissions = nil;
    
	if ( state.IsType( 1, LUA_TTABLE )) {
		
		permissions = [[[ NSMutableDictionary alloc ] init ] autorelease ];
		[ permissions initWithLua:state stackIndex:1 ];
	}
	
	if ( permissions ) {
        NSArray* requested = [ permissions allValues ];
		[ FBSession.activeSession requestNewReadPermissions: requested
		  completionHandler:^(FBSession *session, NSError *error) {
				
			  if ( !error && MOAIFacebookIOS::Get ().ActiveSessionHasPermissions ( requested )) {
				  MOAIFacebookIOS::Get ().PermissionsGranted();
			  }
			  else {
				  MOAIFacebookIOS::Get ().PermissionsDenied( [ error localizedDescription ] );
			  }
		  }];
	}
    
    return 0;
}


//----------------------------------------------------------------//
/**	@name	sendRequest
	@text	Send an app request to the logged in users' friends.
	
	@opt	string	message			The message for the request. See Facebook documentation. Default is nil.
	@opt	table	params			Optional parameters
	@out 	nil
*/
int MOAIFacebookIOS::_sendRequest ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* message = [ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )];
	
	NSMutableDictionary* params = nil;
	
	if ( state.IsType ( 2, LUA_TTABLE )) {
		
		params = [[[ NSMutableDictionary alloc ] init ] autorelease ];
		[ params initWithLua:state stackIndex:2 ];
	}
	
    FBFrictionlessRecipientCache* cache = MOAIFacebookIOS::Get ().mFriendsCache;
    [ FBWebDialogs presentRequestsDialogModallyWithSession:nil
		message:message
		title:nil
		parameters:params
		handler:^( FBWebDialogResult result, NSURL* resultURL, NSError* error ) {
			UNUSED ( resultURL );
		
			if ( error ) {
				MOAIFacebookIOS::Get ().DialogDidNotComplete ();
			}
			else {
				if (result == FBWebDialogResultDialogCompleted) {
					MOAIFacebookIOS::Get ().DialogDidComplete ( resultURL );
				} else {
					MOAIFacebookIOS::Get ().DialogDidNotComplete ();
				}
			}
		}
		friendCache:cache
	];
	
	return 0;
}


//----------------------------------------------------------------//
/**	@name	sessionValid
 @text	Determine whether or not the current Facebook session is valid.
 
 @in		nil
 @out 	boolean	valid
 */
int MOAIFacebookIOS::_sessionValid ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	lua_pushboolean ( state, [ FBSession.activeSession isOpen ]);
	
	return 1;
}

	
//================================================================//
// MOAIFacebookIOS
//================================================================//

//----------------------------------------------------------------//
bool MOAIFacebookIOS::ActiveSessionHasPermissions ( NSArray* permissions ) {
    
    bool result = true;
    for ( NSString* permission in permissions ) {
        
        result = result && [ FBSession.activeSession hasGranted:permission ];
    }
    return result;
}

//----------------------------------------------------------------//
MOAIFacebookIOS::MOAIFacebookIOS () {
    
	RTTI_SINGLE ( MOAILuaObject )
	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	mFriendsCache = [[ FBFrictionlessRecipientCache alloc ] init ];
}

//----------------------------------------------------------------//
MOAIFacebookIOS::~MOAIFacebookIOS () {
    
//	[ FBSession.activeSession close ];
    [ mFriendsCache release ];
    mFriendsCache = nil;
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::RegisterLuaClass ( MOAILuaState& state ) {
    
	state.SetField ( -1, "DIALOG_DID_COMPLETE", 	( u32 )DIALOG_DID_COMPLETE );
	state.SetField ( -1, "DIALOG_DID_NOT_COMPLETE",	( u32 )DIALOG_DID_NOT_COMPLETE );
	state.SetField ( -1, "PERMISSIONS_DENIED",		( u32 )PERMISSIONS_DENIED );
	state.SetField ( -1, "PERMISSIONS_GRANTED",		( u32 )PERMISSIONS_GRANTED );
	state.SetField ( -1, "REQUEST_RESPONSE", 		( u32 )REQUEST_RESPONSE );
	state.SetField ( -1, "SESSION_DID_LOGIN", 		( u32 )SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN", 	( u32 )SESSION_DID_NOT_LOGIN );
	state.SetField ( -1, "TOTAL_EVENTS",			( u32 )TOTAL_EVENTS );
	
	state.SetField ( -1, "NAME_ACHIEVED_LEVEL",			[ FBAppEventNameAchievedLevel UTF8String ] );
	state.SetField ( -1, "NAME_ACTIVATED_APP",			[ FBAppEventNameActivatedApp UTF8String ] );
	state.SetField ( -1, "NAME_ADDED_PAYMENT_INFO",		[ FBAppEventNameAddedPaymentInfo UTF8String ] );
	state.SetField ( -1, "NAME_ADDED_TO_CART",			[ FBAppEventNameAddedToCart UTF8String ] );
	state.SetField ( -1, "NAME_ADDED_TO_WISHLIST",		[ FBAppEventNameAddedToWishlist UTF8String ] );
	state.SetField ( -1, "NAME_COMPLETED_REGISTRATION", [ FBAppEventNameCompletedRegistration UTF8String ] );
	state.SetField ( -1, "NAME_COMPLETED_TUTORIAL",		[ FBAppEventNameCompletedTutorial UTF8String ] );
	state.SetField ( -1, "NAME_INITIATED_CHECKOUT",		[ FBAppEventNameInitiatedCheckout UTF8String ] );
	state.SetField ( -1, "NAME_PURCHASED",				[ FBAppEventNamePurchased UTF8String ] );
	state.SetField ( -1, "NAME_RATED",					[ FBAppEventNameRated UTF8String ] );
	state.SetField ( -1, "NAME_SEARCHED",				[ FBAppEventNameSearched UTF8String ] );
	state.SetField ( -1, "NAME_SPENT_CREDITS",			[ FBAppEventNameSpentCredits UTF8String ] );
	state.SetField ( -1, "NAME_UNLOCKED_ACHIEVEMENT",	[ FBAppEventNameUnlockedAchievement UTF8String ] );
	state.SetField ( -1, "NAME_VIEWED_CONTENT",			[ FBAppEventNameViewedContent UTF8String ] );
    
	state.SetField ( -1, "PARAMETER_NAME_CONTENT_ID",				[ FBAppEventParameterNameContentID UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_CONTENT_TYPE",				[ FBAppEventParameterNameContentType UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_CURRENCY",					[ FBAppEventParameterNameCurrency UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_DESCRIPTION",				[ FBAppEventParameterNameDescription UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_LEVEL",					[ FBAppEventParameterNameLevel UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_MAX_RATING_VALUE",			[ FBAppEventParameterNameMaxRatingValue UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_NUM_ITEMS",				[ FBAppEventParameterNameNumItems UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_PAYMENT_INFO_AVAILABLE",	[ FBAppEventParameterNamePaymentInfoAvailable UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_REGISTRATION_METHOD",		[ FBAppEventParameterNameRegistrationMethod UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_SEARCH_STRING",			[ FBAppEventParameterNameSearchString UTF8String ] );
	state.SetField ( -1, "PARAMETER_NAME_SUCCESS",					[ FBAppEventParameterNameSuccess UTF8String ] );
	
	state.SetField ( -1, "PARAMETER_VALUE_NO",		[ FBAppEventParameterValueNo UTF8String ] );
	state.SetField ( -1, "PARAMETER_VALUE_YES",		[ FBAppEventParameterValueYes UTF8String ] );
	
	luaL_Reg regTable[] = {
        { "declinedPermissions",        _declinedPermissions },
		{ "extendToken",				_extendToken },
		{ "getExpirationDate",			_getExpirationDate },
		{ "getToken",					_getToken },
		{ "graphRequest",				_graphRequest },
        { "hasGranted",                 _hasGranted },
		{ "init",						_init },
		{ "logEvent",					_logEvent },
        { "logPurchase",                _logPurchase },
		{ "login",						_login },
		{ "logout",						_logout },
		{ "postToFeed",					_postToFeed },
		{ "restoreSession",				_restoreSession },
		{ "requestPublishPermissions",	_requestPublishPermissions },
		{ "requestReadPermissions",		_requestReadPermissions },
		{ "sendRequest",				_sendRequest },
		{ "sessionValid",				_sessionValid },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIFacebookIOS > },
		{ NULL, NULL }	
	};
    
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::DialogDidNotComplete ( ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_NOT_COMPLETE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::DialogDidComplete ( ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_COMPLETE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::DialogDidComplete ( NSURL* result ) {
		
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_COMPLETE, state )) {
		
		if ( result ) {
			[[ result absoluteString ] toLua:state ];
			state.DebugCall ( 1, 0 );
		}
		else {
			state.DebugCall ( 0, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::HandleOpenURL ( NSURL* url, NSString* sourceApplication ) {
	
	[ FBAppCall handleOpenURL:url sourceApplication:sourceApplication ];
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::Logout () {
    
    [FBSession.activeSession closeAndClearTokenInformation];
    [FBSession renewSystemCredentials:^(ACAccountCredentialRenewResult result, NSError *error) {
        NSLog(@"%@", error);
    }];
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::PermissionsDenied( NSString *error ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( PERMISSIONS_DENIED, state )) {
		
		if ( error ) {
			
			[ error toLua:state ];
			state.DebugCall ( 1, 0 );
		}
		else {
			state.DebugCall ( 0, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::PermissionsGranted() {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( PERMISSIONS_GRANTED, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::ReceivedRequestResponse ( NSError* error, id result, u32 callbackIdx ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( callbackIdx, state )) {
		
		if ( error ) {
			
			state.Push(false);
			[ [ error localizedDescription ] toLua:state ];
		}
		else {
		
			state.Push(true);
			[ result toLua:state ];
		}
		
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::SessionDidLogin () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_LOGIN, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::SessionDidNotLogin () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_NOT_LOGIN, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

