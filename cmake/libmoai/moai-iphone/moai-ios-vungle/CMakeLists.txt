cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-vungle )

#native
set(VUNGLE_ROOT ${MOAI_ROOT}/3rdparty/vungle-3010/VungleSDK.embeddedframework/VungleSDK.framework)
set(VUNGLE_LIB ${VUNGLE_ROOT}/VungleSDK)
add_library(VungleSDK STATIC IMPORTED)
set_property(TARGET VungleSDK PROPERTY IMPORTED_LOCATION ${VUNGLE_LIB})
set_property(TARGET VungleSDK PROPERTY FRAMEWORK 1)

install(DIRECTORY ${VUNGLE_ROOT} DESTINATION lib)

#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-vungle")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-vungle STATIC  ${SRC})

target_include_directories(moai-ios-vungle PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${VUNGLE_ROOT}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/lib/VungleSDK.framework>"
	)

target_link_libraries( moai-ios-vungle moai-sim moai-util moai-core VungleSDK)

set_target_properties( moai-ios-vungle PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-vungle EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-vungle/host.h DESTINATION include/moai-ios-vungle)


