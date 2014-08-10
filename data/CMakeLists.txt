PROJECT(data)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Documentation.
SET(DATA_FILES
	Border.png
	Codes.xml
	)

# TODO (maybe): Convert *.xml from Unix to DOS format.
# SADXModLoader is usually built with MSVC, so this isn't
# entirely necessary.

#################
# Installation. #
#################

INSTALL(FILES ${DATA_FILES}
	DESTINATION "mods"
	COMPONENT "ModLoader"
	)
