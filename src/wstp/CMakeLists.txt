# FindMathematica WSTP examples

include_directories(${Mathematica_INCLUDE_DIRS})

SET(BASEPATH "${CMAKE_SOURCE_DIR}")

#Bring the headers into the project
include_directories(SYSTEM ${BASEPATH}/ext/SCOTSv0.2/src ${BASEPATH}/ext/SCOTSv0.2/utils)

#Add the required flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -O3 -DNDEBUG -DSCOTS_BDD")

#Build the C++ wrapper implementation library for the SCOTSv2.0 and CUDD
add_library(scots2ext STATIC scots2ext.cc)
target_link_libraries(scots2ext cudd)
target_link_libraries(scots2ext ${Mathematica_WSTP_LIBRARIES})

#Build the WSTP application that will use the SCOTSv2.0 and CUDD connected library
set (_Templates scots2ext.tm)
set (_Targets "")

if (NOT CMAKE_CROSSCOMPILING)
	# add a CMake executable target for each WSTP template based example
	foreach (_Template ${_Templates})
           message(STATUS "Processing template: ${_Template}")
		set (_TemplateFileName "${CMAKE_CURRENT_BINARY_DIR}/${_Template}")
		if (EXISTS "${_TemplateFileName}")
	           message(STATUS "The template file ${_TemplateFileName} is found")
			get_filename_component(_OutputName "${_TemplateFileName}" NAME_WE)
			set (_TargetName "ws${_OutputName}")
			if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${_OutputName}.c")
				Mathematica_WSTP_add_executable (${_TargetName}
					"${_TemplateFileName}" "${CMAKE_CURRENT_BINARY_DIR}/${_OutputName}.c")
			else()
				Mathematica_WSTP_WSPREP_TARGET("${_TemplateFileName}" OUTPUT ${_Template}.c)
				add_executable (${_TargetName} "${_TemplateFileName}.c")
				target_link_libraries (${_TargetName} ${Mathematica_WSTP_LIBRARIES})
			endif()
			target_link_libraries (${_TargetName} scots2ext)
			target_link_libraries (${_TargetName} cudd)
			if (TARGET ${_TargetName})
				set_target_properties(${_TargetName} PROPERTIES OUTPUT_NAME "${_OutputName}")
				set_target_properties(${_TargetName} PROPERTIES FOLDER "WSTP")
				list (APPEND _Targets ${_TargetName})
			endif()
			if (Mathematica_WSTP_LINKER_FLAGS)
				set_target_properties(${_TargetName} PROPERTIES LINK_FLAGS "${Mathematica_WSTP_LINKER_FLAGS}")
			endif()
		else()
     			message(FATAL_ERROR "The template file ${_TemplateFileName} is missing!")
		endif()
           message(STATUS "Processing template: ${_Template} done")
	endforeach()

	# export wsprep header and trailer code for the current target platform
	Mathematica_WSTP_WSPREP_EXPORT_FRAMES(
		OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/wsprep-frames"
		SYSTEM_ID ${Mathematica_SYSTEM_ID})

else()
     #The cross compilation is not supported
     message(FATAL_ERROR "Cross compilation is not supported!")
endif()

# fix WSTP shared library references under Mac OS X
Mathematica_ABSOLUTIZE_LIBRARY_DEPENDENCIES(${_Targets})

message(STATUS "The targets are: ${_Targets}")
