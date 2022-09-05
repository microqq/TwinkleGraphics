# Locate the GLEW library


set( _glew_HEADER_SEARCH_DIRS
"/usr/include"
"/usr/local/include"
"${CMAKE_SOURCE_DIR}/ThirdParty/includes"
#win32
"${GLEWINCLUDEDIR}")
set( _glew_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"${CMAKE_SOURCE_DIR}/ThirdParty/lib"
#win32
"${GLEWLIBDIR}")

# Check environment for root search directory
set( _glew_ENV_ROOT $ENV{GLEW_ROOT} )
if( NOT GLEW_ROOT AND _glew_ENV_ROOT )
	set(GLEW_ROOT ${_glew_ENV_ROOT} )
endif()

# Put user specified location at beginning of search
if( GLEW_ROOT )
	list( INSERT _glew_HEADER_SEARCH_DIRS 0 "${GLEW_ROOT}/include" )
	list( INSERT _glew_LIB_SEARCH_DIRS 0 "${GLEW_ROOT}/lib" )
endif()

# Search for the header
FIND_PATH(GLEW_INCLUDE_DIR "GL/glew.h"
PATHS ${_glew_HEADER_SEARCH_DIRS} )

# Search for the library
if(WIN32)
	FIND_LIBRARY(GLEW_LIBRARY NAMES glew32
		PATHS ${_glew_LIB_SEARCH_DIRS} )
else()
	FIND_LIBRARY(GLEW_LIBRARY NAMES glew
		PATHS ${_glew_LIB_SEARCH_DIRS} )
endif(WIN32)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)