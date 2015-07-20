// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIDataBuffer.h>
#include <moai-util/MOAIImageLoadTask.h>
#include <moai-sim/MOAIImage.h>

//================================================================//
// MOAIImageLoadTask
//================================================================//

//----------------------------------------------------------------//
void MOAIImageLoadTask::Execute () {

	if ( this->mFilename.size() ) {
		this->mImage->Load ( this->mFilename, this->mTransform );
	}
	else if ( this->mData ) {

		void* bytes = 0;
		size_t size = 0;
		ZLByteStream stream;

		this->mData->Lock ( &bytes, &size );
		
		stream.SetBuffer ( bytes, size );
		stream.SetLength ( size );

		this->mImage->Load ( stream, this->mTransform );

		this->mData->Unlock();
	}
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::Init ( cc8* filename, MOAIImage& target, u32 transform ) {

	this->mFilename = filename;
	this->mTransform = transform;
	this->mImage.Set ( *this, &target );
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::Init ( MOAIDataBuffer& data, MOAIImage& target, u32 transform ) {

	this->mTransform = transform;
	this->mData.Set ( *this, &data);
	this->mImage.Set ( *this, &target );
}

//----------------------------------------------------------------//
MOAIImageLoadTask::MOAIImageLoadTask () {
	
	RTTI_SINGLE ( MOAITask )
}

//----------------------------------------------------------------//
MOAIImageLoadTask::~MOAIImageLoadTask () {

	this->mData.Set ( *this, 0 );
	this->mImage.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::Publish () {

	if ( this->mOnFinish ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnFinish.PushRef ( state )) {
			this->mImage->PushLuaUserdata ( state );
			state.DebugCall ( 1, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITask::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAITask::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::SetCallback ( lua_State* L, int idx ) {

	MOAILuaState state ( L );
	this->mOnFinish.SetRef ( *this, state, idx );
}

