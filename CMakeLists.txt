cmake_minimum_required(VERSION 3.8)
project(SimpleRenderEngineProject CXX)

set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

IF (UNIX)
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
ELSE (UNIX)
    SET(SDL2_INCLUDE "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.6/include" CACHE STRING "Where the SDL2 headers can be found")
    include_directories( ${SDL2_INCLUDE})
    SET(SDL2_LIBRARY ${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.6/lib/x86/SDL2.lib )
ENDIF (UNIX)

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
		SET(GLEW_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/glew-2.1.0/include" CACHE STRING "Where the glew.h can be found")
        include_directories(${GLEW_include})
		SET(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/Win/glew-2.1.0/lib/Release/Win32/glew32s.lib" CACHE STRING "Where the glew lib(static) can be found")  
    ENDIF(UNIX)
    
    SET(EXTRA_LIBS ${OPENGL_LIBRARY} ${GLEW_LIBRARY})
ENDIF (APPLE)

IF (UNIX)
    find_package(SDL2_IMAGE REQUIRED)
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

    find_package(SDL2_mixer REQUIRED)
    include_directories(${SDL2MIXER_INCLUDE_DIR})
ELSE (UNIX)
    SET(SDL2_image_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.1/include" CACHE STRING "Where the SDL_image.h can be found")
    include_directories(${SDL2_image_include})
	SET(SDL2_IMAGE_LIBRARIES "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.1/lib/x86/SDL2_image.lib" CACHE STRING "Location of SDL2_image.lib")
	
	SET(SDL2_mixer_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_mixer-2.0.1/include" CACHE STRING "Location of SDL_mixer.h")
    include_directories(${SDL2_mixer_include})
	SET(SDL2MIXER_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_mixer-2.0.1/lib/x86/SDL2_mixer.lib" CACHE STRING "Location of SDL2_mixer.lib")  
ENDIF(UNIX)

#### Box2D #####
IF(APPLE)
    INCLUDE_DIRECTORIES ( thirdparty/macOS/Box2D/include )
    SET(Box2D_LIBRARY_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/macOS/Box2D/lib/libBox2D.a )
	SET(Box2D_LIBRARY_REL ${CMAKE_SOURCE_DIR}/thirdparty/macOS/Box2D/lib/libBox2D.a )
ELSE (APPLE)
    IF (UNIX)
        INCLUDE_DIRECTORIES ( thirdparty/Linux/Box2D/include )
        SET(Box2D_LIBRARY_DEBUG ${CMAKE_SOURCE_DIR}/thirdparty/Linux/Box2D/lib/libBox2D.a )
        SET(Box2D_LIBRARY_REL ${CMAKE_SOURCE_DIR}/thirdparty/Linux/Box2D/lib/libBox2D.a )
    ELSE (UNIX)
	    SET(Box2D_include "${CMAKE_SOURCE_DIR}/thirdparty/Win/Box2D/include" CACHE STRING "Location of Box2D/Box2D.h")
        INCLUDE_DIRECTORIES (${Box2D_include})
	    SET(Box2D_LIBRARY_DEBUG "${CMAKE_SOURCE_DIR}/thirdparty/Win/Box2D/lib/Box2D_d.lib" CACHE STRING "Location of Box2D_d.lib")
	    SET(Box2D_LIBRARY_REL "${CMAKE_SOURCE_DIR}/thirdparty/Win/Box2D/lib/Box2D.lib" CACHE STRING "Location of Box2D.lib")
    ENDIF(UNIX)
ENDIF (APPLE)

#### Lua #####
IF(APPLE)
    SET(LUA_INCLUDE_DIR thirdparty/macOS/Lua/include)
    INCLUDE_DIRECTORIES (${LUA_INCLUDE_DIR})
    SET(LUA_LIBRARIES ${CMAKE_SOURCE_DIR}/thirdparty/macOS/Lua/lib/liblua.a )
ELSE (APPLE)
    IF (UNIX)
        SET(LUA_INCLUDE_DIR /usr/include/lua5.3)
        INCLUDE_DIRECTORIES (${LUA_INCLUDE_DIR})
        SET(LUA_LIBRARIES /usr/lib/x86_64-linux-gnu/liblua5.3.a )
    ELSE (UNIX)
        SET(LUA_INCLUDE_DIR thirdparty/Win/Lua/include)
        INCLUDE_DIRECTORIES (${LUA_INCLUDE_DIR})
        SET(LUA_LIBRARIES ${CMAKE_SOURCE_DIR}/thirdparty/Win/Lua/lib/lua5.3.0-static.lib )
    ENDIF(UNIX)
ENDIF (APPLE)

#### Sol #####
INCLUDE_DIRECTORIES (thirdparty/Sol/include)

# build sre library
include_directories(include)
add_subdirectory(src)

# all_libs contains a list of all libraries that should be linked
set(all_libs ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2MIXER_LIBRARY} ${LUA_LIBRARIES} SRE debug ${Box2D_LIBRARY_DEBUG} optimized ${Box2D_LIBRARY_REL})

# Create DllFileList on Windows
IF (WIN32)
    SET(SDL2_dll "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.6/lib/x86/SDL2.dll" CACHE STRING "Where the SDL2.dll can be found")
    SET(SDL2_image_dll_path "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.1/lib/x86" CACHE PATH "Where the SDL2_image.dll can be found")
    SET(SDL2_mixer_dll_path "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_mixer-2.0.1/lib/x86" CACHE PATH "Where the SDL2_mixer.dll can be found")
    SET(DLLFileList "${SDL2_image_dll_path}/SDL2_image.dll" "${SDL2_image_dll_path}/libjpeg-9.dll" "${SDL2_image_dll_path}/libpng16-16.dll" "${SDL2_image_dll_path}/libtiff-5.dll" "${SDL2_image_dll_path}/libwebp-4.dll" "${SDL2_image_dll_path}/zlib1.dll" "${SDL2_mixer_dll_path}/SDL2_mixer.dll" "${SDL2_mixer_dll_path}/libvorbisfile-3.dll"  "${SDL2_mixer_dll_path}/libvorbis-0.dll"  "${SDL2_mixer_dll_path}/libogg-0.dll"  "${SDL2_mixer_dll_path}/libmodplug-1.dll"  "${SDL2_mixer_dll_path}/libFLAC-8.dll" ${SDL2_dll})
ENDIF(WIN32)

# build targets using sre
add_subdirectory(examples)
add_subdirectory(project)