PROJECT(data)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# ModLoader common headers.
SET(ModLoader_H
	ninja.h
	ModLoader/MemAccess.h
	)

# SADXModLoader headers.
SET(SADXModLoader_H
	SADXModLoader.h
	SADXModLoader/SADXEnums.h
	SADXModLoader/SADXFunctions.h
	SADXModLoader/SADXStructs.h
	SADXModLoader/SADXVars.h
	)

# TODO (maybe): Convert headers from Unix to DOS format.
# SADXModLoader is usually built with MSVC, so this isn't
# entirely necessary.

#################
# Installation. #
#################

INSTALL(FILES ${ModLoader_H} ${SADXModLoader_H}
	DESTINATION "include"
	COMPONENT "sdk"
	)
