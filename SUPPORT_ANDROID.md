# INSTRUCTIONS FOR SUPPORT ANDROID HOST

## Adding new modules

### Modules types

1. Only Android libraries and frameworks

	???


2. Cross compiling libraries which supports bindings between Android host and Lua-Moai

First we need make necessary directories in Moai project

	* Make directory for cmake instructions: 
		/MOAI_ROOT/cmake/libmoai/moai-android/moai-android-<module_name.."-">

	* Make directory with sources:
		/MOAI_ROOT/src/moai-android-<module_name.."-">

	* Make directory with library or framework project:
		/MOAI_ROOT/3rdparty-android/<module_name>"-"<version>

Second we need edit the top-level build instructions files for build Moai with module which we want to add

	* Edit the top-level android cmake script which contains paths and rules for including modules
		/MOAI_ROOT/cmake/libmoai/moai-android/CMakeLists.txt
		add line "add_subdirectory(moai-android-<module_name.."-">)"

	* Edit android mk file which including modules when building starts
		/MOAI_ROOT/lib/android/modules/jni/moaiandroid.mk
		add lines after last addded module:

			"include $(CLEAR_VARS)"
			"LOCAL_MODULE := moai-android-<module_name.."-">"
			"LOCAL_SRC_FILES := $(MOAI_LIBS)/libmoai-android-<module_name.."-">.a"
			"include $(PREBUILT_STATIC_LIBRARY)"

When first and second steps is done we need put module files in 3rdparty and create cmake instructions

	* Make file "/MOAI_ROOT/cmake/libmoai/moai-android/moai-android-<module_name.."-">/CMakeLists.txt"

	* Add lines some like that:
		cmake_minimum_required ( VERSION 2.8.11 )
		project ( moai-android-<module_name.."-"> )

		android_module("moai-android-<module_name.."-">" "<module_name.."-">"-"<version>") //Folder with library or framework project


For undrestand how to write binding you can use examples in docs

Finally we need make source files with bindings

3. Only Moai-side native libraries and frameworks which necessary for running Android host 

	???


### Updating modules


### How to include modules in release version

