# Microsoft Visual C++

# Remove some default CFLAGS/CXXFLAGS.
STRING(REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Disable useless warnings:
# - MSVC "logo" messages
# - MSVCRT "deprecated" functions
SET(MODLOADER_C_FLAGS_COMMON "-nologo -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE")
SET(MODLOADER_CXX_FLAGS_COMMON "${MODLOADER_C_FLAGS_COMMON}")
# NOTE: Mod loader uses the high bit of addresses for other purposes.
# SADX itself isn't large address aware, either.
# FIXME: cmake-2.8.5 is required for /manifest:no to work correctly.
SET(MODLOADER_EXE_LINKER_FLAGS_COMMON "-nologo /manifest:no -subsystem:5.01 -dynamicbase -nxcompat -largeaddressaware:no")

# Disable C++ RTTI and asynchronous exceptions.
SET(MODLOADER_CXX_FLAGS_COMMON "${MODLOADER_CXX_FLAGS_COMMON} -GR- -EHsc")

# Disable the RC and MASM "logo".
SET(CMAKE_RC_FLAGS "-nologo")
SET(CMAKE_ASM_MASM_FLAGS "-nologo")

# Debug/release flags.
SET(MODLOADER_C_FLAGS_DEBUG "-Od -MDd -RTC1")
SET(MODLOADER_CXX_FLAGS_DEBUG "-Od -MDd -RTC1")
SET(MODLOADER_EXE_LINKER_FLAGS_DEBUG "-DEBUG")
SET(MODLOADER_C_FLAGS_RELEASE "-O2 -MD")
SET(MODLOADER_CXX_FLAGS_RELEASE "-O2 -MD")
SET(MODLOADER_EXE_LINKER_FLAGS_RELEASE "-DEBUG")
