// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-chartboost/host.h>
#include <moai-ios-chartboost/MOAIChartboostIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosChartBoostAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosChartBoostAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosChartBoostContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIChartboostIOS );
}
