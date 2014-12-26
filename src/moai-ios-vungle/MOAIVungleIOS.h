//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAIVUNGLEIOS_H
#define	MOAIVUNGLEIOS_H

#ifndef DISABLE_VUNGLE

#include <moai-core/headers.h>

@class MoaiVungleDelegate;

//================================================================//
// MOAIVungleIOS
//================================================================//
class MOAIVungleIOS :
	public MOAIGlobalClass < MOAIVungleIOS, MOAILuaObject > {
private:

	MoaiVungleDelegate*	mDelegate;
		
	//----------------------------------------------------------------//
	static int  _adIsAvailable		( lua_State* L );
	static int	_playModalAd		( lua_State* L );
	static int	_playIncentivizedAd	( lua_State* L );
	static int	_init 				( lua_State* L );
	static int	_setListener		( lua_State* L );
	
public:
		
	enum {
        MOVIE_WILL_APPEAR,
		MOVIE_PLAYED,
        MOVIE_CACHED,
        PRODUCT_SHEET_CLOSED,
		TOTAL
	};
		
	MOAILuaStrongRef	mListeners [ TOTAL ];

	DECL_LUA_SINGLETON ( MOAIVungleIOS );

	
					MOAIVungleIOS				();
					~MOAIVungleIOS				();
    void            NotifyMovieCached           ();
    void			NotifyMoviePlayed			( bool willShowProductSheet, bool playedFull, float playTime, bool didDownload );
    void			NotifyMovieWillAppear		();
    void            NotifyProductSheetClosed    ();
	void			RegisterLuaClass			( MOAILuaState& state );
};

#endif  //DISABLE_VUNGLE

#endif  //MOAIVUNGLEIOS_H
