#-*-cmake-*-
#
# Test for Exiv2 sources
#
# Once loaded this will define
#  EXIV2_FOUND        - system has Exiv2
#  EXIV2_INCLUDE_DIR  - include directory for Exiv2
#

SET(EXIV2_FOUND "NO")

FIND_PATH(EXIV2_INCLUDE_DIR exiv2/image.hpp
	"$ENV{EXIV2_LOCATION}"
	"$ENV{EXIV2_LOCATION}/include"
	"$ENV{EXIV2_HOME}/include"
	/usr/include
	/usr/include/Exiv2
	/opt/local/include/Exiv2
)

IF(EXIV2_INCLUDE_DIR)
	SET(EXIV2_FOUND "YES")
ENDIF(EXIV2_INCLUDE_DIR)

IF(NOT EXIV2_FOUND)
	# make FIND_PACKAGE friendly
	IF(NOT Exiv2_FIND_QUIETLY)
		IF(Exiv2_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "Exiv2 required, please specify it's location with EXIV2_HOME or EXIV2_LOCATION")
		ELSE(Exiv2_FIND_REQUIRED)
			MESSAGE(STATUS "Exiv2 was not found.")
		ENDIF(Exiv2_FIND_REQUIRED)
	ENDIF(NOT Exiv2_FIND_QUIETLY)
ENDIF(NOT EXIV2_FOUND)


#####

