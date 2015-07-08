//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIADTOAPPIOS_H
#define MOAIADTOAPPIOS_H

#ifndef DISABLE_ADTOAPP

#include <moai-core/headers.h>

//================================================================//
// MOAIAdColonyIOS
//================================================================//
class MOAIAdToAppIOS :
public MOAIGlobalClass < MOAIAdToAppIOS, MOAILuaObject >,
public MOAIGlobalEventSource {
private:
	
	//----------------------------------------------------------------//
	static int	_init					( lua_State* L );
	static int	_showInterstitial		( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIAdToAppIOS );
	
            MOAIAdToAppIOS				();
			~MOAIAdToAppIOS			();
	
	void	RegisterLuaClass			( MOAILuaState& state );
};

#endif // MOAIADTOAPP_H

#endif // DISABLE_ADTOAPP