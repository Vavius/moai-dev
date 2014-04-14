cmake_minimum_required ( VERSION 2.8.11 )
project ( box2d )

# keeping this long path as in the liquidfun full repo
# to be able build liquidfun testbed and samples without MOAI
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/liquidfun/liquidfun/Box2D/" )
set ( BOX2D_INCLUDES "${MOAI_ROOT}/3rdparty/liquidfun/liquidfun/Box2D/" )

file ( GLOB_RECURSE BOX2D_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Box2D/*.cpp" )
file ( GLOB_RECURSE BOX2D_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Box2D/*.h" )

add_library( box2d STATIC ${BOX2D_SOURCES} ${BOX2D_HEADERS} )

target_include_directories ( box2d PUBLIC ${BOX2D_INCLUDES})
set_target_properties( box2d PROPERTIES FOLDER Third-Party )

