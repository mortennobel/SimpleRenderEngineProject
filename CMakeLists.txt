cmake_minimum_required(VERSION 3.8)
project(SimpleRenderEngineProject CXX)

set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

IF (APPLE)
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
ELSE (APPLE)
    SET(SDL2_INCLUDE "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.5/include" CACHE STRING "Where the SDL2 headers can be found")  
    include_directories( ${SDL2_INCLUDE})
    SET(SDL2_LIBRARY ${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.5/lib/x86/SDL2.lib )
ENDIF (APPLE)
include_directories(sre)
include_directories(include)

#########################################################
# FIND OPENGL
#########################################################
IF(APPLE)
    INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
    MARK_AS_ADVANCED (OpenGL_LIBRARY)
    SET(EXTRA_LIBS ${OpenGL_LIBRARY})
ELSE (APPLE)
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})
    IF(UNIX)
        find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIR})
    ELSE(UNIX)
		SET(GLEW_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/glew-2.0.0/include" CACHE STRING "Where the glew.h can be found")
        include_directories(${GLEW_include})
		SET(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/Win/glew-2.0.0/lib/Release/Win32/glew32s.lib" CACHE STRING "Where the glew lib(static) can be found")  
    ENDIF(UNIX)
    
    SET(EXTRA_LIBS ${OPENGL_LIBRARY} ${GLEW_LIBRARY})
ENDIF (APPLE)

IF (APPLE)
    find_package(SDL2_IMAGE REQUIRED)
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

    find_package(SDL2_mixer REQUIRED)
    include_directories(${SDL2MIXER_INCLUDE_DIR})
ELSE (APPLE)
    SET(SDL2_image_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.1/include" CACHE STRING "Where the SDL_image.h can be found")
    include_directories(${SDL2_image_include})
	SET(SDL2_IMAGE_LIBRARIES "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.1/lib/x86/SDL2_image.lib" CACHE STRING "Location of SDL2_image.lib")
	
	SET(SDL2_mixer_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_mixer-2.0.1/include" CACHE STRING "Location of SDL_mixer.h")
    include_directories(${SDL2_mixer_include})
	SET(SDL2MIXER_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_mixer-2.0.1/lib/x86/SDL2_mixer.lib" CACHE STRING "Location of SDL2_mixer.lib")  
ENDIF(APPLE)

#### Box2D #####
IF(APPLE)
    INCLUDE_DIRECTORIES ( thirdparty/macOS/Box2D/include )
    SET(Box2D_LIBRARY_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/macOS/Box2D/lib/libBox2D.a )
	SET(Box2D_LIBRARY_REL ${CMAKE_SOURCE_DIR}/thirdparty/macOS/Box2D/lib/libBox2D.a )
ELSE (APPLE)
	SET(Box2D_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/Box2D/include" CACHE STRING "Location of Box2D/Box2D.h")
    INCLUDE_DIRECTORIES (${Box2D_include})
	SET(Box2D_LIBRARY_DEBUG "${CMAKE_SOURCE_DIR}/thirdparty/Win/Box2D/lib/Box2D_d.lib" CACHE STRING "Location of Box2D_d.lib")
	SET(Box2D_LIBRARY_REL "${CMAKE_SOURCE_DIR}/thirdparty/Win/Box2D/lib/Box2D.lib" CACHE STRING "Location of Box2D.lib")
ENDIF (APPLE)

#### Lua #####
IF(APPLE)
    SET(LUA_INCLUDE_DIR thirdparty/macOS/Lua/include)
    INCLUDE_DIRECTORIES (${LUA_INCLUDE_DIR})
    SET(LUA_LIBRARIES ${CMAKE_SOURCE_DIR}/thirdparty/macOS/Lua/lib/liblua.a )
ELSE (APPLE)
    SET(LUA_INCLUDE_DIR thirdparty/Win/Lua/include)
    INCLUDE_DIRECTORIES (${LUA_INCLUDE_DIR})
    SET(LUA_LIBRARIES ${CMAKE_SOURCE_DIR}/thirdparty/Win/Lua/lib/lua5.3.0-static.lib )
ENDIF (APPLE)

#### Sol #####
INCLUDE_DIRECTORIES (thirdparty/Sol/include)

add_subdirectory(src)
# add_subdirectory(examples)
add_subdirectory(project)