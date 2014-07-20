PROJECT(ModLoader)

# Version dependencies:
# - BREAK(): CMake 2.6.0
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

# UNSET() was added in cmake-2.6.3.
# Define an equivalent macro for older versions.
MACRO(UNSET var)
	SET(${var})
ENDMACRO(UNSET)

list (APPEND CMAKE_MODULE_PATH
	"${mcrecover_base_SOURCE_DIR}/cmake/macros"
	)

# Program information.
SET(DESCRIPTION "SADX Mod Loader")
SET(VERSION_MAJOR 3)
SET(VERSION_MINOR 0)
SET(VERSION_PATCH 0)
SET(VERSION_DEVEL 1)
IF(VERSION_PATCH)
	SET(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}+")
ELSE(VERSION_PATCH)
	SET(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}")
ENDIF(VERSION_PATCH)
IF(VERSION_DEVEL)
	SET(VERSION_STRING "${VERSION_STRING}+")
ENDIF(VERSION_DEVEL)
SET(VERSION_STRING_WIN32 "${VERSION_MAJOR},${VERSION_MINOR},${VERSION_PATCH},${VERSION_DEVEL}")

# Git version information.
FIND_PROGRAM(POSIX_SH sh)
IF(POSIX_SH)
	# sh is available.
	# Run the git version script.
	IF(WIN32)
		SET(ENV{SHELLOPTS} igncr)
	ENDIF(WIN32)
	ADD_CUSTOM_TARGET(git_version ALL
		${POSIX_SH} ${ModLoader_SOURCE_DIR}/git_version.sh
		-s "${ModLoader_SOURCE_DIR}"
		-o "${ModLoader_BINARY_DIR}/git_version.h"
		VERBATIM
		)
ELSE(POSIX_SH)
	# sh isn't available.
	# Create a blank git_version.h.
	FILE(WRITE "${ModLoader_BINARY_DIR}/git_version.h"
		"/* dummy file; POSIX sh is not available */\n")
ENDIF(POSIX_SH)

# If no build type is set, default to "debug".
IF(CMAKE_BUILD_TYPE MATCHES ^$)
	SET(CMAKE_BUILD_TYPE "debug")
ENDIF()

# Don't bother with verbose makefiles for now.
# TODO: Make it an option?
#IF (CMAKE_BUILD_TYPE MATCHES ^debug$)
#	SET(CMAKE_VERBOSE_MAKEFILE ON)
#ENDIF()

# Check for platform-specific functionality.
IF(NOT MSVC)
	MESSAGE(FATAL_ERROR "Only MSVC is supported right now.")
ENDIF(NOT MSVC)
INCLUDE(cmake/platform.cmake)

# Set the default CFLAGS.
SET(CMAKE_C_FLAGS_DEBUG "${MODLOADER_C_FLAGS_DEBUG}")
SET(CMAKE_CXX_FLAGS_DEBUG "${MODLOADER_CXX_FLAGS_DEBUG}")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${MODLOADER_EXE_LINKER_FLAGS_DEBUG}")
SET(CMAKE_C_FLAGS_RELEASE "${MODLOADER_C_FLAGS_RELEASE}")
SET(CMAKE_CXX_FLAGS_RELEASE "${MODLOADER_CXX_FLAGS_RELEASE}")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${MODLOADER_EXE_LINKER_FLAGS_RELEASE}")

# Project subdirectories.
ADD_SUBDIRECTORY(src)
