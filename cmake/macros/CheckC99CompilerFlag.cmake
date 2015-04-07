# - Check what flag is needed to activate C99 mode.
# CHECK_C99_COMPILER_FLAG(VARIABLE)
#
#  VARIABLE - variable to store the result
# 
#  This actually calls the check_c_source_compiles macro.
#  See help for CheckCSourceCompiles for a listing of variables
#  that can modify the build.

# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# Based on CHECK_C_COMPILER_FLAG(). (CheckCCompilerFlag.cmake)

INCLUDE(CheckCSourceCompiles)

MACRO(CHECK_C99_COMPILER_FLAG _RESULT)
	# Flag listing borrowed from GNU autoconf's AC_PROG_CC_C99 macro.
	UNSET(${_RESULT})
	
	MESSAGE(STATUS "Checking what CFLAG is required for C99:")
	IF(NOT MSVC)
	FOREACH(CHECK_C99_CFLAG "-std=gnu99" "-std=c99" "-c99" "-AC99" "-xc99=all" "-qlanglvl=extc99")
		SET(SAFE_CMAKE_REQUIRED_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS}")
		SET(CMAKE_REQUIRED_DEFINITIONS "${CHECK_C99_CFLAG}")
		CHECK_C_SOURCE_COMPILES("int main() { return 0; }" CFLAG_${CHECK_C99_CFLAG})
		SET(CMAKE_REQUIRED_DEFINITIONS "${SAFE_CMAKE_REQUIRED_DEFINITIONS}")
		IF(CFLAG_${CHECK_C99_CFLAG})
			SET(${_RESULT} ${CHECK_C99_CFLAG})
			BREAK()
		ENDIF(CFLAG_${CHECK_C99_CFLAG})
		UNSET(CFLAG_${CHECK_C99_CFLAG})
	ENDFOREACH()
	ENDIF(NOT MSVC)
	
	IF(${_RESULT})
		MESSAGE(STATUS "Checking what CFLAG is required for C99: ${${_RESULT}}")
	ELSE(${_RESULT})
		MESSAGE(STATUS "Checking what CFLAG is required for C99: none")
	ENDIF(${_RESULT})
ENDMACRO (CHECK_C99_COMPILER_FLAG)
