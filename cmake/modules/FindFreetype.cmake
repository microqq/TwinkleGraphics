option(FT_WITH_ZLIB OFF)
option(FT_WITH_BZIP2 OFF)
option(FT_WITH_PNG OFF)
option(FT_WITH_HARFBUZZ OFF)
option(FT_WITH_BROTLI OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/freetype)

set_target_properties(freetype PROPERTIES FOLDER Thirdparty)

set(FREETYPE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/freetype/include)
set(FREETYPE_LIBRARIES freetype)

add_definitions(-DFT_DEBUG_LEVEL_TRACE)