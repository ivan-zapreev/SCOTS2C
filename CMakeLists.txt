# Build the SCOTS v2.0 controller Mathematica interface module
cmake_minimum_required(VERSION 2.8.12)

project (Mathematica-interface-for-SCOTS-2-0)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Mathematica")

set (CMAKE_VERBOSE_MAKEFILE OFF)
set (CMAKE_COLOR_MAKEFILE ON)
set (CMAKE_SKIP_ASSEMBLY_SOURCE_RULES ON)
set (CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
set (Mathematica_USE_STATIC_LIBRARIES TRUE)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Debug)
endif()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (POLICY CMP0058)
	# suppress phony rules warning when using ninja generator
	cmake_policy(SET CMP0058 OLD)
endif()

# finds newest Mathematica installation and its default components
#find_package(Mathematica)
find_package(Mathematica COMPONENTS WSTP WolframLibrary)

if (Mathematica_MathLink_DEFINITIONS)
	add_definitions(${Mathematica_MathLink_DEFINITIONS})
endif()

if (Mathematica_WSTP_DEFINITIONS)
	add_definitions(${Mathematica_WSTP_DEFINITIONS})
endif()

SET(OPT_LOCAL_LIB_DIR "/opt/local/lib")
SET(USR_LOCAL_LIB_DIR "/usr/local/lib")
SET(LIB_DIRS "")
if (EXISTS "${OPT_LOCAL_LIB_DIR}" AND IS_DIRECTORY "${OPT_LOCAL_LIB_DIR}")
	SET(LIB_DIRS ${LIB_DIRS} ${OPT_LOCAL_LIB_DIR})
endif()
if (EXISTS "${USR_LOCAL_LIB_DIR}" AND IS_DIRECTORY "${USR_LOCAL_LIB_DIR}")
	SET(LIB_DIRS ${LIB_DIRS} ${USR_LOCAL_LIB_DIR})
endif()
link_directories(${LIB_DIRS})

if (${Mathematica_FOUND})
	message(STATUS "Mathematica Version ${Mathematica_VERSION}")

	if (${Mathematica_WSTP_FOUND})
		message(STATUS "Mathematica WSTP directory ${Mathematica_WSTP_HOST_ROOT_DIR}")
		message(STATUS "Mathematica WSTP include directory ${Mathematica_WSTP_INCLUDE_DIR}")
		message(STATUS "Mathematica WSTP library directory ${Mathematica_WSTP_LIBRARY}")
		message(STATUS "Residing within ${CMAKE_CURRENT_LIST_DIR}")

		#####################################
		# WSTP interface
		#####################################
     		#Find the source files
     		file (GLOB_RECURSE _SourceFiles "./src/wstp/*.tm" "./src/wstp/*.c" "./src/wstp/*.cc")
     		#Copy the source files
     		file (COPY ${_SourceFiles}
			DESTINATION "${CMAKE_BINARY_DIR}/src/wstp/"
			NO_SOURCE_PERMISSIONS)
		#Add the WSTP application source folder
		add_subdirectory(src/wstp/)
	else()
		message(WARNING "The Mathematica WSTP directory is not found, the WSTP interface is not build!")
	endif()

	if (${Mathematica_WolframLibrary_FOUND})
		#####################################
		# LibraryLink interface
		#####################################
     		#Find the source files
     		file (GLOB_RECURSE _SourceFiles "./src/liblink/*.cc")
     		#Copy the source files
     		file (COPY ${_SourceFiles}
			DESTINATION "${CMAKE_BINARY_DIR}/src/liblink/"
			NO_SOURCE_PERMISSIONS)
		#Add the LibraryLink library source folder
		add_subdirectory(src/liblink/)
	else()
		message(WARNING "The Mathematica LibraryLink directory is not found, the LibLink interface is not build!")
	endif()
else()
	message(WARNING "The Mathematica is not found, WSTP and LibLink interfaces are not build!")
endif()

#####################################
# SCOTSv2 optimal controller determinizer
#####################################
#Find the source files
file (GLOB_RECURSE _SourceFiles "./src/optdet/*.cc")
#Copy the source files
file (COPY ${_SourceFiles}
	DESTINATION "${CMAKE_BINARY_DIR}/src/optdet/"
	NO_SOURCE_PERMISSIONS)
#Add the LibraryLink library source folder
add_subdirectory(src/optdet/)
