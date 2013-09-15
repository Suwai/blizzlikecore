# This file is part of the BlizzLikeCore Project. See CREDITS and LICENSE files.
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

cmake_minimum_required (VERSION 2.6)
project(BlizzLikeMoveMapGen)

add_definitions(-DMMAP_GENERATOR -DNO_CORE_FUNCS -DDEBUG)

# zlib
add_definitions( -DNO_vsnprintf )

if(NOT WIN32)
  add_definitions("-ggdb")
  add_definitions("-O3")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/src/game
    ${CMAKE_SOURCE_DIR}/src/game/vmap
    ${CMAKE_SOURCE_DIR}/dep/include/g3dlite
    ${CMAKE_SOURCE_DIR}/src/framework
    ${CMAKE_SOURCE_DIR}/dep/ACE_wrappers
    ${CMAKE_SOURCE_DIR}/objdir/dep/ACE_wrappers
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Include
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Include
    ${CMAKE_SOURCE_DIR}/dep/src/zlib
)

add_library(mvmap
	${CMAKE_SOURCE_DIR}/src/game/vmap/BIH.cpp
	${CMAKE_SOURCE_DIR}/src/game/vmap/VMapManager2.cpp
	${CMAKE_SOURCE_DIR}/src/game/vmap/MapTree.cpp
	${CMAKE_SOURCE_DIR}/src/game/vmap/TileAssembler.cpp
	${CMAKE_SOURCE_DIR}/src/game/vmap/WorldModel.cpp
	${CMAKE_SOURCE_DIR}/src/game/vmap/ModelInstance.cpp
)

if(NOT WIN32)
  add_library(zlib
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/adler32.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/compress.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/crc32.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/deflate.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/example.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/gzio.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/infback.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/inffast.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/inflate.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/inftrees.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/trees.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/uncompr.c
      ${CMAKE_SOURCE_DIR}/dep/src/zlib/zutil.c
  )
endif()

target_link_libraries(mvmap g3dlite zlib)

add_library(mdetour
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Source/DetourAlloc.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Source/DetourCommon.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Source/DetourNavMesh.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Source/DetourNavMeshBuilder.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Source/DetourNavMeshQuery.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Source/DetourNode.cpp
)

add_library(mrecast
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Source/Recast.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Source/RecastAlloc.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Source/RecastArea.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Source/RecastContour.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Source/RecastFilter.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Source/RecastMesh.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Source/RecastMeshDetail.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Source/RecastRasterization.cpp
    ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Source/RecastRegion.cpp
)

set(SOURCES
    ./src/IntermediateValues.cpp
    ./src/generator.cpp
    ./src/MapBuilder.cpp
    ./src/TerrainBuilder.cpp
    ./src/VMapExtensions.cpp
)

add_executable( MoveMapGen ${SOURCES} )

target_link_libraries( MoveMapGen g3dlite mvmap mdetour mrecast zlib )

install(TARGETS MoveMapGen DESTINATION ${BIN_DIR}/tools)