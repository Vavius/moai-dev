//
//  AdToAppSDK.h
//
//  Copyright (c) 2015 AdToApp. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AdToAppSDK : NSObject

+(void)startWithAppId:(NSString*)appId;

+(BOOL)showInterstitial;
+(BOOL)hasInterstitial;

@end
