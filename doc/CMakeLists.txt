PROJECT(doc)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Documentation.
SET(DOC_FILES
	codes.md
	CodeList.xsd
	)

# TODO (maybe): Convert headers from Unix to DOS format.
# SADXModLoader is usually built with MSVC, so this isn't
# entirely necessary.

#################
# Installation. #
#################

INSTALL(FILES ${DOC_FILES}
	DESTINATION "doc"
	COMPONENT "doc"
	)
