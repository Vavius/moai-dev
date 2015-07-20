// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIIMAGELOADTASK_H
#define MOAIIMAGELOADTASK_H

#include <moai-util/MOAITask.h>

class MOAIDataBuffer;
class MOAIImage;

//================================================================//
// MOAIImageLoadTask
//================================================================//
class MOAIImageLoadTask : 
	public MOAITask {
private:

	STLString								mFilename;
	MOAILuaSharedPtr < MOAIDataBuffer >		mData;
	MOAILuaSharedPtr < MOAIImage >			mImage;
	MOAILuaMemberRef						mOnFinish;
	u32										mTransform;

	//----------------------------------------------------------------//
	void		Execute				();
	void		Publish				();

public:

	DECL_LUA_FACTORY ( MOAIImageLoadTask )

	//----------------------------------------------------------------//
	void		Init				( cc8* filename, MOAIImage& target, u32 transform );
	void		Init				( MOAIDataBuffer& data, MOAIImage& target, u32 transform );
				MOAIImageLoadTask	();
				~MOAIImageLoadTask	();
	void		RegisterLuaClass	( MOAILuaState& state );
	void		RegisterLuaFuncs	( MOAILuaState& state );
	void		SetCallback			( lua_State* L, int idx );
};

#endif
