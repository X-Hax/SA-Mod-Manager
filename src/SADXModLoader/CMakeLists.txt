PROJECT(SADXModLoader)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

# See http://www.cmake.org/Wiki/BuildingWinDLL
# Note that SADXModLoader doesn't export any symbols.

# SADXModLoader source and binary directories.
# Binary directory is required for generated headers.
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

# Include the top-level include/ directory.
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")

# Include the top-level binary directory for git_version.h.
INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}")

# External libraries.
FIND_LIBRARY(BASS_LIBRARY bass "${CMAKE_SOURCE_DIR}/extlib/bass")
IF(NOT BASS_LIBRARY)
	MESSAGE(FATAL_ERROR "BASS library is missing")
ENDIF(NOT BASS_LIBRARY)
FIND_LIBRARY(BASS_VGMSTREAM_LIBRARY bass_vgmstream "${CMAKE_SOURCE_DIR}/extlib/bass")
IF(NOT BASS_VGMSTREAM_LIBRARY)
	MESSAGE(FATAL_ERROR "BASS_VGMSTREAM library is missing")
ENDIF(NOT BASS_VGMSTREAM_LIBRARY)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/extlib/bass")

# Write the config.h file.
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/config.SADXModLoader.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/config.SADXModLoader.h"
	)

# Sources.
SET(SADXModLoader_SRC
	dllmain.cpp
	CodeParser.cpp
	IniFile.cpp
	FileMap.cpp
	MediaFns.cpp
	TextConv.cpp
	TextureReplacement.cpp
	)

# Headers.
SET(SADXModLoader_H
	CodeParser.hpp
	IniFile.hpp
	FileMap.hpp
	MediaFns.hpp
	TextConv.hpp
	TextureReplacement.h
	)

# SADXModLoader is compiled as a Unicode DLL.
# ANSI functions are accessed directly if necessary.
SET(SADXModLoader_C_FLAGS_COMMON "-DUNICODE -D_UNICODE -D_USRDLL -DSADXMODLOADER_EXPORTS")

# This applies to the current subdirectory only.
SET(CMAKE_C_FLAGS_DEBUG		"${CMAKE_C_FLAGS_DEBUG} ${SADXModLoader_C_FLAGS_COMMON}")
SET(CMAKE_CXX_FLAGS_DEBUG	"${CMAKE_CXX_FLAGS_DEBUG} ${SADXModLoader_C_FLAGS_COMMON}")
SET(CMAKE_C_FLAGS_RELEASE	"${CMAKE_C_FLAGS_RELEASE} ${SADXModLoader_C_FLAGS_COMMON}")
SET(CMAKE_CXX_FLAGS_RELEASE	"${CMAKE_CXX_FLAGS_RELEASE} ${SADXModLoader_C_FLAGS_COMMON}")

# MSVC precompiled header.
INCLUDE(AddMSVCPrecompiledHeader)
ADD_MSVC_PRECOMPILED_HEADER("stdafx.h" "stdafx.cpp" SADXModLoader_SRC)
LIST(APPEND SADXModLoader_H stdafx.h)

# Resource file.
# Add this after PCH to prevent the RC from depending on the PCH,
# since the MSVC resource compiler doesn't support /FI.
LIST(APPEND SADXModLoader_SRC SADXModLoader.rc)

######################
# Build the library. #
######################

ADD_LIBRARY(SADXModLoader SHARED
	${SADXModLoader_SRC}
	${SADXModLoader_H}
	)
TARGET_LINK_LIBRARIES(SADXModLoader
	${BASS_LIBRARY}
	${BASS_VGMSTREAM_LIBRARY}
	shlwapi
	dbghelp
	gdiplus
	modutils
	)

# Set the MSVC debug path.
INCLUDE(SetMSVCDebugPath)
SET_MSVC_DEBUG_PATH(SADXModLoader)

# Win32: image version.
INCLUDE(Win32ImageVersionLinkerFlags)
WIN32_IMAGE_VERSION_LINKER_FLAGS(${VERSION_MAJOR} ${VERSION_MINOR})

# Split debug information. (MinGW only)
INCLUDE(SplitDebugInformation)
SPLIT_DEBUG_INFORMATION(SADXModLoader)

#################
# Installation. #
#################

INSTALL(TARGETS SADXModLoader
	RUNTIME DESTINATION "mods"
	LIBRARY DESTINATION "mods"
	ARCHIVE DESTINATION "mods"
	COMPONENT "modloader"
	)

IF(MSVC)
	INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/SADXModLoader.pdb"
		DESTINATION "mods"
		COMPONENT "debug"
		)
ELSE(MSVC)
	INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/SADXModLoader.debug"
		DESTINATION "mods"
		COMPONENT "debug"
		)
ENDIF(MSVC)
