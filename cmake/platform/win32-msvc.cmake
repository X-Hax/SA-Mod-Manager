# Win32-specific CFLAGS/CXXFLAGS.
# For Microsoft Visual C++ compilers.

# Basic platform flags:
# - wchar_t should be a distinct type.
# - Enable strict type checking in the Windows headers.
# - Set minimum Windows version to Windows 2000. (Windows NT 5.0)
SET(MCRECOVER_C_FLAGS_WIN32 "-Zc:wchar_t -DSTRICT -D_WIN32_WINNT=0x0500")

# Append the CFLAGS.
SET(MODLOADER_C_FLAGS_COMMON "${MODLOADER_C_FLAGS_COMMON} ${MODLOADER_C_FLAGS_WIN32}")
SET(MODLOADER_CXX_FLAGS_COMMON "${MODLOADER_CXX_FLAGS_COMMON} ${MODLOADER_C_FLAGS_WIN32}")
UNSET(MODLOADER_C_FLAGS_WIN32)
