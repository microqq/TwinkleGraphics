# Locate the FREEIMAGE library


set( _freeimage_HEADER_SEARCH_DIRS
"/usr/include"
"/usr/local/include"
"${CMAKE_SOURCE_DIR}/includes"
#win32
"$ENV{CUSTOM_DEV_DEPENDENCIES_PATH}/FreeImage" )
set( _freeimage_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"${CMAKE_SOURCE_DIR}/lib"
#win32
"$ENV{CUSTOM_DEV_DEPENDENCIES_PATH}/FreeImage/win" )

# Check environment for root search directory
set( _freeimage_ENV_ROOT $ENV{FREEIMAGE_ROOT} )
if( NOT FREEIMAGE_ROOT AND _freeimage_ENV_ROOT )
	set(FREEIMAGE_ROOT ${_freeimage_ENV_ROOT} )
endif()

# Put user specified location at beginning of search
if( FREEIMAGE_ROOT )
	list( INSERT _freeimage_HEADER_SEARCH_DIRS 0 "${FREEIMAGE_ROOT}/include" )
	list( INSERT _freeimage_LIB_SEARCH_DIRS 0 "${FREEIMAGE_ROOT}/lib" )
endif()

# Search for the header
FIND_PATH(FREEIMAGE_INCLUDE_DIR "freeimage.h"
PATHS ${_freeimage_HEADER_SEARCH_DIRS} )

# Search for the library
if(WIN32)
	FIND_LIBRARY(FREEIMAGE_LIBRARY NAMES FreeImage
		PATHS ${_freeimage_LIB_SEARCH_DIRS} )
else()
	FIND_LIBRARY(FREEIMAGE_LIBRARY NAMES freeimage
		PATHS ${_freeimage_LIB_SEARCH_DIRS} )
endif(WIN32)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FREEIMAGE DEFAULT_MSG
	FREEIMAGE_LIBRARY FREEIMAGE_INCLUDE_DIR)