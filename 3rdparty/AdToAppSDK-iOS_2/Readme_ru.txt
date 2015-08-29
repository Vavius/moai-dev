1. Добавьте в ваш проект библиотеку libAdToApp.a., а так же системные библиотеки: CoreMotion, SystemConfiguration, StoreKit, MessageUI, EventKitUI, EventKit, CoreTelephony, CoreMedia, AudioToolbox, MediaPlayer, Social, WebKit, AVFoundation, CoreLocation, AdSupport, libz, libsqlite3.

2. Зайдите в настройки проекта и добавьте ключ -ObjC в Other Linker Flags.

3. Чтобы инициализировать SDK, используйте следующий код при запуске вашего приложения  (Ключ appId взять из профиля приложения на сайте http://adtoapp.com ) :
	#import "AdToAppSDK.h"
	...
    [AdToAppSDK startWithAppId:@"f3d710e0-6ea1-408c-accd-2925371ac79a:00362439-1050-4b2f-81f6-22bf7cf4e8e1"];

	Например, вы можете это сделать в функции application:didFinishLaunchingWithOptions: :

#import "AdToAppSDK.h"
#import "ATALog.h"

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    /*
    ataloglevel = 	ATALogFatal |
                    ATALogError |
                    ATALogWarn |
                    ATALogDebug |
                    ATALogVerbose;
    */

    [AdToAppSDK startWithAppId:@"f3d710e0-6ea1-408c-accd-2925371ac79a:00362439-1050-4b2f-81f6-22bf7cf4e8e1"];

	...

    return YES;
}

4. Добавьте в проект ресурсные бандлы StartApp.bundle и папку с ресурсами VungleResources.

5. Перейдите в настройки проекта -> секция Info (файл Info.plist) и добавьте ключ AppLovinSdkKey со значением M_tom-o_IROTf2mZSF4GNGGleuARfi81i0qT-bTH2sqiuaGLrKcCbtyEm6gQqpSWpG_otS4vIvbBTtXJC8Wo0S

Для показа рекламы используйте вызов из главного потока: [AdToAppSDK showInterstitial].
