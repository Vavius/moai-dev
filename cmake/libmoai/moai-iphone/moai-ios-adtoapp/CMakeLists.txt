cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-adtoapp )



set(ADTOAPP_ROOT ${MOAI_ROOT}/3rdparty/AdToAppSDK-iOS_2)
set(ADTOAPP_LIB ${ADTOAPP_ROOT}/libAdToApp)
add_library(AdToApp SHARED IMPORTED)
set_property(TARGET AdToApp PROPERTY IMPORTED_LOCATION ${ADTOAPP_LIB})
set_property(TARGET AdToApp PROPERTY FRAMEWORK 1)

install(DIRECTORY "${ADTOAPP_ROOT}" DESTINATION lib)


add_library(moai-ios-adtoapp STATIC
		"${MOAI_ROOT}/src/moai-ios-adtoapp/MOAIAdToAppIOS.h"
		"${MOAI_ROOT}/src/moai-ios-adtoapp/MOAIAdToAppIOS.mm"
		"${MOAI_ROOT}/src/moai-ios-adtoapp/host.mm"
		)
target_include_directories(moai-ios-adtoapp PUBLIC
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${ADTOAPP_ROOT}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries(moai-ios-adtoapp AdToapp moai-sim moai-util moai-core)
set_target_properties( moai-ios-adtoapp PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-adtoapp EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-adtoapp/host.h DESTINATION include/moai-ios-adtoapp)
