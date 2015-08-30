option ( MOAI_NSURL "Enable MOAIHttpTaskNSURL" TRUE )
 set ( CMAKE_OSX_SYSROOT iphoneos )
  set ( CMAKE_OSX_ARCHITECTURES "armv7;i386" )
  set ( CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator" )
  set ( XCODE_ATTRIBUTE_SDKROOT iphoneos )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99  -DHAVE_MEMMOVE " )
  #default in ios7 is libc++ but some of our ad support libs are libstdc++ so force the lot to build as libstdc++
  #set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++" )

    # message ( "========================" )
    # message ( ${CMAKE_C_FLAGS_RELEASE} )
    # message ( "========================" )
    # message ( ${CMAKE_CXX_FLAGS_RELEASE} )
    # message ( "========================" )
    # message ( "========================" )

    # message ( ${CMAKE_C_FLAGS_DEBUG} )
    # message ( "========================" )
    # message ( ${CMAKE_CXX_FLAGS_DEBUG} )
    # message ( "========================" )
    # message ( "========================" )

    # message ( ${CMAKE_C_FLAGS_MINSIZEREL} )
    # message ( "========================" )
    # message ( ${CMAKE_CXX_FLAGS_MINSIZEREL} )
    # message ( "========================" )
    # message ( "========================" )

    # message ( ${CMAKE_C_FLAGS_RELWITHDEBINFO} )
    # message ( "========================" )
    # message ( ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} )
    # message ( "========================" )
    # message ( "========================" )

  # O4 doesn't work in cmake xcode generator; 
  # LTO leads to some linking problems + massive increase in compilation time
  # while not giving much performance (at least in Miner)

  # set ( CMAKE_C_FLAGS_RELEASE "-O4 -flto -DNDEBUG" )
  # set ( CMAKE_CXX_FLAGS_RELEASE "-O4 -flto -DNDEBUG" )
  
