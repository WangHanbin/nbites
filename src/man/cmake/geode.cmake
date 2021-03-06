
include("${CMAKE_CURRENT_LIST_DIR}/find_nbites_dir.cmake")

############################ DEFINES && DEFINITIONS
SET( GEODE_BUILD ON )
SET( OFFLINE OFF )
SET( BUILDING_FOR_A_REMOTE_NAO ON )

############################# CROSS-COMPILATION VARIABLES
# Set the variable for the cross-compilation directory, cmake variables

SET( TOOLCHAIN_DIR "${NBITES_DIR}/lib/geodetoolchain" )
SET( OE_SYSROOT "${TOOLCHAIN_DIR}/sysroot/" )
SET( ROBOT_TYPE "NAO_RL_33" )

SET( CMAKE_CROSSCOMPILING   TRUE  )
SET( CMAKE_SYSTEM_NAME      Linux )
SET( CMAKE_SYSTEM_VERSION   1     )
SET( CMAKE_SYSTEM_PROCESSOR geode )

INCLUDE("${TOOLCHAIN_DIR}/toolchain-geode.cmake")

SET( CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -m32 -march=geode -mmmx -m3dnow -fno-align-jumps -fno-align-functions -fno-align-labels -fno-align-loops -pipe -fomit-frame-pointer")
SET( CMAKE_C_FLAGS
  "${CMAKE_CXX_FLAGS}" )

SET( CMAKE_BUILD_TYPE "release" CACHE STRING "Build type" FORCE)

# where should we look for libraries we need
SET(CMAKE_FIND_ROOT_PATH  ${OE_SYSROOT} ${NBITES_EXT})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
