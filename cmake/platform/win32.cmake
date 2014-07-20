# Win32-specific CFLAGS/CXXFLAGS.
# Redirection module.

# Common Win32 flags:
# - Set minimum Windows version to Windows 2000. (Windows NT 5.0)
# - Enable STRICT.
SET(MODLOADER_C_FLAGS_WIN32 "-D_WIN32_WINNT=0x0500 -DSTRICT")

IF(MSVC)
	INCLUDE(cmake/platform/win32-msvc.cmake)
ELSE(MSVC)
	INCLUDE(cmake/platform/win32-gcc.cmake)
ENDIF(MSVC)
