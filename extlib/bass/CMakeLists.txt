PROJECT(bass)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# BASS library and additional DLLs.
SET(BASS_DLL_FILES
	bass.dll
	bass_vgmstream.dll
	libmpg123-0.dll
	libogg.dll
	libvorbis.dll
	libvorbisfile.dll
	)

# BASS VGMSTREAM documentation.
SET(BASS_VGMSTREAM_DOC_FILES
	COPYING_VGMSTREAM
	COPYING_BASS_VGMSTREAM
	)

# TODO (maybe): Convert *.xml from Unix to DOS format.
# SADXModLoader is usually built with MSVC, so this isn't
# entirely necessary.

#################
# Installation. #
#################

INSTALL(FILES ${BASS_DLL_FILES} ${BASS_VGMSTREAM_DOC_FILES}
	DESTINATION "."
	COMPONENT "ModLoader"
	)
