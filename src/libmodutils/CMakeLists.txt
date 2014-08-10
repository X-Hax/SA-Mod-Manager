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

SET(LIBMODUTILS_PDB_FILE "${CMAKE_CURRENT_BINARY_DIR}/modutils.pdb")

IF(MSVC)
	# CMake seems to use weird settings for the PDB file.
	# (at least version 2.8.12.2; 3.0.0 might be different)
	STRING(REGEX REPLACE
		"/Fd<OBJECT_DIR>/${_FS_C}"
		"/Fd${LIBMODUTILS_PDB_FILE}"
		CMAKE_C_COMPILE_OBJECT "${CMAKE_C_COMPILE_OBJECT}")
	STRING(REGEX REPLACE
		"/Fd<OBJECT_DIR>/${_FS_CXX}"
		"/Fd${LIBMODUTILS_PDB_FILE}"
		CMAKE_CXX_COMPILE_OBJECT "${CMAKE_CXX_COMPILE_OBJECT}")
ENDIF(MSVC)

# Don't actually build the library, since the
# ModLoader doesn't use it.
IF(0)
ADD_LIBRARY(modutils STATIC
	${libmodutils_SRC}
	${libmodutils_H}
	)

SET_TARGET_PROPERTIES(modutils
	PROPERTIES
		PDB "${LIBMODUTILS_PDB_FILE}"
		VERSION "${VERSION_MAJOR}.${VERSION_MINOR}"
	)
ENDIF(0)

#################
# Installation. #
#################

INSTALL(FILES ${libmodutils_SRC} ${libmodutils_H}
	DESTINATION "libmodutils"
	COMPONENT "sdk"
	)
