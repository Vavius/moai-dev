cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-crashlytics )

# crashlytics framework
set(CRASHLYTICS_ROOT ${MOAI_ROOT}/3rdparty/Crashlytics/Crashlytics.framework)
set(CRASHLYTICS_LIB ${CRASHLYTICS_ROOT}/Crashlytics)
add_library(Crashlytics SHARED IMPORTED)
set_property(TARGET Crashlytics PROPERTY IMPORTED_LOCATION ${CRASHLYTICS_LIB})
set_property(TARGET Crashlytics PROPERTY FRAMEWORK 1)

# fabric framework
set(FABRIC_ROOT ${MOAI_ROOT}/3rdparty/Fabric/Fabric.framework)
set(FABRIC_LIB ${FABRIC_ROOT}/Fabric)
add_library(Fabric SHARED IMPORTED)
set_property(TARGET Fabric PROPERTY IMPORTED_LOCATION ${FABRIC_LIB})
set_property(TARGET Fabric PROPERTY FRAMEWORK 1)

install(DIRECTORY "${CRASHLYTICS_ROOT}" DESTINATION lib)
install(DIRECTORY "${FABRIC_ROOT}" DESTINATION lib)

add_library(moai-ios-crashlytics STATIC
        "${MOAI_ROOT}/src/moai-ios-crashlytics/MOAICrashlyticsIOS.h"
        "${MOAI_ROOT}/src/moai-ios-crashlytics/MOAICrashlyticsIOS.mm"
        "${MOAI_ROOT}/src/moai-ios-crashlytics/host.mm"
        )

target_include_directories(moai-ios-crashlytics PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${CRASHLYTICS_ROOT}>"
        "$<BUILD_INTERFACE:${FABRIC_ROOT}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-crashlytics moai-sim moai-util moai-core)
set_target_properties( moai-ios-crashlytics PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-crashlytics EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-crashlytics/host.h DESTINATION include/moai-ios-crashlytics)
