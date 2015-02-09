PROJECT(libmodutils)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

# Include the top-level include/ directory.
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")

# Sources.
SET(libmodutils_SRC
	AnimationFile.cpp
	LandTableInfo.cpp
	ModelInfo.cpp
	GoalRing.cpp
	GoalRingModels.cpp
	)

# Headers.
SET(libmodutils_H
	AnimationFile.h
	LandTableInfo.h
	ModelInfo.h
	)

# SADXModLoader is compiled as a Unicode DLL.
# ANSI functions are accessed directly if necessary.
SET(libmodutils_C_FLAGS_COMMON "-DUNICODE -D_UNICODE")

# This applies to the current subdirectory only.
SET(CMAKE_C_FLAGS_DEBUG		"${CMAKE_C_FLAGS_DEBUG} ${libmodutils_C_FLAGS_COMMON}")
SET(CMAKE_CXX_FLAGS_DEBUG	"${CMAKE_CXX_FLAGS_DEBUG} ${libmodutils_C_FLAGS_COMMON}")
SET(CMAKE_C_FLAGS_RELEASE	"${CMAKE_C_FLAGS_RELEASE} ${libmodutils_C_FLAGS_COMMON}")
SET(CMAKE_CXX_FLAGS_RELEASE	"${CMAKE_CXX_FLAGS_RELEASE} ${libmodutils_C_FLAGS_COMMON}")

# MSVC precompiled header.
INCLUDE(AddMSVCPrecompiledHeader)
ADD_MSVC_PRECOMPILED_HEADER("stdafx.h" "stdafx.cpp" libmodutils_SRC)
LIST(APPEND libmodutils_H stdafx.h)

######################
# Build the library. #
######################

ADD_LIBRARY(modutils STATIC
	${libmodutils_SRC}
	${libmodutils_H}
	)
INCLUDE(SetMSVCDebugPath)
SET_MSVC_DEBUG_PATH(modutils)

#################
# Installation. #
#################

INSTALL(FILES ${libmodutils_SRC} ${libmodutils_H}
	DESTINATION "libmodutils"
	COMPONENT "sdk"
	)
