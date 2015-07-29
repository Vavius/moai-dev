// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifndef _WIN32

#include <zl-util/ZLUnique.h>

#if __APPLE__
	#include <CoreFoundation/CoreFoundation.h>
#else
	#include <kashmir/devrand.h>
	#include <kashmir/uuid.h>
#endif

//================================================================//
// ZLUnique
//================================================================//

//----------------------------------------------------------------//
STLString ZLUnique::GetGUID () {
	
	#if __APPLE__
		CFUUIDRef uuid = CFUUIDCreate( NULL );
		CFStringRef guid = CFUUIDCreateString ( NULL, uuid );
		CFRelease ( uuid );
		const char * output = CFStringGetCStringPtr ( guid, kCFStringEncodingUTF8 );
		if (output == NULL) {
			output = CFStringGetCStringPtr ( guid, kCFStringEncodingMacRoman );
		}
		CFRelease ( guid );
		return STLString ( output );
	#else
		kashmir::system::DevRand devrandom;
		std::stringstream buffer;
		kashmir::uuid_t uuid;
		devrandom >> uuid;
		buffer << uuid;
		return buffer.str ();
	#endif
}

#endif
